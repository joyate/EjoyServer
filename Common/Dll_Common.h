#pragma once
#include "stdio.h"


#ifdef _USRDLL

#define DLL_EXPORT	__declspec(dllexport)

#else

#define DLL_EXPORT	__declspec(dllimport)

#endif
