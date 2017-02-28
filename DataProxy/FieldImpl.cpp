#include "FieldImpl.h"


FieldImpl::FieldImpl(MYSQL_ROW _row,MYSQL_FIELD* _fields,int nFieldCnt):
	m_row(_row),
	m_fields(_fields),
	m_fdcnt(nFieldCnt)
{


}


FieldImpl::~FieldImpl(void)
{
}
