#pragma once

#include <vector>

class PacketOStream
{
public:
	PacketOStream(char* _buff,int _len);
	~PacketOStream(void);


	template<typename T>
	PacketOStream& operator << (const T& _var)
	{
		memmove( m_buff + m_currIndex,&_var,sizeof(T));
		m_currIndex += sizeof(T);

		return *this;
	}


	PacketOStream& operator << (const char* buff)
	{
		unsigned short len = strlen(buff);
		memmove(m_buff + m_currIndex,&len,sizeof(unsigned short));
		m_currIndex += sizeof(unsigned short);

		memmove(m_buff + m_currIndex,buff,len);
		m_currIndex += len;

		return *this;
	}


	// 封装下包头
	unsigned short EncodeHeader()
	{
		memmove(m_buff,&m_currIndex,sizeof(short));
		return m_currIndex;
	}


private:
	char* m_buff;
	int m_len;

	int m_currIndex;
};

