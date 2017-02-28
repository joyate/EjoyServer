#pragma once

#include "Type_Common.h"
#include "boost/enable_shared_from_this.hpp"
#include "boost/array.hpp"
#include "boost/asio.hpp"

#define PACKET_WRITE_BUFFER 64*1024

class Connection : public boost::enable_shared_from_this<Connection>
{
public:
	Connection(boost::asio::io_service& ios,int _id);
	~Connection(void);


	// 
	void Start();

	//
	boost::asio::ip::tcp::socket& Socket() { return m_socket;}


	int GetID()	const { return m_ID;}

	void SetPlayerID(int _id) { m_PlayerID = _id;}
	int GetPlayerID() const { return m_PlayerID;}

	// 
	bool TranslateMsg(unsigned short& _msg_id,std::vector<char>& _msg_pak);

	// 处理断开
	void HandleDisconnect();

	// 输出缓冲 
	char* GetSendBuffer()	{ return m_send_buff.data(); }

	void SendPacket(unsigned short uLen);

protected:

	//
	void read_header();

	// 
	void handle_read_header(const boost::system::error_code& e,
		std::size_t bytes_transferred);

	//
	void handle_read_content(const boost::system::error_code& e,
		std::size_t bytes_transferred);

	//
	void handle_send(const boost::system::error_code& e,
		std::size_t bytes_transferred);

private:
	//
	boost::asio::ip::tcp::socket m_socket;
	
	// 玩家ID
	int m_PlayerID;

	// 自己的ID
	int m_ID;

	// buffer
	boost::array<char, 1024> buffer_;


	volatile bool m_bsending;
	boost::array<char,PACKET_WRITE_BUFFER> m_send_buff;
};

typedef boost::shared_ptr<Connection> ConnectionPtr;