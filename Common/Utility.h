#pragma once
#include "Type_Common.h"
#include <vector>
#include <string>
#include <assert.h>

namespace Utility
{




	/*
	 *	@desc 获取指定目录下的以ext_filter为后缀的文件，放到_vecFiles
	 *	@param dir 指定目录
	 *	@param [in/out] _vecFiles 搜到的文件放入该容器
	 *	@param ext_filter 搜索该参数指定的后缀文件
	 *	@return 返回找到的文件个数
	 */
	int GetFilesWithExtInDir(const char* dir,std::vector<std::string>& _vecFiles,const char* ext_filter);


	/*
	 *	@desc 将二进制转换成字符串
	 *	@param pData 二进制数据
	 *	@param nLength 二进制数据长度
	 *	@param pBuff 输出字符串缓存
	 *	@return 返回字符串长度
	 */
	int Hex(void* pData,int nLength,char* pBuff);



	/*
	 * @desc 将16进制字符串转换成2进制数据
	 * @param pBuff 16进制字符串
	 * @param nLength 16进制字符串长度
	 * @param pData 输出2进制数据缓存
	 * @return 2进制长度
	 *	
	 */
	int UnHex(char* pBuff,int nLength,byte* pData);



	/*
	 *	@desc 转换成16进制字符
	 *	@param _from 待转换的数字
	 *	@return 
	 */
	inline char TranHex(byte _from)
	{
		switch (_from)
		{
		case 0:
			return '0';
		case 1:
			return '1';
		case 2:
			return '2';
		case 3:
			return '3';
		case 4:
			return '4';
		case 5:
			return '5';
		case 6:
			return '6';
		case 7:
			return '7';
		case 8:
			return '8';
		case 9:
			return '9';
		case 10:
			return 'A';
		case 11:
			return 'B';
		case 12:
			return 'C';
		case 13:
			return 'D';
		case 14:
			return 'E';
		case 15:
			return 'F';
		default:
			assert(false && "invalid num");
			return 0;
		}
	}
	
	/*
	 *	@desc 将16进制字符转换成数字
	 */
	inline byte TranUnHex(char _from)
	{
		switch (_from)
		{
		case '0':
			return 0;
		case '1':
			return 1;
		case '2':
			return 2;
		case '3':
			return 3;
		case '4':
			return 4;
		case '5':
			return 5;
		case '6':
			return 6;
		case '7':
			return 7;
		case '8':
			return 8;
		case '9':
			return 9;
		case 'A':
			return 10;
		case 'B':
			return 11;
		case 'C':
			return 12;
		case 'D':
			return 13;
		case 'E':
			return 14;
		case 'F':
			return 15;
		default:
			assert(false && "invalid char");
			return 0;
		}
	}

}

