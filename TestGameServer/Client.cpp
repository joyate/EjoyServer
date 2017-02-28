#include "Client.h"
#include <iostream>
#include <istream>
#include <ostream>


#include "PacketDefine.h"
#include "InstanceMgr.h"

#include "PacketDefine_Handler.h"




Client::Client(void):
	m_socket(m_ios),
	m_bRun(true)
{
}


Client::~Client(void)
{
}

void Client::ConnectServer( std::string& ip,std::string& port )
{
	try
	{
		boost::asio::ip::tcp::endpoint edp(boost::asio::ip::address::from_string(ip),atoi(port.c_str()));
		m_socket.connect(edp);

		boost::asio::ip::tcp::no_delay op(true);
		m_socket.set_option(op);

	}
	catch(boost::system::error_code& ec)
	{
		printf("FATAL:%s",ec.message().c_str());
	}
}

int Client::Run()
{

	LoginIn login;
	strcpy_s(login.m_account,sizeof(login.m_account),"test");
	login.m_pass = 123456;

	
	PacketOStream pos(m_write_buff.data(),PACKET_BUFFER_LENGTH);

	login.Serialize(pos);
	int len = pos.EncodeHeader();
	

	boost::asio::write(m_socket, boost::asio::buffer(m_write_buff,len));

	return 0;
}

int Client::ReadThread()
{
	while (m_bRun)
	{
		
		try
		{

			boost::asio::read(m_socket,boost::asio::buffer(m_read_buff,sizeof(WORD)));

			unsigned short ulen = *(unsigned short*)(m_read_buff.data());

			boost::asio::read(m_socket,boost::asio::buffer(m_read_buff.data()+sizeof(WORD),ulen - sizeof(WORD)));

			unsigned short uMsgID = *(unsigned short*)(m_read_buff.data() + sizeof(WORD));
			IPacket* pPacket = sInstanceMgr.GetInstance(uMsgID);

			PacketIStream pis(m_read_buff.data()+sizeof(WORD),ulen);
			pPacket->UnSerialize(pis);

			Packet_Handler_Proxy* pHandler = sHandlerMgr.GetInstance(pPacket->GetID());
			int nRet = (*pHandler)(pPacket);
			if (nRet)
			{

			}

		}
		catch(std::exception& e)
		{
			printf("%s",e.what());
			m_bRun = false;
			break;
		}
	
	}

	return 0;
}
