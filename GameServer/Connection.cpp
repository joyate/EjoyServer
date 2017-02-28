#include "Connection.h"
#include "boost/bind.hpp"
#include "Server.h"
#include <istream>


extern Server svr;

Connection::Connection(boost::asio::io_service& ios,int _id):
	m_socket(ios),
	m_ID(_id),
	m_bsending(false)
{

}


Connection::~Connection(void)
{
}

void Connection::Start()
{
	read_header();
}


void Connection::read_header()
{
	boost::asio::async_read(m_socket,boost::asio::buffer(buffer_,sizeof(WORD)),
		boost::bind(&Connection::handle_read_header,shared_from_this(),
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred));
}


void Connection::handle_read_header( const boost::system::error_code& e, std::size_t bytes_transferred )
{
	if (!e)
	{
		assert(bytes_transferred == sizeof(WORD));
		int len = *(short*)(buffer_.data());

		boost::asio::async_read(m_socket,boost::asio::buffer(buffer_.data()+sizeof(WORD),len - sizeof(WORD)),
			boost::bind(&Connection::handle_read_content,shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	
	
	}else
	{
		svr.m_Dispatcher->OnError(shared_from_this());	
	}

}


//
void Connection::handle_read_content( const boost::system::error_code& e, std::size_t bytes_transferred )
{
	if (!e)
	{
		svr.m_Dispatcher->Dispatch(shared_from_this());

	}else
	{
		svr.m_Dispatcher->OnError(shared_from_this());	
	}

	// 下一个消息包
	read_header();
}


// 解包格式，长度剥离出来，ID暂时留着
bool Connection::TranslateMsg( unsigned short& _msg_id,std::vector<char>& _msg_pak )
{
	int cur_index = 0;

	WORD nMsgLen = *(WORD*)(buffer_.data() + cur_index);
	cur_index += sizeof(WORD);
	_msg_id = *(WORD*)(buffer_.data() + cur_index);
	
	_msg_pak.resize(nMsgLen - sizeof(WORD));
	memmove(&_msg_pak[0],buffer_.data()+cur_index,nMsgLen-sizeof(WORD));

	return true;
}


void Connection::HandleDisconnect()
{
	m_ID = -1;
	m_PlayerID = -1;

	boost::system::error_code ignored_ec;
	m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
}

void Connection::SendPacket(unsigned short uLen)
{
	if (uLen <= 0)
	{
		return;
	}

	assert(!m_bsending && "send not ready");
	m_bsending = true;
	boost::asio::async_write(m_socket,
		boost::asio::buffer(m_send_buff,uLen),
		boost::bind(&Connection::handle_send,shared_from_this(),
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred));
}

void Connection::handle_send( const boost::system::error_code& e, std::size_t bytes_transferred )
{

	m_bsending = false;

	if (!e)
	{

	
	}else
	{
		svr.m_Dispatcher->OnError(shared_from_this());	
	}

}


