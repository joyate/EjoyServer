#pragma once
#include "Dll_Common.h"
#include "IRecordSet.h"

class DLL_EXPORT IDataBase
{
public:
	virtual ~IDataBase() {};




	/*
	 * @desc 初始化数据库
	 * @param szIp 数据库ip地址
	 * @param port 数据库端口
	 * @param szUser 数据库用户名
	 * @param szPass 数据库密码
	 * @param db 所用数据库名
	 * @return 连接成功返回true,连接失败返回false
	 *	
	 */
	virtual bool Initialize(const char* szIp,int port,const char* szUser,const char* szPass,const char* db) = 0;


	/*
	 * @desc 同步查询
	 * @param sql 查询sql语句
	 * @return 返回结果集
	 *	
	 */
	virtual IRecordSet* Query(const char* sql) = 0;


	/*
	 * @desc 无返回结果的sql语句，
	 * @param sql sql语句
	 * @return 影响行
	 * @mark 该接口用于执行sql语句，无结果集的那种，update,insert,delete
	 */
	virtual int DirectQuery(const char* sql) = 0;



	/*
	 *	@desc 关闭数据库连接
	 */
	virtual void Close() = 0;

};