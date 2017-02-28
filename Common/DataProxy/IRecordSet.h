#pragma once
#include "Dll_Common.h"
#include "IField.h"



class DLL_EXPORT IRecordSet
{
protected:
	virtual ~IRecordSet() {};


public:

	/*
	 * @desc 获取结果集行数
	 * @return 行数
	 *	
	 */
	virtual int GetRowCount()	=	0;


	/*
	 * @desc 获取下一行
	 * @return 有下一行，返回true，否则false
	 *	
	 */
	virtual	IField* NextRow()		= 0;


};





