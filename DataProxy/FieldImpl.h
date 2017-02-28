#pragma once
#include "IField.h"
#include <assert.h>

#ifdef WIN32
#include <windows.h>
#endif

#include "mysql.h"

#define FIELD_NAME_MATCH_MAX 12

class FieldImpl : public IField
{
public:
	FieldImpl(MYSQL_ROW _row,MYSQL_FIELD* _fields,int nFieldCnt);
	virtual ~FieldImpl(void);


	virtual int				GetInt(int _idx)		
	{ 
		if(m_row)
		{
			assert(_idx < m_fdcnt);
			return atoi(m_row[_idx]);
		}
		
		return 0;
	}
	virtual float			GetFloat(int _idx)
	{
		if(m_row)
		{
			assert(_idx < m_fdcnt);
			return (float)atof(m_row[_idx]);
		}

		return 0;
	}
	virtual double			GetDouble(int _idx)
	{
		if(m_row)
		{
			assert(_idx < m_fdcnt);
			return atof(m_row[_idx]);
		}

		return 0;
	}
	virtual char			GetChar(int _idx)
	{
		if(m_row)
		{
			assert(_idx < m_fdcnt);
			return (char)atoi(m_row[_idx]);
		}

		return 0;
	}
	virtual byte			GetByte(int _idx)
	{
		if(m_row)
		{
			assert(_idx < m_fdcnt);
			return (byte)atoi(m_row[_idx]);
		}

		return 0;
	}
	virtual const char*		GetString(int _idx)
	{
		if(m_row)
		{
			assert(_idx < m_fdcnt);
			return m_row[_idx];
		}

		return NULL;
	}

	
	virtual int				GetInt(const char* szField)
	{
		int idx = GetIndex(szField);
		return GetInt(idx);
	}
	virtual float			GetFloat(const char* szField)
	{
		int idx = GetIndex(szField);
		return GetFloat(idx);
	}
	virtual	double			GetDouble(const char* szField)
	{
		int idx = GetIndex(szField);
		return GetDouble(idx);
	}
	virtual char			GetChar(const char* szField)
	{
		int idx = GetIndex(szField);
		return GetChar(idx);
	}
	virtual byte			GetByte(const char* szField)
	{
		int idx = GetIndex(szField);
		return GetByte(idx);
	}
	virtual const char*		GetString(const char* szField)
	{
		int idx = GetIndex(szField);
		return GetString(idx);
	}

	virtual int				GetFieldCnt()
	{
		return m_fdcnt;
	}

protected:

	int GetIndex(const char* szField)
	{
		for (int i = 0; i < m_fdcnt; i++)
		{
			if (!_strnicmp(szField,m_fields[i].name,FIELD_NAME_MATCH_MAX))
			{
				return i;
			}
		}

		return m_fdcnt;
	}


private:
	MYSQL_ROW m_row;
	MYSQL_FIELD* m_fields;
	int m_fdcnt;
};

