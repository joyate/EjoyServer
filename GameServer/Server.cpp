#include "Server.h"
#include "Game_Define.h"

//test
#include "IDataBase.h"
#include "DataProxyPlugin.h"



Server::Server(void):
	m_db_ptr(NULL)
{
	
}


Server::~Server(void)
{
}

bool Server::Init()
{
	if (!Engine::Init())
	{
		return false;
	}


	m_dbPluginMgr.LoadPlugins("./plugins");
	CDataProxyPlugin* pPlugin = m_dbPluginMgr.GetPlugin("mysql");
	m_db_ptr = pPlugin->Create();
	bool bRet = m_db_ptr->Initialize("219.237.222.40",3306,"admin","zWIzjvnC","test");
	if (bRet)
	{
		printf("mysql init success\n");
	}

	m_Dispatcher.reset(new Dispatcher(*GetNextIOS(true)));
	m_Acceptor.reset(new Acceptor(*GetNextIOS(true)));
	m_dc.reset(new CDataCacheImpl(*GetNextIOS(true)));

	if (!m_Dispatcher->Init() || !m_Acceptor->Init(std::string("127.0.0.1"),std::string("7777")) || !m_dc->Initialize("127.0.0.1"))
	{
		return false;
	}

	//pPlugin->ReleasePlugin(m_db_ptr);

	return true;
}


void Server::Run()
{
	Engine::Run();
}

void Server::Release()
{

	if (m_db_ptr)
	{
		CDataProxyPlugin* plugin = m_dbPluginMgr.GetPlugin("mysql");
		plugin->ReleasePlugin(m_db_ptr);
	}

}
