#pragma once

#include "boost/asio.hpp"
#include "boost/array.hpp"

#define PACKET_BUFFER_LENGTH 1024

class Client
{
public:
	Client(void);
	~Client(void);


	void ConnectServer(std::string& ip,std::string& port);


	int Run();

	int ReadThread();

private:
	
	bool m_bRun;

	boost::asio::io_service m_ios;
	boost::asio::ip::tcp::socket m_socket;


	boost::array<char,PACKET_BUFFER_LENGTH> m_write_buff;
	boost::array<char,PACKET_BUFFER_LENGTH> m_read_buff;
};

