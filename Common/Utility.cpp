#include "Utility.h"

#ifdef WIN32
#include <windows.h>
#endif

#define _HIBYTE(x)			(byte(x) >> 4 & 0x0f)
#define _LOBYTE(x)			(byte(x) & 0x0f)
#define _TOBYTE(hb,lb)		((((byte)(hb) & 0x0f) << 4) | (byte)(lb) & 0x0f)


#ifdef WIN32

int Utility::GetFilesWithExtInDir( const char* dir,std::vector<std::string>& _vecFiles,const char* ext_filter)
{

	char newdir[128] = {0};
	sprintf_s(newdir,"%s/%s",dir,"*");

	WIN32_FIND_DATAA wfd; 
	HANDLE hFind = FindFirstFileA(newdir, &wfd); 
	if (hFind == INVALID_HANDLE_VALUE) // 如果没有找到或查找失败 
		return 0;
	
	do
	{
		if (!strcmp(wfd.cFileName, ".") || !strcmp(wfd.cFileName, "..") )
		{
			continue;
		}

		if(wfd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{  
			//找到目录
			char nextdir[MAX_PATH] = {0};
			sprintf_s(nextdir,"%s/%s/",dir,wfd.cFileName);

			GetFilesWithExtInDir(nextdir,_vecFiles,ext_filter);
		}
		else
		{
			const char* pext = strrchr(wfd.cFileName,'.');
			if (!_stricmp(pext,ext_filter))
			{
				char fullPath[MAX_PATH] = {0};
				sprintf_s(fullPath,"%s/%s",dir,wfd.cFileName);
				_vecFiles.push_back(fullPath);
			}	
		}

	}while(FindNextFileA(hFind, &wfd));

	FindClose(hFind);

	return (int)_vecFiles.size();


}

#elif _LINUX


int Utility::GetFilesWithExtInDir( const char* dir,std::vector<std::string>& _vecFiles,const char* ext_filter)
{
	return 0;
}








#endif


int Utility::Hex( void* pData,int nLength,char* pBuff )
{
	int nCnt = 0;
	for (int i = 0; i < nLength; i++)
	{
		byte ch = *((byte*)pData + i);

		pBuff[nCnt++] = TranHex(_LOBYTE(ch));
		pBuff[nCnt++] = TranHex(_HIBYTE(ch));
	}

	return nCnt;
}

int Utility::UnHex( char* pBuff,int nLength,byte* pData )
{
	int nCnt = 0;
	int i = 0;
	for (; i < nLength - 1; )
	{

		byte ch1 = TranUnHex(*(pBuff + (i++)));
		byte ch2 = TranUnHex(*(pBuff + (i++)));
		pData[nCnt++] = _TOBYTE(ch2,ch1);
	}
	assert(i == nLength);
	return nCnt;
}




