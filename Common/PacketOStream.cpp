#include "PacketOStream.h"
#include <assert.h>

PacketOStream::PacketOStream(char* _buff,int _len):
	m_buff(_buff),
	m_len(_len),
	m_currIndex(0)
{
	assert(m_currIndex + (int)sizeof(short) <= m_len);
	m_currIndex += sizeof(short); // for 
}


PacketOStream::~PacketOStream(void)
{
}
