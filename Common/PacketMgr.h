#pragma once

#include "IPacket.h"
#include <map>

template<typename T>
class PacketMgr
{
public:
	PacketMgr(void);
	~PacketMgr(void) {}



	void AddPacket(unsigned short _msg_id,T* _pack)
	{
		m_mapPacket.insert(std::make_pair(_msg_id,_pack));
	}

	T* GetPacket(unsigned short _msg_id)
	{
		std::map<unsigned short,T*>::iterator _iter = m_mapPacket.find(_msg_id);
		if (_iter != m_mapPacket.end())
		{
			return _iter->second;
		}

		return NULL;
	}

	void Instance();

private:
	std::map<unsigned short,T*> m_mapPacket;
};


typedef PacketMgr<IPacket> PacketMgrT;
extern PacketMgrT sPacketMgr;













