#pragma once

#include "boost/asio.hpp"
#include "GamePlayer.h"
#include "Connection.h"
#include "boost/enable_shared_from_this.hpp"

class Dispatcher : public boost::enable_shared_from_this<Dispatcher>
{
public:
	Dispatcher(boost::asio::io_service& _ios);
	~Dispatcher(void);

	

	bool Init();


	int Dispatch(ConnectionPtr pConnPtr);


	// 处理错误
	void OnError(ConnectionPtr pConnPtr);

protected:

	//
	void HandlePacket(ConnectionPtr pConnPtr,unsigned short msgID,std::vector<char> msgPack);

	// 处理出错，一般是关闭连接
	void HandleError(ConnectionPtr pConnPtr);

private:
	boost::asio::io_service& m_ios;

};

