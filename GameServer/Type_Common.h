#pragma once


#ifdef _WIN32

	typedef int INT;
	typedef __int64 INT64;

	typedef unsigned char byte;

	//typedef unsigned short WORD;

#elif _LINUX

	typedef int				INT;
	typedef _int_ptr		HANDLE;

	typedef unsigned short	WORD;
	typedef unsigned int	DWORD;

#endif