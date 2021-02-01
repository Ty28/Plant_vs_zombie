#pragma once


enum MOVE_TYPE {
	NONE_MOVE,
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_DOWN
};

enum STATE_TYPE {
	STATE_NONE,                      //不处于任何明确的状态
	STATE_MOVE,                      //移动状态
	STATE_STAND,                     //站立状态
	STATE_DIE,                       //死亡装填
	STATE_ATTACK,                    //攻击状态
	STATE_BEATTACK                   //被攻击状态
};

enum OBJECT_TYPE {
	NONE_TYPE,						  //空类型
	ZOMBIE_NORMAL,                    //普通僵尸
	ZOMBIE_CONEHEAD,                  //路障僵尸
	ZOMBIE_BUCKETHEAD,                //铁桶僵尸
	ZOMBIE_SCREENDOOR,				  //铁门僵尸
	ZOMBIE_FLAG,                      //旗子僵尸
	ZOMBIE_FOOTBALL,				  //橄榄僵尸
	ZOMBIE_JACKINTHEBOX,			  //小丑僵尸
	ZOMBIE_NEWSPAPER,				  //读报僵尸
	ZOMBIE_POLEVAULTING,			  //撑杆僵尸
	ZOMBIE_CATAPULT,				  //投石僵尸
	ZOMBIE_DANCING,					  //舞王僵尸
	ZOMBIE_BACKUP,					  //伴舞僵尸

	PLANT_PEASHOOTER,                 //豌豆射手
	PLANT_REPEATER,					  //双发射手
	PLANT_MELONPULT,				  //西瓜投手
	PLANT_SPIKEROCK,				  //地刺
	PLANT_WALLNUT,                    //坚果墙
	PLANT_TALLNUT,					  //高坚果
	PLANT_SQUASH,					  //窝瓜
	PLANT_POTATOMINE,				  //土豆地雷
	PLANT_CHERRYBOMB,				  //樱桃炸弹
	PLANT_JALAPENO,					  //火爆辣椒
	PLANT_SUNNER,                     //向日葵
	PLANT_GARLIC,					  //大蒜

	BULLET_ZOMBIE,					  //僵尸普攻
	BULLET_BASKETBALL,				  //僵尸篮球攻击
	BULLET_SUN,                       //阳光，被认为是子弹的一种
	BULLET_NORMAL,                    //普通子弹
	BULLET_TWO,                       //双重子弹
	BULLLET_MELON,					  //西瓜子弹
	BULLET_SECKILL					  //秒杀
};






