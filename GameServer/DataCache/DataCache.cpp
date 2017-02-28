#include "DataCache.h"
#include "hiredis.h"

#include "boost/bind.hpp"

CDataCacheImpl::CDataCacheImpl( boost::asio::io_service& _ios ):
	m_ios(_ios),
	m_redis_ct(NULL)
{

}



bool CDataCacheImpl::Initialize( const char* dc_ip,int port )
{
	redisContext* pContext = redisConnect(dc_ip,port);

	if (pContext != NULL && pContext->err)
	{
		printf_s("error:%s \n",pContext->errstr);
		return false;
	}

	m_redis_ct = pContext;
	return true;
}




int CDataCacheImpl::Get( const char* key, char* data, int& nlen)
{
	if (!key)
	{
		return DCI_E_ERR;
	}

	redisReply* reply = (redisReply*)redisCommand(m_redis_ct,"GET %s",key);
	if (reply)
	{
		if (reply->type == REDIS_REPLY_STRING)
		{
			strcpy(data,reply->str);
			nlen = reply->len;
			freeReplyObject(reply);
			return DCI_E_SUCC;
		}

	}

	
	return DCI_E_NONE;
}

int CDataCacheImpl::Set( const char* key,const char* data )
{
	if (!key)
	{
		return DCI_E_ERR;
	}

	redisReply* reply = (redisReply*)redisCommand(m_redis_ct,"SET %s %s",key,data);
	if (reply)
	{
		if (reply->type == REDIS_REPLY_STATUS && !stricmp(reply->str,"OK"))
		{
			freeReplyObject(reply);
			return DCI_E_SUCC;
		}

	}

	return DCI_E_ERR;
}



