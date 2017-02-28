#include "type_common.h"
#include "Server.h"

Server svr;

int main(int argc,const char* argv[])
{
	if (!svr.Init())
	{
		return 0;
	}

	svr.Run();

		
	return 0;
}