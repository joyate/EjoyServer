#include "PacketDefine.h"

// 
#define DEFAULT_IMPL_PACKET(class) \
	int class::OnHandle(ConnectionPtr conn_ptr,GamePlayerPtr gp_ptr) { return 0;}

// 客户端实现的handle,都是默认的空实现
DEFAULT_IMPL_PACKET(LoginRes);






// 本地实现的handle
int LoginIn::OnHandle( ConnectionPtr conn_ptr,GamePlayerPtr gp_ptr )
{
	int pID = conn_ptr->GetPlayerID();

	printf("test login in");
	LoginRes res;
	res.res = 0;

	if (!strcmp(m_account,"test") && m_pass == 123456)
	{
		printf("login success");
		res.res = 1;
	}

	
	boost::array<char,1024> buff;
	PacketOStream pos(buff.data(),1024);
	res.Serialize(pos);

	int len = pos.EncodeHeader();


	return 0;
}
