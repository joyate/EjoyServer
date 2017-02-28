#pragma once
#include "Dll_Common.h"
#include "Type_Common.h"

class DLL_EXPORT IField
{
protected:
	IField(){}
	virtual ~IField() {};



public:

	virtual int				GetInt(int _idx)	= 0;
	virtual float			GetFloat(int _idx)	= 0;
	virtual double			GetDouble(int _idx) = 0;
	virtual char			GetChar(int _idx)	= 0;
	virtual byte			GetByte(int _idx)	= 0;
	virtual const char*		GetString(int _idx) = 0;

	
	virtual int				GetInt(const char* szField)		= 0;
	virtual float			GetFloat(const char* szField)	= 0;
	virtual	double			GetDouble(const char* szField)	= 0;
	virtual char			GetChar(const char* szField)	= 0;
	virtual byte			GetByte(const char* szField)	= 0;
	virtual const char*		GetString(const char* szField)	= 0;



	virtual int				GetFieldCnt() = 0;
};






