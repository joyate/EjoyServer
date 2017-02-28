#pragma once

#include "IPacket.h"
#include <map>

template<typename T>
class InstanceMgr
{
public:
	InstanceMgr(void);
	~InstanceMgr(void) {}



	void AddInstance(unsigned short _msg_id,T* _pack)
	{
		m_mapInstance.insert(std::make_pair(_msg_id,_pack));
	}

	T* GetInstance(unsigned short _msg_id)
	{
		std::map<unsigned short,T*>::iterator _iter = m_mapInstance.find(_msg_id);
		if (_iter != m_mapInstance.end())
		{
			return _iter->second;
		}

		return NULL;
	}

	void Instance();

private:
	std::map<unsigned short,T*> m_mapInstance;
};


typedef InstanceMgr<IPacket> InstanceMgrT;
extern InstanceMgrT sInstanceMgr;













