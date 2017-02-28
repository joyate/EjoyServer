#pragma once
#include <map>




/*
* 插件管理器
*
*
*/

class CDataProxyPlugin;

class CPluginMgr
{
public:
	CPluginMgr(void);
	~CPluginMgr(void);

	typedef std::map<std::string,CDataProxyPlugin*>::iterator mapDBPIter;

	/*
	 * @desc 载入dataproxy插件
	 * @param plugin_dir 插件所在目录
	 * @return 载入插件数目
	 *	
	 */
	int LoadPlugins(const char* plugin_dir);
	

	/*
	 * @desc 获得插件
	 * @param sz_plugin_name 插件名称
	 * @return dataproxy插件
	 *	
	 */
	CDataProxyPlugin* GetPlugin(const char* sz_plugin_name);


private:

	std::map<std::string,CDataProxyPlugin*> m_mapPlugins;
};

