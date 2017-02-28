#pragma once
#include "Type_Common.h"
#include "boost/asio.hpp"
#include "boost/enable_shared_from_this.hpp"

/*
* 缓存设计原则
* 分不分库？可能应该分，分为全局库以及玩家库
* key的命名规则，Zone_x:Server_x:TableName:PlayerID
* 关于value的设计，hset是个不错的选择，有域的概念。
*
*/




struct redisContext;

class CDataCacheImpl : public boost::enable_shared_from_this<CDataCacheImpl>
{
public:
	CDataCacheImpl(boost::asio::io_service& _ios);

	enum
	{
		DCI_E_NONE = -3,
		DCI_E_ERR = -2,
		DCI_E_BUSY = -1,
		DCI_E_SUCC = 0,
	};


	/*
	 * @desc 初始化数据缓存
	 * @param dc_ip 数据缓存服务器IP
	 * @param port 数据缓存服务器端口
	 * @return 成功与否
	 *	
	 */
	bool Initialize(const char* dc_ip,int port = 6379);



	/*
	 *	@desc 获取key的value
	 *	@param key 所取的key
	 *	@param data 结果存放所处
	 *	@param nlen 结果长度
	 *	@return 代表结果的状态值
	 */
	int CDataCacheImpl::Get( const char* key, char* data,int& nlen);



	/*
	 * @desc 获取函数回调
	 * @param _key 获取数据对应的key
	 * @return 
	 *	
	 */
	template<typename T,typename F>
	void PostLoad(T* ptr,F func)
	{
		if (!ptr)
		{
			return;
		}

		m_ios.post(boost::bind(func,ptr,shared_from_this()));
	}


	/*
	 *	@desc 设置key的value
	 *	@param key 设置的key
	 *	@param data value存放所处
	 *	@return 代表结果的状态值
	 */
	int CDataCacheImpl::Set(const char* key,const char* data);


private:
	boost::asio::io_service& m_ios;
	redisContext* m_redis_ct;
};

typedef boost::shared_ptr<CDataCacheImpl> dc_ptr;