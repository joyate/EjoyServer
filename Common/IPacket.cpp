#include "IPacket.h"

#define PACKET_LENGTH_MAX 64*1024

IPacket::IPacket(void)
{
	m_buff.resize(PACKET_LENGTH_MAX);
}


IPacket::~IPacket(void)
{
}

