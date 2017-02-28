#include "net.h"

#ifdef WIN32

#include <windows.h>

#ifndef snprintf
#define snprintf _snprintf
#endif


#endif


/* Defined in hiredis.c */
void __redisSetError(redisContext *c, int type, const char *str);


static void __redisSetErrorFromErrno(redisContext *c, int type, const char *prefix) {
#ifdef _LINUX
	char buf[128] = { 0 };
	size_t len = 0;

	if (prefix != NULL)
		len = snprintf(buf,sizeof(buf),"%s: ",prefix);
	strerror_r(errno,buf+len,sizeof(buf)-len);
	__redisSetError(c,type,buf);

#elif WIN32

	char buf[128] = { 0 };
	size_t len = 0;

	if (prefix != NULL)
		len = snprintf(buf,sizeof(buf),"%s: ",prefix);
	strerror_r(errno,buf+len,sizeof(buf)-len);
	__redisSetError(c,type,buf);

#endif
}



static int redisSetReuseAddr(redisContext *c, SOCKET fd) {
#ifdef _LINUX
	int on = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1) {
		__redisSetErrorFromErrno(c,REDIS_ERR_IO,NULL);
		close(fd);
		return REDIS_ERR;
	}
#elif WIN32

	int opt=1;
	if(setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,(const char*)&opt,sizeof(opt)) == SOCKET_ERROR)
	{
		__redisSetErrorFromErrno(c,REDIS_ERR_IO,NULL);
		closesocket(fd);
		return REDIS_ERR;
	}
	

#endif
	return REDIS_OK;
}



static SOCKET redisCreateSocket(redisContext *c, int type) {

	SOCKET s;
	if ((s = socket(type, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		__redisSetErrorFromErrno(c,REDIS_ERR_IO,NULL);
		return REDIS_ERR;
	}
	if (type == AF_INET) {
		if (redisSetReuseAddr(c,s) == REDIS_ERR) {
			return REDIS_ERR;
		}
	}
	return s;
}


static int redisSetBlocking(redisContext *c, SOCKET fd, int blocking) {
    
	int flags;

#ifdef _LINUX
    /* Set the socket nonblocking.
     * Note that fcntl(2) for F_GETFL and F_SETFL can't be
     * interrupted by a signal. */
    if ((flags = fcntl(fd, F_GETFL)) == -1) {
        __redisSetErrorFromErrno(c,REDIS_ERR_IO,"fcntl(F_GETFL)");
        close(fd);
        return REDIS_ERR;
    }

    if (blocking)
        flags &= ~O_NONBLOCK;
    else
        flags |= O_NONBLOCK;

    if (fcntl(fd, F_SETFL, flags) == -1) {
        __redisSetErrorFromErrno(c,REDIS_ERR_IO,"fcntl(F_SETFL)");
        close(fd);
        return REDIS_ERR;
    }

#elif WIN32

	if(blocking)
		flags = 0;
	else
		flags = 1;

	if ((flags = ioctlsocket(fd, FIONBIO, (unsigned long *) &flags)) == -1) {
		__redisSetErrorFromErrno(c,REDIS_ERR_IO,"ioctlsocket FIONBIO");
		closesocket(fd);
		return REDIS_ERR;
	}

#endif

    return REDIS_OK;
}


static int redisSetTcpNoDelay(redisContext *c, SOCKET fd) {

	int yes = 1;
	if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY,
#ifdef _LINUX
		&yes,
#elif WIN32
		(char*)&yes,
#endif
		sizeof(yes)) == SOCKET_ERROR) {
		__redisSetErrorFromErrno(c,REDIS_ERR_IO,"setsockopt(TCP_NODELAY)");
#ifdef _LINUX
		close(fd);
#elif WIN32
		closesocket(fd);
#endif
		return REDIS_ERR;
	}
	return REDIS_OK;
}


#define __MAX_MSEC (((LONG_MAX) - 999) / 1000)

static int redisContextWaitReady(redisContext *c, SOCKET fd, const struct timeval *timeout) {
	
#ifdef _LINUX

	struct pollfd   wfd[1];
	long msec;

	msec          = -1;
	wfd[0].fd     = fd;
	wfd[0].events = POLLOUT;

	/* Only use timeout when not NULL. */
	if (timeout != NULL) {
		if (timeout->tv_usec > 1000000 || timeout->tv_sec > __MAX_MSEC) {
			__redisSetErrorFromErrno(c, REDIS_ERR_IO, NULL);
			close(fd);
			return REDIS_ERR;
		}

		msec = (timeout->tv_sec * 1000) + ((timeout->tv_usec + 999) / 1000);

		if (msec < 0 || msec > INT_MAX) {
			msec = INT_MAX;
		}
	}

	if (errno == EINPROGRESS) {
		int res;

		if ((res = poll(wfd, 1, msec)) == -1) {
			__redisSetErrorFromErrno(c, REDIS_ERR_IO, "poll(2)");
			close(fd);
			return REDIS_ERR;
		} else if (res == 0) {
			errno = ETIMEDOUT;
			__redisSetErrorFromErrno(c,REDIS_ERR_IO,NULL);
			close(fd);
			return REDIS_ERR;
		}

		if (redisCheckSocketError(c, fd) != REDIS_OK)
			return REDIS_ERR;

		return REDIS_OK;
	}

	__redisSetErrorFromErrno(c,REDIS_ERR_IO,NULL);
	close(fd);
	return REDIS_ERR;


#elif WIN32

	struct timeval to;
	struct timeval *toptr = NULL;
	fd_set wfd;
	int err;
	socklen_t errlen;

	/* Only use timeout when not NULL. */
	if (timeout != NULL) {
		to = *timeout;
		toptr = &to;
	}

	if (errno == WSAEINPROGRESS) {
		FD_ZERO(&wfd);
		FD_SET(fd, &wfd);

		if (select(FD_SETSIZE, NULL, &wfd, NULL, toptr) == SOCKET_ERROR) {
			__redisSetErrorFromErrno(c,REDIS_ERR_IO,"select(2)");
#ifdef _LINUX
			close(fd);
#elif WIN32
			closesocket(fd);
#endif
			return REDIS_ERR;
		}

		if (!FD_ISSET(fd, &wfd)) {
			errno = WSAETIMEDOUT;
			__redisSetErrorFromErrno(c,REDIS_ERR_IO,NULL);
#ifdef _LINUX
			close(fd);
#elif WIN32
			closesocket(fd);
#endif
			return REDIS_ERR;
		}

		err = 0;
		errlen = sizeof(err);
		if (getsockopt(fd, SOL_SOCKET, SO_ERROR, 
#ifdef _LINUX
			&err,
#elif WIN32
			(char*)&err,
#endif
			&errlen) == SOCKET_ERROR) {
			__redisSetErrorFromErrno(c,REDIS_ERR_IO,"getsockopt(SO_ERROR)");
#ifdef _LINUX
			close(fd);
#elif WIN32
			closesocket(fd);
#endif
			return REDIS_ERR;
		}

		if (err) {
			errno = err;
			__redisSetErrorFromErrno(c,REDIS_ERR_IO,NULL);
#ifdef _LINUX
			close(fd);
#elif WIN32
			closesocket(fd);
#endif
			return REDIS_ERR;
		}

		return REDIS_OK;
	}

	__redisSetErrorFromErrno(c,REDIS_ERR_IO,NULL);
#ifdef _LINUX
	close(fd);
#elif WIN32
	closesocket(fd);
#endif
	return REDIS_ERR;


#endif

}


int redisCheckSocketError( redisContext *c, int fd )
{
	int err = 0;
	socklen_t errlen = sizeof(err);

	if (getsockopt(fd, SOL_SOCKET, SO_ERROR, 
#ifdef _LINUX
		&err,
#elif WIN32
		(char*)&err,
#endif
		&errlen) == SOCKET_ERROR) {
		__redisSetErrorFromErrno(c,REDIS_ERR_IO,"getsockopt(SO_ERROR)");
#ifdef _LINUX
		close(fd);
#elif WIN32
		closesocket(fd);
#endif
		return REDIS_ERR;
	}

	if (err) {
		errno = err;
		__redisSetErrorFromErrno(c,REDIS_ERR_IO,NULL);
#ifdef _LINUX
		close(fd);
#elif WIN32
		closesocket(fd);
#endif
		return REDIS_ERR;
	}

	return REDIS_OK;

}

int redisContextSetTimeout( redisContext *c, const struct timeval tv )
{
	if (setsockopt(c->fd,SOL_SOCKET,SO_RCVTIMEO,
#ifdef _LINUX
		&tv,
#elif WIN32
		(char*)&tv,
#endif
		sizeof(tv)) == SOCKET_ERROR) {
		__redisSetErrorFromErrno(c,REDIS_ERR_IO,"setsockopt(SO_RCVTIMEO)");
		return REDIS_ERR;
	}
	if (setsockopt(c->fd,SOL_SOCKET,SO_SNDTIMEO,
#ifdef _LINUX
		&tv,
#elif WIN32
		(char*)&tv,
#endif
		sizeof(tv)) == SOCKET_ERROR) {
		__redisSetErrorFromErrno(c,REDIS_ERR_IO,"setsockopt(SO_SNDTIMEO)");
		return REDIS_ERR;
	}
	return REDIS_OK;
}

int redisContextConnectTcp( redisContext *c, const char *addr, int port, const struct timeval *timeout )
{
#ifdef _LINUX
	int s, rv;
    char _port[6];  /* strlen("65535"); */
    struct addrinfo hints, *servinfo, *p;
    int blocking = (c->flags & REDIS_BLOCK);

    snprintf(_port, 6, "%d", port);
    memset(&hints,0,sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    /* Try with IPv6 if no IPv4 address was found. We do it in this order since
     * in a Redis client you can't afford to test if you have IPv6 connectivity
     * as this would add latency to every connect. Otherwise a more sensible
     * route could be: Use IPv6 if both addresses are available and there is IPv6
     * connectivity. */
    if ((rv = getaddrinfo(addr,_port,&hints,&servinfo)) != 0) {
         hints.ai_family = AF_INET6;
         if ((rv = getaddrinfo(addr,_port,&hints,&servinfo)) != 0) {
            __redisSetError(c,REDIS_ERR_OTHER,gai_strerror(rv));
            return REDIS_ERR;
        }
    }
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((s = socket(p->ai_family,p->ai_socktype,p->ai_protocol)) == -1)
            continue;

        if (redisSetBlocking(c,s,0) != REDIS_OK)
            goto error;
        if (connect(s,p->ai_addr,p->ai_addrlen) == -1) {
            if (errno == EHOSTUNREACH) {
                close(s);
                continue;
            } else if (errno == EINPROGRESS && !blocking) {
                /* This is ok. */
            } else {
                if (redisContextWaitReady(c,s,timeout) != REDIS_OK)
                    goto error;
            }
        }
        if (blocking && redisSetBlocking(c,s,1) != REDIS_OK)
            goto error;
        if (redisSetTcpNoDelay(c,s) != REDIS_OK)
            goto error;

        c->fd = s;
        c->flags |= REDIS_CONNECTED;
        rv = REDIS_OK;
        goto end;
    }
    if (p == NULL) {
        char buf[128];
        snprintf(buf,sizeof(buf),"Can't create socket: %s",strerror(errno));
        __redisSetError(c,REDIS_ERR_OTHER,buf);
        goto error;
    }

error:
    rv = REDIS_ERR;
end:
    freeaddrinfo(servinfo);
    return rv;  // Need to return REDIS_OK if alright

#elif WIN32

	SOCKET s;
	int blocking = (c->flags & REDIS_BLOCK);
	struct sockaddr_in sa;

	if((s = redisCreateSocket(c,AF_INET)) < 0)
		return REDIS_ERR;
	if(redisSetBlocking(c,s,blocking) != REDIS_OK)
		return REDIS_ERR;

	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	if ( (sa.sin_addr.s_addr = inet_addr(addr)) == 0) {
		struct hostent *he;

		he = gethostbyname(addr);
		if (he == NULL) {
			char buf[128];
			snprintf(buf,sizeof(buf),"Can't resolve: %s", addr);
			__redisSetError(c,REDIS_ERR_OTHER,buf);
#ifdef _LINUX
			close(s);
#elif WIN32
			closesocket(s);
#endif
			return REDIS_ERR;
		}
		memcpy(&sa.sin_addr, he->h_addr, sizeof(struct in_addr));
	}

	if (connect(s, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR) {
		errno = WSAGetLastError();
		if (errno == WSAEINPROGRESS && !blocking) {
			/* This is ok. */
		} else {
			if (redisContextWaitReady(c,s,timeout) != REDIS_OK)
				return REDIS_ERR;
		}
	}

	/* Reset socket to be blocking after connect(2). */
	if (blocking && redisSetBlocking(c,s,1) != REDIS_OK)
		return REDIS_ERR;

	if (redisSetTcpNoDelay(c,s) != REDIS_OK)
		return REDIS_ERR;

	c->fd = s;
	c->flags |= REDIS_CONNECTED;
	return REDIS_OK;


#endif
}

int redisContextConnectUnix( redisContext *c, const char *path, const struct timeval *timeout )
{
#ifdef _LINUX
	int s;
	int blocking = (c->flags & REDIS_BLOCK);
	struct sockaddr_un sa;

	if ((s = redisCreateSocket(c,AF_LOCAL)) < 0)
		return REDIS_ERR;
	if (redisSetBlocking(c,s,0) != REDIS_OK)
		return REDIS_ERR;

	sa.sun_family = AF_LOCAL;
	strncpy(sa.sun_path,path,sizeof(sa.sun_path)-1);
	if (connect(s, (struct sockaddr*)&sa, sizeof(sa)) == -1) {
		if (errno == EINPROGRESS && !blocking) {
			/* This is ok. */
		} else {
			if (redisContextWaitReady(c,s,timeout) != REDIS_OK)
				return REDIS_ERR;
		}
	}

	/* Reset socket to be blocking after connect(2). */
	if (blocking && redisSetBlocking(c,s,1) != REDIS_OK)
		return REDIS_ERR;

	c->fd = s;
	c->flags |= REDIS_CONNECTED;
	return REDIS_OK;
#elif WIN32
	return REDIS_ERR;
#endif
}

int redisKeepAlive( redisContext *c, int interval )
{
	int val = 1;
	SOCKET fd = c->fd;


	if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE,
#ifdef _LINUX
		&val,
#elif WIN32
		(const char*)&val,
#endif
		sizeof(val)) == SOCKET_ERROR){

#ifdef _LINUX
		close(fd);
#elif WIN32
		closesocket(fd);
#endif
		__redisSetError(c,REDIS_ERR_OTHER,strerror(errno));
		return REDIS_ERR;
	}

#ifdef _OSX
	val = interval;
	if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPALIVE, &val, sizeof(val)) < 0) {
		__redisSetError(c,REDIS_ERR_OTHER,strerror(errno));
		return REDIS_ERR;
	}
#elif _LINUX
	val = interval;
	if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &val, sizeof(val)) < 0) {
		__redisSetError(c,REDIS_ERR_OTHER,strerror(errno));
		return REDIS_ERR;
	}

	val = interval/3;
	if (val == 0) val = 1;
	if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL, &val, sizeof(val)) < 0) {
		__redisSetError(c,REDIS_ERR_OTHER,strerror(errno));
		return REDIS_ERR;
	}

	val = 3;
	if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, &val, sizeof(val)) < 0) {
		__redisSetError(c,REDIS_ERR_OTHER,strerror(errno));
		return REDIS_ERR;
	}
#endif
	 return REDIS_OK;
}


#ifndef strerror_r
char * strerror_r( int errnum, char *buf, size_t buflen )
{
	int pos = snprintf(buf,buflen,"%d",errnum);
	return buf+pos;
}
#endif



