#pragma once
#include "InstanceMgr.h"
#include "PacketDefine.h"

/*
* 客户端Handler的定义
*
*/


#define PACKET_HANDLER(_class)															\
class _class##_Handler																	\
{																						\
public:																					\
	static int OnHandle(IPacket* packet);												\
}

typedef int (*packet_handler_ptr)(IPacket* packet);

class Packet_Handler_Proxy
{
public:
	Packet_Handler_Proxy(packet_handler_ptr handler_func):
		m_func(handler_func)
	{}


	int operator()(IPacket* packet)
	{
		return (*m_func)(packet);
	}

private:
	packet_handler_ptr m_func;
};

typedef InstanceMgr<Packet_Handler_Proxy> HandlerMgrT;
extern HandlerMgrT sHandlerMgr;

// Client Handler 定义
PACKET_HANDLER(LoginRes);