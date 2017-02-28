#include "GamePlayer.h"


GamePlayer::GamePlayer(/*boost::asio::io_service& ios,*/int _id,int _conn_id):
	//m_ios(ios),
	m_ID(_id),
	m_ConnID(_conn_id)
{


}


GamePlayer::~GamePlayer(void)
{

}


int GamePlayer::OnExit()
{
	return 0;
}
