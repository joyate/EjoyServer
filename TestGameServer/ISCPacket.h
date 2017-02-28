#pragma once

class PacketIStream;
class PacketOStream;

class ISCPacket
{
public:
	ISCPacket(void);
	virtual ~ISCPacket(void) = 0;


	enum { ID = 0 };

	// 获取包ID
	virtual int GetID() const = 0;

	// 反序列化
	virtual bool UnSerialize(PacketIStream& packet) = 0;

	// 序列化
	virtual bool Serialize(PacketOStream& packet) = 0;

	// 处理函数
	virtual int OnHandle() = 0;


};

