#pragma once
#include <vector>
#include "Type_Common.h"


/*
* Packet的设计原则，
* 服务器和客户端统一IPacket接口，但是处理接口不同，因为服务器和客户端需要的参数不同
*
* Packet的序列化，反序列化中msgID基本无用处，但是为了对称，还是相对统一处理
*
* 前后端包定义共用一个声明文件，在序列化与反序列化上共用，在处理上不一样。在前后端分开处理
* 通过前后端中的PacketDefine_Handler.cpp定义。
*/



class PacketIStream;
class PacketOStream;


class IPacket
{
public:
	IPacket(void);
	virtual ~IPacket(void) = 0;

	enum { ID = 0 };

	// 获取包ID
	virtual int GetID() const = 0;

	// 反序列化
	virtual bool UnSerialize(PacketIStream& packet) = 0;
	
	// 序列化
	virtual bool Serialize(PacketOStream& packet) = 0;

	// 处理函数
//#ifdef _SERVER_
//	virtual int OnHandle(ConnectionPtr conn_ptr,GamePlayerPtr gp_ptr) = 0;
//#elif _CLIENT_
//	virtual int OnHandle() = 0;
//#endif


private:
	std::vector<byte> m_buff;
};

