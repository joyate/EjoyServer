#pragma once
#include "IDataBase.h"
#ifdef WIN32
#include <windows.h>
#endif


typedef IDataBase* (*DataBaseCreateFunc)();
typedef const char* (*DataBaseNameGetFunc)();
typedef void (*DataBaseRelease)(IDataBase*);


class CDataProxyPlugin
{
public:
	CDataProxyPlugin(void);
	~CDataProxyPlugin(void);


	bool LoadPlugin(const char* sz_plugin_name);

	/*
	 * @desc 创建资源
	 * @return 资源指针，失败返回NULL
	 *	
	 */
	IDataBase* Create();

	/*
	 * @desc 获取插件名称
	 * @return 插件名称
	 *	
	 */
	const char* GetPluginName();

	/*
	 * @desc 释放插件资源
	 * @param 资源指针
	 * @return 
	 *	
	 */
	void ReleasePlugin(IDataBase* _db_plugin);

private:
#ifdef WIN32
	HMODULE m_module;
#elif _LINUX
	void* m_soptr;
#endif

DataBaseCreateFunc	m_db_create_func;
DataBaseNameGetFunc m_dbn_get_func;
DataBaseRelease		m_db_release_func;
};

