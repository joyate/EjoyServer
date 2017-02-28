#include "Client.h"
#include "boost/shared_ptr.hpp"
#include "boost/thread.hpp"


int main(int argc,const char* argv[])
{

	Client cli;

	cli.ConnectServer(std::string("127.0.0.1"),std::string("7777"));

	boost::shared_ptr<boost::thread> thread_ptr(new boost::thread(
		boost::bind(&Client::ReadThread,&cli)));

	cli.Run();

	thread_ptr->join();

	return 0;
}