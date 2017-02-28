#pragma once

#include "boost/asio.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/thread.hpp"
#include <vector>

typedef boost::shared_ptr<boost::asio::io_service> ios_ptr;


struct ios_t
{
	bool m_bexcl;			//  «∑Ò∂¿’º
	ios_ptr m_ios_ptr;		// io_service share_ptr

	ios_t()
	{
		m_bexcl = false;
		m_ios_ptr.reset();
	}

	~ios_t()
	{
		m_bexcl = false;
		m_ios_ptr.reset();
	}
};

typedef boost::shared_ptr<ios_t> ele_ios_t;
template<int IOS_CNT = GameDefine::GAME_IO_SERVICE_MAX>
class Engine
{
public:
	Engine(void):m_CurrentIndex(0){}
	~Engine(void){}

	typedef boost::shared_ptr<boost::asio::io_service::work> work_ptr;

	//
	virtual bool Init()
	{
		for (int i= 0; i < IOS_CNT; i++)
		{
			ele_ios_t io_s(new ios_t);
			io_s->m_ios_ptr.reset(new boost::asio::io_service);	
			m_vecIOS.push_back(io_s);
		}
		
		return true;
	}


	// 
	ios_ptr GetIOService(int i)
	{
		int idx = i%IOS_CNT;
		return m_vecIOS[idx]->m_ios_ptr;
	}

	ios_ptr GetNextIOS(bool bExcl = false)
	{
		ios_ptr ret_ios;
		int trycnt = IOS_CNT;
		do 
		{
			ele_ios_t io_s = m_vecIOS[m_CurrentIndex++%IOS_CNT];
			if (io_s->m_bexcl)
			{
				trycnt--;
				continue;
			}

			if (bExcl)
			{
				io_s->m_bexcl = true;
			}

			ret_ios = io_s->m_ios_ptr;

		} while (!ret_ios && trycnt > 0);

		return ret_ios;
	}


	//
	virtual void Run()
	{
		std::vector<boost::shared_ptr<boost::thread> > threads;
		for (std::size_t i = 0; i < IOS_CNT; ++i)
		{
			work_ptr ios_work(new boost::asio::io_service::work(*GetIOService(i)));
			m_work.push_back(ios_work);

			boost::shared_ptr<boost::thread> thread(new boost::thread(
				boost::bind(&boost::asio::io_service::run, GetIOService(i))));
			threads.push_back(thread);
		}

		for (std::size_t i = 0; i < threads.size(); ++i)
			threads[i]->join();
	}



	void Stop()
	{
		std::for_each(m_vecIOS.begin(),m_vecIOS.end(),boost::bind(&Engine::Stop,this,_1));
	}

protected:

	void Stop(ele_ios_t _ios)
	{
		if (_ios)
		{
			_ios->m_ios_ptr->stop();
		}
	}

private:

	int m_CurrentIndex;
	std::vector<ele_ios_t> m_vecIOS;
	std::vector<work_ptr> m_work;
};

