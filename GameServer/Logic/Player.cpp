#include "Player.h"
#include "../Game_Define.h"
#include "../DataCache/DataCache.h"

#include "IDataBase.h"
#include "DataProxy/IField.h"
#include "DataProxy/IRecordSet.h"
#include "Utility.h"
#include "../Server.h"

#define DATACACHE_BUFFER_MAX 8*1024


Player::Player( int _id ):
	m_nID(_id)
{

}


int Player::_LoadFromDC( dc_ptr _cache )
{

	if (!_cache || m_nID == GameDefine::INVALID_ID)
	{
		return -1;
	}

	char temp[DATACACHE_BUFFER_MAX] = {0};
	char key[64] = {0};
	int len = 0;
	sprintf(key,"Server:%d:Player:%d",1,m_nID);
	int nRet = _cache->Get(key,temp,len);
	if (nRet)
	{
		return nRet;
	}

	len = Utility::UnHex(temp,len,(byte*)&m_Data);
	assert(len > 0 && "data wrong");

	return 0;
}

int Player::_LoadFromDB( dc_ptr _cache )
{
	IDataBase* pDB = svr.GetDBConn();
	if (!pDB)
	{
		return -1;
	}

	char sql[256] = {0};
	sprintf(sql,"select hp,mp,map_id,money from Player where PlayerID = %d",m_nID);
	IRecordSet* rs = pDB->Query(sql);

	IField* field = rs->NextRow();
	
	if(field)
	{
		m_Data.hp = field->GetInt(1);
		m_Data.mp = field->GetInt(2);
		m_Data.mapID = field->GetInt(3);
		m_Data.money = field->GetInt(4);

	}
	else
	{
		m_Data.hp = 100;
		m_Data.mp = 100;
		m_Data.mapID = 1;
		m_Data.money = 0;

		sprintf(sql,"insert into Player(PlayerID,hp,mp,map_id,money) values(%d,%d,%d,%d,%d)",m_nID,m_Data.hp,m_Data.mp,m_Data.mapID,m_Data.money);
		int nRet = pDB->DirectQuery(sql);

		assert(nRet > 0 && "insert failed");
	}

	char temp[DATACACHE_BUFFER_MAX] = {0};
	char key[64] = {0};
	sprintf(key,"Server:%d:Player:%d",1,m_nID);

	int nlen = Utility::Hex((byte*)&m_Data,sizeof(m_Data),temp);
	_cache->Set(key,temp);

	return 0;
}

int Player::Load( dc_ptr _cache )
{
	if (!_cache)
	{
		return -1;
	}

	int nRet = _LoadFromDC(_cache);

	if (nRet)
	{
		_cache->PostLoad(this,&Player::_LoadFromDB);
	}

	return 0;
}



