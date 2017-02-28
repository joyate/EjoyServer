#pragma once
#include "PacketDefine.h"
#include "InstanceMgr.h"

#include "Connection.h"
#include "GamePlayer.h"

/*
* 消息包handler设计，通过代理方式实属无奈，由于前后端handler的参数不一样。
* 故在外面再封装一层，已适配前后端handler的需求
*/



#define PACKET_HANDLER(_class)															\
class _class##_Handler																	\
{																						\
public:																					\
	static int OnHandle(ConnectionPtr conn_ptr,GamePlayerPtr gp_ptr,IPacket* packet);	\
}

typedef int (*packet_handler_ptr)(ConnectionPtr conn_ptr,GamePlayerPtr gp_ptr,IPacket* packet);

class Packet_Handler_Proxy
{
public:
	Packet_Handler_Proxy(packet_handler_ptr handler_func):
		m_func(handler_func)
	{}


	int operator()(ConnectionPtr conn_ptr,GamePlayerPtr gp_ptr,IPacket* packet)
	{
		return (*m_func)(conn_ptr,gp_ptr,packet);
	}

private:
	packet_handler_ptr m_func;
};

typedef InstanceMgr<Packet_Handler_Proxy> HandlerMgrT;
extern HandlerMgrT sHandlerMgr;

// Server Handler 定义
PACKET_HANDLER(LoginIn);
