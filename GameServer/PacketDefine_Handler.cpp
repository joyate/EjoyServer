#include "PacketDefine_Handler.h"
#include "Logic/Player.h"
#include "Server.h"



int LoginIn_Handler::OnHandle( ConnectionPtr conn_ptr,GamePlayerPtr gp_ptr,IPacket* packet)
{
	int pID = conn_ptr->GetPlayerID();
	LoginIn* pLogin = reinterpret_cast<LoginIn*>(packet);

	printf("test login in\n");
	LoginRes res;
	res.res = 0;

	if (!strcmp(pLogin->m_account,"test") && pLogin->m_pass == 123456)
	{
		printf("login success\n");
		res.res = 1;
	}

	Player* pPlayer = new Player(1);
	pPlayer->Load(svr.m_dc);


	PacketOStream pos(conn_ptr->GetSendBuffer(),PACKET_WRITE_BUFFER);
	res.Serialize(pos);

	unsigned short len = pos.EncodeHeader();
	conn_ptr->SendPacket(len);

	return 0;
}





