#pragma once
#include "IPacket.h"
#include "PacketIStream.h"
#include "PacketOStream.h"


class LoginIn : public IPacket
{
public:
	LoginIn() {
		memset(m_account,0,sizeof(m_account));
		m_pass = 0;
	}
	~LoginIn() {}

	enum { ID = 1 };

	virtual int GetID() const
	{
		return ID;
	}

	virtual bool UnSerialize(PacketIStream& packet);
	virtual bool Serialize(PacketOStream& packet);

//#ifdef _SERVER_
//	virtual int OnHandle(ConnectionPtr conn_ptr,GamePlayerPtr gp_ptr);
//#elif _CLIENT_
//	virtual int OnHandle();
//#endif


public:
	char m_account[32];
	int  m_pass;
};


class LoginRes : public IPacket
{
public:

	enum { ID = 2 };

	virtual int GetID() const
	{
		return ID;
	}

	virtual bool UnSerialize(PacketIStream& packet);
	virtual bool Serialize(PacketOStream& packet);

	
//#ifdef _SERVER_
//	virtual int OnHandle(ConnectionPtr conn_ptr,GamePlayerPtr gp_ptr);
//#elif _CLIENT_
//	virtual int OnHandle();
//#endif


	int res; // 0 failed, 1 success
};


