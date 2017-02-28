#include "RecordSetImpl.h"
#include "FieldImpl.h"
#include <new>

RecordSetImpl::RecordSetImpl(MYSQL_RES* _res,char* _buff):
	m_res(_res),
	m_fd_buffer(_buff)
{
}


RecordSetImpl::~RecordSetImpl(void)
{
	if (m_res)
	{
		mysql_free_result(m_res);
	}
}


IField* RecordSetImpl::NextRow()
{
	if (!m_res || !m_fd_buffer)
	{
		return NULL;
	}


	MYSQL_ROW row = mysql_fetch_row(m_res);
	if (row == NULL)
	{
		return NULL;
	}


	MYSQL_FIELD* fields = mysql_fetch_fields(m_res);
	int fieldcnt = mysql_num_fields(m_res);
	return new(m_fd_buffer) FieldImpl(row,fields,fieldcnt);
	
}

