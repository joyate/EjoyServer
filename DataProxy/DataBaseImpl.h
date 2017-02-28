#pragma once

#ifdef WIN32
#include <windows.h>
#endif

#include "IDataBase.h"
#include "mysql.h"



class DLL_EXPORT DataBaseImpl :
	public IDataBase
{
public:
	DataBaseImpl(void);
	virtual ~DataBaseImpl(void);


	virtual bool Initialize(const char* szIp,int port,const char* szUser,const char* szPass,const char* db);


	virtual IRecordSet* Query(const char* sql);



	virtual int DirectQuery(const char* sql);


	virtual void Close();

private:
	MYSQL* m_mysql;

	// »º´æ
	char* m_rs_buffer;
	char* m_fd_buffer;
};

