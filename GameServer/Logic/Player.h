#pragma once
#include "GameStruct/GS_Player.h"
#include "../DataCache/DataCache.h"




class Player
{
public:

	Player(int _id);
	~Player();

	int Load(dc_ptr _cache);



	
	// 从缓存载入
	int _LoadFromDC(dc_ptr _cache);



	/*
	 * @desc 当缓存被miss后，从数据库中载入数据
	 * @param _cache 缓存实现
	 * @return 返回结果状态
	 * @mark 该函数由程序触发，不要主动调用。而且这个函数执行时所在的线程
	 *	也和本类的其他函数是不在一个线程的。
	 */
	int _LoadFromDB(dc_ptr _cache);



private:
	int m_nID;
	tPlayerData m_Data;
};
