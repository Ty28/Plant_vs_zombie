#pragma once


enum MOVE_TYPE {
	NONE_MOVE,
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_DOWN
};

enum STATE_TYPE {
	STATE_NONE,                      //�������κ���ȷ��״̬
	STATE_MOVE,                      //�ƶ�״̬
	STATE_STAND,                     //վ��״̬
	STATE_DIE,                       //����װ��
	STATE_ATTACK,                    //����״̬
	STATE_BEATTACK                   //������״̬
};

enum OBJECT_TYPE {
	NONE_TYPE,						  //������
	ZOMBIE_NORMAL,                    //��ͨ��ʬ
	ZOMBIE_CONEHEAD,                  //·�Ͻ�ʬ
	ZOMBIE_BUCKETHEAD,                //��Ͱ��ʬ
	ZOMBIE_SCREENDOOR,				  //���Ž�ʬ
	ZOMBIE_FLAG,                      //���ӽ�ʬ
	ZOMBIE_FOOTBALL,				  //��魽�ʬ
	ZOMBIE_JACKINTHEBOX,			  //С��ʬ
	ZOMBIE_NEWSPAPER,				  //������ʬ
	ZOMBIE_POLEVAULTING,			  //�Ÿ˽�ʬ
	ZOMBIE_CATAPULT,				  //Ͷʯ��ʬ
	ZOMBIE_DANCING,					  //������ʬ
	ZOMBIE_BACKUP,					  //���轩ʬ

	PLANT_PEASHOOTER,                 //�㶹����
	PLANT_REPEATER,					  //˫������
	PLANT_MELONPULT,				  //����Ͷ��
	PLANT_SPIKEROCK,				  //�ش�
	PLANT_WALLNUT,                    //���ǽ
	PLANT_TALLNUT,					  //�߼��
	PLANT_SQUASH,					  //�ѹ�
	PLANT_POTATOMINE,				  //��������
	PLANT_CHERRYBOMB,				  //ӣ��ը��
	PLANT_JALAPENO,					  //������
	PLANT_SUNNER,                     //���տ�
	PLANT_GARLIC,					  //����

	BULLET_ZOMBIE,					  //��ʬ�չ�
	BULLET_BASKETBALL,				  //��ʬ���򹥻�
	BULLET_SUN,                       //���⣬����Ϊ���ӵ���һ��
	BULLET_NORMAL,                    //��ͨ�ӵ�
	BULLET_TWO,                       //˫���ӵ�
	BULLLET_MELON,					  //�����ӵ�
	BULLET_SECKILL					  //��ɱ
};






