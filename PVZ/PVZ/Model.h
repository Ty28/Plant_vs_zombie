#pragma once
#include "Base.h"
#include <vector>

class Bullet:public Base
{
public:
	Bullet(const Point &point, OBJECT_TYPE bType);
	virtual ~Bullet();
	virtual bool Move(MOVE_TYPE type);
	virtual int BeAttacked(const Base *obj);
	virtual Base *Attack(void);
	virtual int Main(void);
private:
	int m_xBlock;
};

class Plant :public Base
{
public:
	Plant(OBJECT_TYPE objType, OBJECT_TYPE bulletType, const Point & point);
	virtual ~Plant();
	virtual bool Move(MOVE_TYPE type);
	virtual int BeAttacked(const Base *obj);
	virtual Base *Attack(void);
	virtual int Main(void);
};

class Zombie :public Base
{
public:
	Zombie(OBJECT_TYPE objType, OBJECT_TYPE bulletType, const Point &point);
	virtual ~Zombie();
	virtual bool Move(MOVE_TYPE type);
	virtual int BeAttacked(const Base *obj);
	virtual Base *Attack(void);
	virtual int Main(void);
};

class Model
{
public:
	Model();
	virtual ~Model();
	bool NewObject(OBJECT_TYPE objType, const Point &point);
	bool DelPlants(const Point &p);
	void ClearCoolingState();
	int Main(void);
public:
	int sunCount() const { return m_sunCnt; }
	int scoreCount() const { return m_score; }
	const std::vector<Base*> &PlantsObj() const { return m_vPlantsObj; }
	const std::vector<Base*> &ZombieObj() const { return m_vZombieObj; }
	const std::vector<Base*> &BulletObj() const { return m_vBulletObj; }
	const std::vector<PObjMapItem> &StoreMap() const { return m_vStoreMap; }
	const std::vector<std::vector<PObjMapItem> > &ObjMap() const { return m_vObjMap; }
	void incSunCount(int value) { if(value >= 0) m_sunCnt += value; }
	void SetStoreMapItemNegative(unsigned int index);
	void ResetStoreMapItemNegative();
	void SetObjectMapItemNegative(const Point &p);
	void ResetObjectMapItemNegative();
	bool isZombie(const Point &p) const {
		for (unsigned int i = 0; i < m_vZombieObj.size(); ++i) {
			if (m_vZombieObj[i]->position() == p) return true;
		}
		return false;
		/*if (p.X < 0 || p.X > MAXBLOCK_X || p.Y < 0 || p.Y > MAXBLOCK_Y) return false;
		return (m_vObjMap[p.Y][p.X]->objType >= ZOMBIE_NORMAL && m_vObjMap[p.Y][p.X]->objType <= ZOMBIE_BACKUP);*/
	}
	bool isPlant(const Point &p) const {
		for (unsigned int i = 0; i < m_vPlantsObj.size(); ++i) {
			if (m_vPlantsObj[i]->position() == p) return true;
		}
		return false;
		/*if (p.X < 0 || p.X > MAXBLOCK_X || p.Y < 0 || p.Y > MAXBLOCK_Y) return false;
		return (m_vObjMap[p.Y][p.X]->objType >= PLANT_SUNNER && m_vObjMap[p.Y][p.X]->objType <= PLANT_GARLIC);*/
	}
	bool isBullet(const Point &p) const {
		for (unsigned int i = 0; i < m_vBulletObj.size(); ++i)
		{
			if (m_vBulletObj[i]->position() == p) return true;
		}
		return false;
	}
	Base *getZombie(const Point &p) const{
		for (unsigned int i = 0; i < m_vZombieObj.size(); ++i)
		{
			if (m_vZombieObj[i]->position() == p)
				return m_vZombieObj[i];
		}
		return NULL;
	}
	Base *getPlant(const Point &p) const{
		for (unsigned int i = 0; i < m_vPlantsObj.size(); ++i)
		{
			if (m_vPlantsObj[i]->position() == p)
				return m_vPlantsObj[i];
		}
		return NULL;
	}
	Base *getBullet(const Point &p) const {
		for (unsigned int i = 0; i < m_vBulletObj.size(); ++i)
		{
			if (m_vBulletObj[i]->position() == p)
				return m_vBulletObj[i];
		}
		return NULL;
	}
private:
	int m_sunCnt;
	int m_score;
	std::vector<Base*> m_vPlantsObj;
	std::vector<Base*> m_vZombieObj;
	std::vector<Base*> m_vBulletObj;
	std::vector<std::vector<PObjMapItem> > m_vObjMap;
	std::vector<PObjMapItem> m_vStoreMap;
	std::vector<PObjMapItem> m_vZombieMap;
	void InitObjMap();
	void GenerateStoreMap();
	void GenerateZombieMap();
	bool isCooling(OBJECT_TYPE objType) const { return m_vStoreMap[objType - PLANT_SUNNER]->cnt < m_vStoreMap[objType - PLANT_SUNNER]->coolingTime; }
	bool isSunEnough(OBJECT_TYPE objType) const { return m_vStoreMap[objType - PLANT_SUNNER]->cost <= m_sunCnt; }
	bool isMapOccupied(const Point &p) const {
		if (getPlant(p)) return true;
		if (getZombie(p)) return true;
		return false;
	}
	void delPlant(const Base *obj);
	void delZombie(const Base *obj);
	void delBullet(const Base *obj);
	static bool Readin(void);
};

