#pragma once
#include <Windows.h>
#define CYCLETIME ((int)100)

#define MAXBLOCK_X ((int)10)
#define MAXBLOCK_Y ((int)5)
#define BLOCKWIDTH ((int)6)
#define BLOCKHEIGHT ((int)6)
#define MAPSIZE_X (MAXBLOCK_X * (BLOCKWIDTH + 1) + 1)
#define MAPSIZE_Y (MAXBLOCK_Y * (BLOCKHEIGHT + 1) + 1)
#define STOREBLOCK_X ((int)4)
#define STOREBLOCK_Y ((int)4)

#define SPTI(p) ((int)(((p).X) + (((p).Y) * STOREBLOCK_X)))

enum MOVE_TYPE {
	NONE_MOVE,
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_UP, 
	MOVE_DOWN
};

enum SPEED_MODE {
	SPEED_SLOW = 3,
	SPEED_NORMAL = 2,
	SPEED_FAST = 1
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
	ZOMBIE_DANCING,					  //������ʬ1
	ZOMBIE_BACKUP,					  //���轩ʬ1

	PLANT_SUNNER,                     //���տ�
	PLANT_PEASHOOTER,                 //�㶹����
	PLANT_REPEATER,					  //˫������
	PLANT_SNOWPEA,					  //��������
	PLANT_MELONPULT,				  //����Ͷ��
	PLANT_SQUASH,					  //�ѹ�
	PLANT_WALLNUT,                    //���ǽ
	PLANT_TALLNUT,					  //�߼��
	PLANT_SPIKEROCK,				  //�ش�
	PLANT_POTATOMINE,				  //��������
	PLANT_CHERRYBOMB,				  //ӣ��ը��
	PLANT_JALAPENO,					  //������
	PLANT_GARLIC,					  //����
	BULLET_ZOMBIE,					  //��ʬ�չ�
	BULLET_BASKETBALL,				  //��ʬ���򹥻�
	BULLET_SUN,                       //���⣬����Ϊ���ӵ���һ��
	BULLET_NORMAL,                    //��ͨ�ӵ�
	BULLET_TWO,                       //˫���ӵ�
	BULLET_SNOW,					  //�����ӵ�
	BULLET_MELON,					  //�����ӵ�
	BULLET_SECKILL					  //��ɱ
};

typedef struct stObjectInfo {
	OBJECT_TYPE type;
	int hp;
	int speed;
	int attack;
	int coolingTime;
	int cost;
	int score;
	stObjectInfo(OBJECT_TYPE _type, int _hp, int _speed, int _attack, int _coolingTime, int _cost, int _score) :
		type(_type), hp(_hp), speed(_speed), attack(_attack), coolingTime(_coolingTime), cost(_cost), score(_score) {}
}ObjInfo, *PObjInfo;

typedef struct stObjMap {
	OBJECT_TYPE objType;
	LPCWSTR name;
	int coolingTime;
	int cost;
	int cnt;
	bool negative;
	bool repaint;
	stObjMap():objType(NONE_TYPE),cnt(0) {}
	stObjMap(OBJECT_TYPE _objType, bool _negative) :objType(_objType), cnt(0), negative(_negative) {}
	stObjMap(OBJECT_TYPE _objType, LPCWSTR _name, int _coolingTime, int _cost, bool _negative, bool _repaint) :
		objType(_objType), name(_name), coolingTime(_coolingTime), cost(_cost), cnt(_coolingTime), negative(_negative), repaint(_repaint) {}
}ObjMapItem, *PObjMapItem;

class Point
{
public:
	Point():X(0), Y(0){}
	Point(int x, int y) :X(x), Y(y) {}
	Point(const Point &p) :X(p.X), Y(p.Y) {}
	Point(const COORD &cr) :X(cr.X), Y(cr.Y) {}
	friend const Point operator+(const Point &p1, const Point &p2) {
		return Point(p1.X + p2.X, p1.Y + p2.Y);
	}
	friend const Point operator-(const Point &p1, const Point &p2) {
		return Point(p1.X - p2.X, p1.Y - p2.Y);
	}
	friend const Point operator*(const Point &p1, const Point &p2) {
		return Point(p1.X * p2.X, p1.Y * p2.Y);
	}
	friend bool operator==(const Point &p1, const Point &p2) {
		return (p1.X == p2.X && p1.Y == p2.Y);
	}
	Point &operator=(const Point &p) {
		this->X = p.X;
		this->Y = p.Y;
		return *this;
	}
	Point &operator+=(const Point &p) {
		this->X += p.X;
		this->Y += p.Y;
		return *this;
	}
	Point &operator-=(const Point &p) {
		this->X -= p.X;
		this->Y -= p.Y;
		return *this;
	}
	Point &operator*=(const Point &p) {
		this->X *= p.X;
		this->Y *= p.Y;
		return *this;
	}

	int X;
	int Y;
};

class Base
{
public:
	Base(LPCWSTR name, const Point &point, OBJECT_TYPE otype, OBJECT_TYPE btype, int frames, int cnt);
	virtual ~Base() {}
	virtual bool isDead(void) { return m_hp <= 0; }
	virtual bool Move(MOVE_TYPE type) = 0;
	virtual int BeAttacked(const Base *obj) = 0;
	virtual Base *Attack(void) = 0;
	virtual int Main(void) = 0;
public:
	const Point &position(void) const { return m_point; }
	OBJECT_TYPE objectType(void) const { return m_objectType; }
	OBJECT_TYPE bulletType(void) const { return m_bulletType; }
	int hp(void) const { return m_hp; }
	int speed(void) const { return m_speed; }
	int attack(void) const { return m_attack; }
	int coolingTime(void) const { return m_coolingTime; }
	int cost(void) const { return m_cost; }
	int scroe(void) const { return m_score; }
	int frames(void) const { return m_frames; }
	int cnt(void) const { return m_cnt; }
	bool isNegative(void) const { return m_negative; }

	void setHp(int value) { m_hp = value; }
	void setSpeed(int value) { m_speed = value; }
	void setAttack(int value) { m_attack = value; }
	void setCoolingTime(int value) { m_coolingTime = value; }
	void setCost(int value) { m_cost = value; }
	void setScroe(int value) { m_score = value; }
	void setFrames(int value) { m_frames = value; }
	void setCnt(int value) { m_cnt = value; }
	void setNegative(int value) { m_negative = value; }
	
protected:
	LPCWSTR m_name;
	Point m_point;
	OBJECT_TYPE m_objectType;
	OBJECT_TYPE m_bulletType;
	int m_hp;
	int m_speed;
	int m_attack;
	int m_coolingTime;
	int m_cost;
	int m_score;
	int m_frames;
	int m_cnt;
	bool m_negative;
public:
	static const PObjInfo GetInfo(OBJECT_TYPE type);
};
