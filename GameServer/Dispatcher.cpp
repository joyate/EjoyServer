#include "Dispatcher.h"
#include "Server.h"
#include "boost/bind.hpp"
#include "InstanceMgr.h"
#include "PacketIStream.h"

#include "PacketDefine_Handler.h"



Dispatcher::Dispatcher(boost::asio::io_service& _ios):
	m_ios(_ios)
{
}


Dispatcher::~Dispatcher(void)
{
}

bool Dispatcher::Init()
{
	
	return true;
}

int Dispatcher::Dispatch( ConnectionPtr pConnPtr)
{
	unsigned short msgID;
	std::vector<char> Packet;
	bool bRet = pConnPtr->TranslateMsg(msgID,Packet);

	if (bRet)
	{
		m_ios.post(boost::bind(&Dispatcher::HandlePacket,shared_from_this(),pConnPtr,msgID,Packet));
	}

	return 0;
}

void Dispatcher::HandlePacket( ConnectionPtr pConnPtr,unsigned short msgID,std::vector<char> msgPack )
{
	GamePlayerPtr pPlayer;

	GPIter gp_ptr = svr.m_mapGamePlayerPtr.find(pConnPtr->GetPlayerID());
	if (gp_ptr != svr.m_mapGamePlayerPtr.end())
	{
		pPlayer = gp_ptr->second;
	}


	IPacket* pPacket = sInstanceMgr.GetInstance(msgID);
	pPacket->UnSerialize(PacketIStream(msgPack));
	
	Packet_Handler_Proxy* pHandler = sHandlerMgr.GetInstance(pPacket->GetID());
	int nRet = (*pHandler)(pConnPtr,pPlayer,pPacket);

	if (nRet)
	{

	}

}

void Dispatcher::HandleError( ConnectionPtr pConnPtr )
{
	GPIter gp_ptr = svr.m_mapGamePlayerPtr.find(pConnPtr->GetPlayerID());
	if (gp_ptr != svr.m_mapGamePlayerPtr.end())
	{
		gp_ptr->second->OnExit();

		svr.m_mapGamePlayerPtr.erase(pConnPtr->GetPlayerID());
	}


	// 去掉自己的
	svr.m_mapConnectionPtr.erase(pConnPtr->GetID());
}


void Dispatcher::OnError( ConnectionPtr pConnPtr )
{
	assert(pConnPtr && "connection error");

	m_ios.post(boost::bind(&Dispatcher::HandleError,shared_from_this(),pConnPtr));
}


