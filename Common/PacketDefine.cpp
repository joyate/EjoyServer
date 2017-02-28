#include "PacketDefine.h"


bool LoginIn::UnSerialize( PacketIStream& packet )
{
	unsigned short _id;	
	packet >> _id;

	packet >> m_account;
	packet >> m_pass;

	return true;
}

bool LoginIn::Serialize( PacketOStream& packet )
{

	packet << (short)ID;

	packet << m_account;
	packet << m_pass;

	return true;
}


bool LoginRes::UnSerialize( PacketIStream& packet )
{
	unsigned short _id;	
	packet >> _id;
	packet >> res;

	return true;

}

bool LoginRes::Serialize( PacketOStream& packet )
{
	packet << (short)ID;
	packet << res;

	return true;
}



