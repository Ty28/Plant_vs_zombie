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
	NONE_TYPE,						  //¿ÕÀàÐÍ
	ZOMBIE_NORMAL,                    //ÆÕÍ¨½©Ê¬
	ZOMBIE_CONEHEAD,                  //Â·ÕÏ½©Ê¬
	ZOMBIE_BUCKETHEAD,                //ÌúÍ°½©Ê¬
	ZOMBIE_SCREENDOOR,				  //ÌúÃÅ½©Ê¬
	ZOMBIE_FLAG,                      //Æì×Ó½©Ê¬
	ZOMBIE_FOOTBALL,				  //éÏé­½©Ê¬
	ZOMBIE_JACKINTHEBOX,			  //Ð¡³ó½©Ê¬
	ZOMBIE_NEWSPAPER,				  //¶Á±¨½©Ê¬
	ZOMBIE_POLEVAULTING,			  //³Å¸Ë½©Ê¬
	ZOMBIE_CATAPULT,				  //Í¶Ê¯½©Ê¬
	ZOMBIE_DANCING,					  //ÎèÍõ½©Ê¬1
	ZOMBIE_BACKUP,					  //°éÎè½©Ê¬1

	PLANT_SUNNER,                     //ÏòÈÕ¿û
	PLANT_PEASHOOTER,                 //Íã¶¹ÉäÊÖ
	PLANT_REPEATER,					  //Ë«·¢ÉäÊÖ
	PLANT_SNOWPEA,					  //º®±ùÉäÊÖ
	PLANT_MELONPULT,				  //Î÷¹ÏÍ¶ÊÖ
	PLANT_SQUASH,					  //ÎÑ¹Ï
	PLANT_WALLNUT,                    //¼á¹ûÇ½
	PLANT_TALLNUT,					  //¸ß¼á¹û
	PLANT_SPIKEROCK,				  //µØ´Ì
	PLANT_POTATOMINE,				  //ÍÁ¶¹µØÀ×
	PLANT_CHERRYBOMB,				  //Ó£ÌÒÕ¨µ¯
	PLANT_JALAPENO,					  //»ð±¬À±½·
	PLANT_GARLIC,					  //´óËâ
	BULLET_ZOMBIE,					  //½©Ê¬ÆÕ¹¥
	BULLET_BASKETBALL,				  //½©Ê¬ÀºÇò¹¥»÷
	BULLET_SUN,                       //Ñô¹â£¬±»ÈÏÎªÊÇ×Óµ¯µÄÒ»ÖÖ
	BULLET_NORMAL,                    //ÆÕÍ¨×Óµ¯
	BULLET_TWO,                       //Ë«ÖØ×Óµ¯
	BULLET_SNOW,					  //º®±ù×Óµ¯
	BULLET_MELON,					  //Î÷¹Ï×Óµ¯
	BULLET_SECKILL					  //ÃëÉ±
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
