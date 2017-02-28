#pragma once

#include "Game_Define.h"
#include "boost/asio.hpp"
#include "boost/array.hpp"





// 玩家实体类
class GamePlayer
{
public:
	GamePlayer(/*boost::asio::io_service& ios,*/int _id,int _conn_id);
	~GamePlayer(void);



	//
	int GetID()	{ return m_ID;}


	// 离开游戏
	int OnExit();

private:

	// Connection ID
	int m_ConnID;
	
	// 自己ID
	int m_ID;
};


typedef boost::shared_ptr<GamePlayer> GamePlayerPtr;
