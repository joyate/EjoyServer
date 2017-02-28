#pragma once

#include <string>
#include <vector>
#include <assert.h>

class PacketIStream
{
public:
	PacketIStream(const char* _buff,int _len);
	PacketIStream(std::vector<char>& _buff);


	~PacketIStream(void);



	template<typename T>
	PacketIStream& operator >> (T& _var)
	{
		assert(m_currIndex+(short)sizeof(T) <= m_len);
		memmove(&_var,m_buff+m_currIndex,sizeof(T));
		m_currIndex += sizeof(T);

		return *this;
	}



	PacketIStream& operator >> (char* _array)
	{
		unsigned short len = *(unsigned short*)(m_buff+m_currIndex);
		assert(m_currIndex + len <= m_len);
		m_currIndex += sizeof(unsigned short);


		memmove(_array,m_buff + m_currIndex, len);
		m_currIndex += len;

		return *this;
	}
	


private:
	int m_currIndex;

	const char* m_buff;
	int m_len;
};

