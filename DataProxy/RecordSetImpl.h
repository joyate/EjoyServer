#pragma once

#ifdef WIN32
#include <windows.h>
#endif

#include "IRecordSet.h"
#include "IField.h"
#include "mysql.h"

class RecordSetImpl : public IRecordSet
{
public:
	RecordSetImpl(MYSQL_RES* _res,char* _buff);
	virtual ~RecordSetImpl(void);



	virtual int GetRowCount()
	{
		if(m_res)
			return (int)mysql_num_rows(m_res);
		return 0;
	}


	virtual	IField* NextRow();


protected:

	int GetFieldCount()
	{
		if(m_res)
			return mysql_num_fields(m_res);
		return 0;
	}


private:
	MYSQL_RES* m_res;
	char* m_fd_buffer;
};

