#include "PacketDefine.h"

#define DEFAULT_IMPL_PACKET(class) \
	int class::OnHandle() { return 0;}

// 服务器实现的handle,都是默认的空实现
DEFAULT_IMPL_PACKET(LoginIn);








// 本地实现的handle
int LoginRes::OnHandle()
{

	printf("login in success, o yeah");

	return 0;
}

