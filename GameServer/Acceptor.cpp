#include "Acceptor.h"
#include "Server.h"

extern Server svr;

Acceptor::Acceptor(boost::asio::io_service& _ios):
	m_acceptor(_ios)
{

}


Acceptor::~Acceptor(void)
{
}

bool Acceptor::Init(std::string& addr, std::string& nPort )
{
	boost::asio::ip::tcp::resolver resolver(m_acceptor.get_io_service());
	boost::asio::ip::tcp::resolver::query query(addr, nPort);
	boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
	m_acceptor.open(endpoint.protocol());
	m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	m_acceptor.bind(endpoint);
	m_acceptor.listen();


	StartAccept();

	return true;
}

void Acceptor::StartAccept()
{
	static int CP_ID = 1;

	m_new_connection_ptr.reset(new Connection(*svr.GetNextIOS(),CP_ID++));
	m_acceptor.async_accept(m_new_connection_ptr->Socket(),
		boost::bind(&Acceptor::handle_accept,this,
		boost::asio::placeholders::error));
}

void Acceptor::handle_accept( const boost::system::error_code& e )
{

	static int GP_ID = 1;

	if (!e)
	{
		GamePlayerPtr gp(new GamePlayer(GP_ID++,m_new_connection_ptr->GetID()));
		svr.m_mapGamePlayerPtr.insert(std::make_pair(GP_ID,gp));
		m_new_connection_ptr->SetPlayerID(GP_ID);
		m_new_connection_ptr->Start();

	}

	StartAccept();
}
