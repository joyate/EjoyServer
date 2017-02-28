#include "InstanceMgr.h"
#include "PacketDefine.h"

InstanceMgrT sInstanceMgr;

#define INSTANCE_PACKET(PC) do { sInstanceMgr.AddInstance(PC::ID,new PC); } while(0)



template<>
InstanceMgr<IPacket>::InstanceMgr()
{
	Instance();
}


template<>
void InstanceMgr<IPacket>::Instance()
{
	INSTANCE_PACKET(LoginIn);
	INSTANCE_PACKET(LoginRes);
}

