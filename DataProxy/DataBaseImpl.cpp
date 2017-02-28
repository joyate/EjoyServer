#include "DataBaseImpl.h"
#include "RecordSetImpl.h"
#include "FieldImpl.h"
#include <new>

DataBaseImpl::DataBaseImpl(void):
	m_mysql(NULL)
{
}


DataBaseImpl::~DataBaseImpl(void)
{
	//Close();
}

bool DataBaseImpl::Initialize( const char* szIp,int port,const char* szUser,const char* szPass,const char* db )
{

	m_mysql = mysql_init(NULL);
	if (!m_mysql)
	{
		return false;
	}

	if(!mysql_real_connect(m_mysql,szIp,szUser,szPass,db,port,NULL,0))
	{
		return false;
	}


	m_rs_buffer = new char[sizeof(RecordSetImpl)];
	m_fd_buffer = new char[sizeof(FieldImpl)];

	return true;
}

IRecordSet* DataBaseImpl::Query( const char* sql )
{

	if(mysql_query(m_mysql,sql))
	{
		printf("%s\n",mysql_error(m_mysql));
		return NULL;
	}

	MYSQL_RES* res = mysql_store_result(m_mysql);
	if (!res)
	{
		printf("%s\n",mysql_error(m_mysql));
		return NULL;
	}

	return new(m_rs_buffer) RecordSetImpl(res,m_fd_buffer);

}

void DataBaseImpl::Close()
{
	if (m_mysql)
	{
		mysql_close(m_mysql);
		m_mysql = NULL;
	}

	if (m_rs_buffer)
	{
		delete[] m_rs_buffer;
		m_rs_buffer = NULL;
	}

	if (m_fd_buffer)
	{
		delete[] m_fd_buffer;
		m_fd_buffer = NULL;
	}
}



int DataBaseImpl::DirectQuery( const char* sql )
{
	if(mysql_query(m_mysql,sql))
	{
		printf("%s\n",mysql_error(m_mysql));
		return -1;
	}


	my_ulonglong af_row = (my_ulonglong)mysql_affected_rows(m_mysql);
	if (af_row == (my_ulonglong)-1)	// ³ö´í
	{
		printf("%s\n",mysql_error(m_mysql));
		return -1;
	}
	

	return (int)af_row;
}
