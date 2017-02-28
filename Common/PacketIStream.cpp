#include "PacketIStream.h"


PacketIStream::PacketIStream(const char* _buff,int _len):
	m_buff(_buff),
	m_len(_len),
	m_currIndex(0)
{
}

PacketIStream::PacketIStream( std::vector<char>& _buff ):
	m_buff(_buff.data()),
	m_len(_buff.size()),
	m_currIndex(0)
{

}


PacketIStream::~PacketIStream(void)
{
}
