#include "PluginMgr.h"
#include "Utility.h"
#include "DataProxyPlugin.h"

CPluginMgr::CPluginMgr(void)
{
}


CPluginMgr::~CPluginMgr(void)
{
}

int CPluginMgr::LoadPlugins( const char* plugin_dir )
{
	std::vector<std::string> _vecFiles;
#ifdef WIN32
	int nCnt = Utility::GetFilesWithExtInDir(plugin_dir,_vecFiles,".dll");
#elif _LINUX
	int nCnt = Utility::GetFilesWithExtInDir(plugin_dir,_vecFiles,".so");
#endif
	if (nCnt > 0)
	{
		std::vector<std::string>::iterator _iter = _vecFiles.begin();
		for (;_iter != _vecFiles.end(); _iter++)
		{
			CDataProxyPlugin* pPlugin = new CDataProxyPlugin;
			pPlugin->LoadPlugin(_iter->c_str());
			m_mapPlugins.insert(std::make_pair(pPlugin->GetPluginName(),pPlugin));
		}
	}

	return 0;
}

CDataProxyPlugin* CPluginMgr::GetPlugin( const char* sz_plugin_name )
{
	mapDBPIter _iter = m_mapPlugins.find(sz_plugin_name);
	
	if (_iter != m_mapPlugins.end())
	{
		return _iter->second;
	}

	return NULL;
}
