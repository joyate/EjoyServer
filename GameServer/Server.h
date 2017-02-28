#pragma once

#include "Engine.h"
#include "Dispatcher.h"
#include "Acceptor.h"
#include "Connection.h"
#include "GamePlayer.h"

#include "PluginMgr.h"
#include "DataCache/DataCache.h"
#include "DataProxyPlugin.h"

class IDataBase;

class Server : public Engine<GameDefine::GAME_IO_SERVICE_MAX>
{
public:
	Server(void);
	~Server(void);



	// ≥ı ºªØ
	virtual bool Init();

	// 
	virtual void Run();


	boost::shared_ptr<Acceptor> GetAcceptor() { return m_Acceptor;}

	//
	IDataBase* GetDBConn() { return m_db_ptr;}

	//
	void Release();
	

public:
	std::map<int,ConnectionPtr> m_mapConnectionPtr;
	std::map<int,GamePlayerPtr> m_mapGamePlayerPtr;
	boost::shared_ptr<Dispatcher> m_Dispatcher;
	boost::shared_ptr<CDataCacheImpl> m_dc;
private:

	IDataBase* m_db_ptr;
	boost::shared_ptr<Acceptor> m_Acceptor;
	CPluginMgr m_dbPluginMgr;
	
};


typedef std::map<int,GamePlayerPtr>::iterator GPIter;
extern Server svr;