#define _CRT_SECURE_NO_WARNINGS
#include "Model.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

LPCWSTR names[] = {
	L"",
	L"∆’Õ®Ω© ¨",
	L"¬∑’œΩ© ¨",
	L"Ã˙Õ∞Ω© ¨",
	L"Ã˙√≈Ω© ¨",
	L"∆Ï◊”Ω© ¨",
	L"ÈœÈ≠Ω© ¨",
	L"–°≥ÛΩ© ¨",
	L"∂¡±®Ω© ¨",
	L"≥≈∏ÀΩ© ¨",
	L"Õ∂ ØΩ© ¨",
	L"ŒËÕıΩ© ¨",
	L"∞ÈŒËΩ© ¨",
	L"œÚ»’ø˚",
	L"Õ„∂π…‰ ÷",
	L"À´∑¢…‰ ÷",
	L"∫Æ±˘…‰ ÷",
	L"Œ˜πœÕ∂ ÷",
	L"Œ—πœ",
	L"º·π˚«Ω",
	L"∏ﬂº·π˚",
	L"µÿ¥Ã",
	L"Õ¡∂πµÿ¿◊",
	L"”£Ã“’®µØ",
	L"ª±¨¿±Ω∑",
	L"¥ÛÀ‚",
	L"",
	L"°Ò",
	L"",
	L"°Ò",
	L"°Ò",
	L"°Ò",
	L"°Ô",
	L""
};
std::vector<PObjInfo> g_vObjInfo;

Bullet::Bullet(const Point &point, OBJECT_TYPE bType):
Base(names[bType], point, bType, bType, 0, BLOCKWIDTH)
{
	m_frames = m_speed;
}

Bullet::~Bullet()
{
}

bool Bullet::Move(MOVE_TYPE type)
{
	switch (type)
	{
	case MOVE_LEFT:
		m_point.X--;
		break;
	case MOVE_RIGHT:
		m_point.X++;
		break;
	case MOVE_UP:
		m_point.Y--;
		break;
	case MOVE_DOWN:
		m_point.Y++;
		break;
	}
	if (m_objectType == BULLET_BASKETBALL) m_cnt = m_frames;
	else m_cnt = 0;
	return true;
}

int Bullet::BeAttacked(const Base *obj)
{
	return 0;
}

Base *Bullet::Attack(void)
{
	return NULL;;
}

int Bullet::Main(void)
{
	static int count = 0;
	count++;
	if (m_objectType == BULLET_BASKETBALL)
	{
		if (count >= 2) m_cnt--;

		if (m_cnt <= 0)
		{
			m_cnt = 0;
			return 1;
		}
	}
	else
	{
		if (count >= 2) m_cnt++;

		if (m_cnt >= m_frames)
		{
			m_cnt = m_frames;
			return 1;
		}
	}
	
	
	return 0;
}

Plant::Plant(OBJECT_TYPE objType, OBJECT_TYPE bulletType, const Point & point) :
Base(names[objType], point, objType, bulletType, 0, 0)
{
	m_frames = m_speed * 1000 / CYCLETIME;
}

Plant::~Plant()
{
}

bool Plant::Move(MOVE_TYPE type)
{
	return false;
}

int Plant::BeAttacked(const Base *obj)
{
	if (obj->objectType() == PLANT_SQUASH) return 0;
	if (obj->objectType() == PLANT_CHERRYBOMB) return 0;
	if (obj->objectType() == PLANT_JALAPENO) return 0;
	m_hp -= obj->attack();
	return obj->attack();
}

Base *Plant::Attack(void)
{
	Bullet *bullet = new Bullet(m_point, m_bulletType);
	return bullet;
}

int Plant::Main(void)
{
	m_cnt++;
	if (m_cnt >= m_frames)
	{
		m_cnt = 0;
		return 1;
	}
	return 0;
}


Zombie::Zombie(OBJECT_TYPE objType, OBJECT_TYPE bulletType, const Point & point):
Base(names[objType], point, objType, bulletType, 0, 0)
{
	if (m_objectType == ZOMBIE_FLAG || m_objectType == ZOMBIE_FOOTBALL)
	{
		m_coolingTime = SPEED_FAST;
	}
	else m_coolingTime = SPEED_NORMAL;
	m_frames = m_coolingTime * m_speed * 1000 / CYCLETIME;
}

Zombie::~Zombie()
{
}

bool Zombie::Move(MOVE_TYPE type)
{
	static int cnt = 0;
	if (++cnt <= 4)
	{
		m_cnt = 0;
		return true;
	}
	switch (type)
	{
	case MOVE_LEFT:
		m_point.X--;
		break;
	case MOVE_RIGHT:
		m_point.X++;
		break;
	case MOVE_UP:
		m_point.Y--;
		break;
	case MOVE_DOWN:
		m_point.Y++;
		break;
	}
	m_cnt = 0;
	cnt = 0;
	return true;
}

int Zombie::BeAttacked(const Base *obj)
{
	static int i = 0;
	i++;
	if (m_objectType == ZOMBIE_NEWSPAPER && i >= 5)
	{
		if (m_coolingTime != SPEED_FAST)
		{
			m_coolingTime--;
			m_frames = m_coolingTime * m_speed * 1000 / CYCLETIME;
		}
		m_negative = true;
	}
	m_hp -= obj->attack();
	if (obj->bulletType() == BULLET_SNOW)
	{
		if (m_coolingTime != SPEED_SLOW)
		{
			m_coolingTime++;
			m_frames = m_coolingTime * m_speed * 1000 / CYCLETIME;
		}
	}
	return obj->attack();
}

Base *Zombie::Attack(void)
{
	static int attCnt = 0;
	if (attCnt < 6 && m_bulletType == BULLET_BASKETBALL)
	{
		attCnt++;
		Bullet *bullet = new Bullet(m_point, m_bulletType);
		bullet->setCnt(0);
		return bullet;
	}
	else if (m_objectType == ZOMBIE_JACKINTHEBOX)
	{
		double rate = 0.0;
		switch (m_point.X)
		{
		case 8:case 7:
			rate = 0.05;
			break;
		case 9:case 6:case 5:
			rate = 0.0;
			break;
		case 4:
			rate = 0.2;
			break;
		case 3:
			rate = 0.25;
			break;
		case 2:
			rate = 0.33;
			break;
		case 1:
			rate = 0.5;
			break;
		case 0:
			rate = 1.0;
			break;
		default:
			rate = 0.0;
			break;
		}
		if (rand() < RAND_MAX * rate / 3) return (Base*)1;
	}
	return NULL;
}

int Zombie::Main(void)
{
	m_cnt++;
	if (m_cnt >= m_frames)
	{
		return 1;
	}
	return 0;
}

Model::Model():
m_sunCnt(50),
m_score(0),
m_vObjMap(MAXBLOCK_Y, std::vector<PObjMapItem>(MAXBLOCK_X))
{
	srand((unsigned int)time(NULL));
	static bool state = false;
	if (!state)
	{
		if (Readin())
		{
			state = true;
		}
		else
		{
			std::cout << "Read in failed." << std::endl;
			system("pause");
			exit(-2);
		}
	}
	InitObjMap();
	GenerateStoreMap();
	GenerateZombieMap();
}

Model::~Model()
{
	Base *pObj = NULL;
	while (!m_vPlantsObj.empty())
	{
		pObj = m_vPlantsObj[m_vPlantsObj.size() - 1];
		delete pObj;
		m_vPlantsObj.pop_back();
	}
	while (!m_vZombieObj.empty())
	{
		pObj = m_vZombieObj[m_vZombieObj.size() - 1];
		delete pObj;
		m_vZombieObj.pop_back();
	}
	
	while (!m_vBulletObj.empty())
	{
		pObj = m_vBulletObj[m_vBulletObj.size() - 1];
		delete pObj;
		m_vBulletObj.pop_back();
	}

	PObjMapItem pMapItem;

	for (unsigned int j = 0; j < MAXBLOCK_X - 1; ++j)
	{
		for (unsigned int i = 0; i < MAXBLOCK_Y; ++i)
		{
			delete m_vObjMap[i][j];
			m_vObjMap[i][j] = NULL;
		}
	}

	while (!m_vStoreMap.empty())
	{
		pMapItem = m_vStoreMap[m_vStoreMap.size() - 1];
		delete pMapItem;
		m_vStoreMap.pop_back();
	}
}

bool Model::NewObject(OBJECT_TYPE objType, const Point &point)
{
	Base *pObj = NULL;
	if (objType >= BULLET_ZOMBIE)
	{

	}
	else if (objType >= PLANT_SUNNER)
	{
		if (isCooling(objType)) return false;
		if (!isSunEnough(objType)) return false;
		if (isMapOccupied(point)) return false;
		if (objType == PLANT_SPIKEROCK || objType == PLANT_POTATOMINE) return false;
		OBJECT_TYPE bulletType;
		switch (objType)
		{
		case PLANT_PEASHOOTER:
			bulletType = BULLET_NORMAL;
			break;
		case PLANT_REPEATER:
			bulletType = BULLET_TWO;
			break;
		case PLANT_SNOWPEA:
			bulletType = BULLET_SNOW;
			break;
		case PLANT_MELONPULT:
			bulletType = BULLET_MELON;
			break;
		default:
			bulletType = NONE_TYPE;
			break;
		}
		pObj = new Plant(objType, bulletType, point);
		m_vPlantsObj.push_back(pObj);
		m_vObjMap[point.Y][point.X]->objType = objType;
		m_vStoreMap[objType - PLANT_SUNNER]->cnt = 0;
		m_sunCnt -= m_vStoreMap[objType - PLANT_SUNNER]->cost;
		m_vObjMap[point.Y][point.X]->repaint = true;
	}
	else
	{
		OBJECT_TYPE bulletType;
		switch (objType)
		{
		case ZOMBIE_CATAPULT:
			bulletType = BULLET_BASKETBALL;
			break;
		default:
			bulletType = NONE_TYPE;
			break;
		}
		pObj = new Zombie(objType, bulletType, point);
		m_vZombieObj.push_back(pObj);
		m_vObjMap[point.Y][point.X]->objType = objType;
		m_vObjMap[point.Y][point.X]->repaint = true;
	}
	
	return true;
}

bool Model::DelPlants(const Point & p)
{
	Base *obj;
	unsigned int i;
	for (i = 0; i < m_vPlantsObj.size(); ++i)
	{
		if (m_vPlantsObj[i]->position() == p) break;
	}
	if (i == m_vPlantsObj.size()) return false;
	obj = m_vPlantsObj[i];
	for (; i < m_vPlantsObj.size() - 1; ++i)
	{
		m_vPlantsObj[i] = m_vPlantsObj[i + 1];
	}
	m_vPlantsObj.pop_back();
	delete obj;
	m_vObjMap[p.Y][p.X]->objType = NONE_TYPE;
	return true;
}

void Model::ClearCoolingState()
{
	for (unsigned int i = 0; i < m_vStoreMap.size(); ++i)
	{
		m_vStoreMap[i]->cnt = m_vStoreMap[i]->coolingTime;
	}
}

int Model::Main(void)
{
	static int cnt = 0, cnt1 = 0;
	static bool start = false;
	int res = 0;
	Base *obj;
	for (unsigned int i = 0; i < m_vPlantsObj.size(); ++i)
	{
		obj = m_vPlantsObj[i];
		switch (obj->objectType())
		{
		case PLANT_SUNNER:
			if (obj->Main())
			{
				m_sunCnt += obj->attack();
			}
			break;
		case PLANT_PEASHOOTER:case PLANT_SNOWPEA:case PLANT_MELONPULT:
			if (obj->Main())
			{
				int j = 0;
				Bullet *bullet = dynamic_cast<Bullet*>(obj->Attack());
				if (!bullet) exit(-4);
				for (j = obj->position().X; j < MAXBLOCK_X; ++j)
				{
					if (isZombie(Point(j, obj->position().Y)))
					{
						m_vBulletObj.push_back(bullet);
						break;
					}
				}
				if (j == MAXBLOCK_X)
				{
					delete bullet;
				}
			}
			break;
		case PLANT_REPEATER:
			if (obj->Main())
			{
				int j = 0;
				Bullet *bullet1 = dynamic_cast<Bullet*>(obj->Attack());
				Bullet *bullet2 = dynamic_cast<Bullet*>(obj->Attack());
				if (!bullet1 || !bullet2) exit(-4);
				bullet2->setCnt(0);
				for (int j = obj->position().X; j < MAXBLOCK_X; ++j)
				{
					if (isZombie(Point(j, obj->position().Y)))
					{
						m_vBulletObj.push_back(bullet1);
						m_vBulletObj.push_back(bullet2);
						break;
					}
				}
				if (j == MAXBLOCK_X)
				{
					delete bullet1;
					delete bullet2;
				}
			}
			break;
		case PLANT_SQUASH:
			if (obj->Main())
			{
				Point pos = obj->position();
				if (isZombie(pos + Point(1, 0)))
				{
					Base *zombie;
					while (zombie = getZombie(pos + Point(1, 0)))
					{
						m_score += zombie->scroe();
						delZombie(zombie);
					}
					delPlant(obj);
				}
			}
			break;
		case PLANT_CHERRYBOMB:
			if (obj->Main())
			{
				Point pos = obj->position() - Point(1, 1);
				Base *zombie;
				for (int y = 0; y < 3; y++)
				{
					for (int x = 0; x < 3; ++x)
					{
						if (x == y && x == 1) continue;
						if (isZombie(pos + Point(x, y)))
						{

							while (zombie = getZombie(pos + Point(x, y)))
							{
								m_score += zombie->scroe();
								delZombie(zombie);
							}
						}
					}
				}

				delPlant(obj);
			}
			break;
		case PLANT_JALAPENO:
			if (obj->Main())
			{
				int y = obj->position().Y;
				Base *zombie;
				for (int x = 0; x < MAXBLOCK_X; x++)
				{
					if (isZombie(Point(x, y)))
					{
						while (zombie = getZombie(Point(x, y)))
						{
							m_score += zombie->scroe();
							delZombie(zombie);
						}
					}
				}
				delPlant(obj);
			}
			break;
		default:
			break;
		}
	}
	for (unsigned int i = 0; i < m_vZombieObj.size(); ++i)
	{
		obj = m_vZombieObj[i];
		switch (obj->objectType())
		{
		case ZOMBIE_NORMAL:
		case ZOMBIE_CONEHEAD:
		case ZOMBIE_BUCKETHEAD:
		case ZOMBIE_SCREENDOOR:
		case ZOMBIE_FLAG:
		case ZOMBIE_FOOTBALL:
		case ZOMBIE_NEWSPAPER:
		case ZOMBIE_CATAPULT:
			if (obj->Main())
			{
				if (obj->position().X <= 0) return 1;
				int j = 0;
				Bullet *bullet = dynamic_cast<Bullet*>(obj->Attack());
				if (bullet)
				{
					for (j = obj->position().X; j >= 0; --j)
					{
						if (isPlant(Point(j, obj->position().Y)))
						{
							m_vBulletObj.push_back(bullet);
							obj->setCnt(0);
							break;
						}
					}
					if (j == -1)
					{
						delete bullet;
					}
				}
				else
				{
					Point nextPoint = obj->position() - Point(1, 0);
					if (isPlant(nextPoint))
					{
						Base *plant = getPlant(nextPoint);
						plant->BeAttacked(obj);
						if (plant->objectType() == PLANT_GARLIC && plant->isNegative() == false)
						{
							if (obj->position().Y != 0 && !isPlant(obj->position() + Point(0, -1)))
							{
								m_vObjMap[obj->position().Y][obj->position().X]->repaint = true;
								obj->Move(MOVE_UP);
								m_vObjMap[obj->position().Y][obj->position().X]->objType = obj->objectType();
								m_vObjMap[obj->position().Y][obj->position().X]->repaint = true;
							}
							else if (obj->position().Y != MAXBLOCK_Y && !isPlant(obj->position() + Point(0, 1)))
							{
								m_vObjMap[obj->position().Y][obj->position().X]->repaint = true;
								obj->Move(MOVE_DOWN);
								m_vObjMap[obj->position().Y][obj->position().X]->objType = obj->objectType();
								m_vObjMap[obj->position().Y][obj->position().X]->repaint = true;
							}
							plant->setNegative(true);
						}
						if (plant->isDead())
						{
							delPlant(plant);
						}
					}
					else
					{
						if (isBullet(nextPoint))
						{
							Base *bullet = getBullet(nextPoint);
							obj->BeAttacked(bullet);
							delBullet(bullet);
							if (obj->isDead())
							{
								m_score += obj->scroe();
								delZombie(obj);
								break;
							}
						}
						obj->Move(MOVE_LEFT);
						m_vObjMap[nextPoint.Y][nextPoint.X + 1]->repaint = true;
						m_vObjMap[nextPoint.Y][nextPoint.X]->objType = obj->objectType();
						m_vObjMap[nextPoint.Y][nextPoint.X]->repaint = true;
					}
				}
			}
			break;
		case ZOMBIE_JACKINTHEBOX:
			if (obj->Main())
			{
				if (obj->position().X <= 0) return 1;
				Point nextPoint = obj->position() - Point(1, 0);
				if (obj->Attack())
				{
					Point pos = obj->position() - Point(1, 1);
					for (int y = 0; y < 3; y++)
					{
						for (int x = 0; x < 3; ++x)
						{
							if (x == y && x == 1) continue;
							if (isPlant(pos + Point(x, y)))
							{
								delPlant(getPlant(pos + Point(x, y)));
							}
						}
					}
					delZombie(obj);
					break;
				}
				if (isPlant(nextPoint))
				{
					Base *plant = getPlant(nextPoint);
					plant->BeAttacked(obj);
					if (plant->objectType() == PLANT_GARLIC && plant->isNegative() == false)
					{
						if (obj->position().Y != 0 && !isPlant(obj->position() + Point(0, -1)))
						{
							m_vObjMap[obj->position().Y][obj->position().X]->repaint = true;
							obj->Move(MOVE_UP);
							m_vObjMap[obj->position().Y][obj->position().X]->objType = obj->objectType();
							m_vObjMap[obj->position().Y][obj->position().X]->repaint = true;
						}
						else if (obj->position().Y != MAXBLOCK_Y && !isPlant(obj->position() + Point(0, 1)))
						{
							m_vObjMap[obj->position().Y][obj->position().X]->repaint = true;
							obj->Move(MOVE_DOWN);
							m_vObjMap[obj->position().Y][obj->position().X]->objType = obj->objectType();
							m_vObjMap[obj->position().Y][obj->position().X]->repaint = true;
						}
						plant->setNegative(true);
					}
					if (plant->isDead())
					{
						delPlant(plant);
					}
				}
				else
				{
					if (isBullet(nextPoint))
					{
						Base *bullet = getBullet(nextPoint);
						obj->BeAttacked(bullet);
						delBullet(bullet);
						if (obj->isDead())
						{
							m_score += obj->scroe();
							delZombie(obj);
							break;
						}
					}
					obj->Move(MOVE_LEFT);
					m_vObjMap[nextPoint.Y][nextPoint.X + 1]->repaint = true;
					m_vObjMap[nextPoint.Y][nextPoint.X]->objType = obj->objectType();
					m_vObjMap[nextPoint.Y][nextPoint.X]->repaint = true;
				}
			}
			break;
		case ZOMBIE_POLEVAULTING:
			if (obj->Main())
			{
				if (obj->position().X <= 0) return 1;
				Point nextPoint = obj->position() - Point(1, 0);
				if (isPlant(nextPoint))
				{
					if (!isPlant(nextPoint - Point(1, 0)) && obj->isNegative() == false)
					{
						if (getPlant(nextPoint)->objectType() == PLANT_TALLNUT)
						{
							obj->setNegative(true);
							break;
						}
						m_vObjMap[obj->position().Y][obj->position().X]->repaint = true;
						obj->Move(MOVE_LEFT);
						obj->Move(MOVE_LEFT);
						obj->setNegative(true);
						if (obj->position().X < 0) return 1;
						m_vObjMap[obj->position().Y][obj->position().X]->objType = obj->objectType();
						m_vObjMap[obj->position().Y][obj->position().X]->repaint = true;
					}
					else
					{
						Base *plant = getPlant(nextPoint);
						plant->BeAttacked(obj);
						if (plant->objectType() == PLANT_GARLIC && plant->isNegative() == false)
						{
							if (obj->position().Y != 0 && !isPlant(obj->position() + Point(0, -1)))
							{
								m_vObjMap[obj->position().Y][obj->position().X]->repaint = true;
								obj->Move(MOVE_UP);
								m_vObjMap[obj->position().Y][obj->position().X]->objType = obj->objectType();
								m_vObjMap[obj->position().Y][obj->position().X]->repaint = true;
							}
							else if (obj->position().Y != MAXBLOCK_Y && !isPlant(obj->position() + Point(0, 1)))
							{
								m_vObjMap[obj->position().Y][obj->position().X]->repaint = true;
								obj->Move(MOVE_DOWN);
								m_vObjMap[obj->position().Y][obj->position().X]->objType = obj->objectType();
								m_vObjMap[obj->position().Y][obj->position().X]->repaint = true;
							}
							plant->setNegative(true);
						}
						if (plant->isDead())
						{
							delPlant(plant);
						}
					}
				}
				else
				{
					if (isBullet(nextPoint))
					{
						Base *bullet = getBullet(nextPoint);
						obj->BeAttacked(bullet);
						delBullet(bullet);
						if (obj->isDead())
						{
							m_score += obj->scroe();
							delZombie(obj);
							break;
						}
					}
					obj->Move(MOVE_LEFT);
					m_vObjMap[nextPoint.Y][nextPoint.X + 1]->repaint = true;
					m_vObjMap[nextPoint.Y][nextPoint.X]->objType = obj->objectType();
					m_vObjMap[nextPoint.Y][nextPoint.X]->repaint = true;
				}
			}
			break;
		case ZOMBIE_DANCING:
			break;
		case ZOMBIE_BACKUP:
			break;
		default:
			break;
		}
	}
	for (unsigned int i = 0; i < m_vBulletObj.size(); ++i)
	{
		obj = m_vBulletObj[i];
		if (obj->Main())
		{
			if (obj->bulletType() == BULLET_BASKETBALL)
			{
				if (obj->position().X <= 0)
				{
					delBullet(obj);
					continue;
				}
				Point nextPoint = obj->position() + Point(-1, 0);
				if (isPlant(nextPoint))
				{
					Base *plant = getPlant(nextPoint);
					plant->BeAttacked(obj);
					if (plant->isDead())
					{
						m_vObjMap[nextPoint.Y][nextPoint.X]->objType = obj->objectType();
						m_vObjMap[nextPoint.Y][nextPoint.X]->repaint = true;
						delPlant(plant);
					}
					delBullet(obj);
					m_vObjMap[nextPoint.Y][nextPoint.X + 1]->negative = true;
				}
				else
				{
					obj->Move(MOVE_LEFT);
					if (obj->position().X < 0)
					{
						delBullet(obj);
						m_vObjMap[nextPoint.Y][nextPoint.X + 1]->negative = true;
					}
				}
			}
			else
			{
				if (obj->position().X >= MAXBLOCK_X - 1)
				{
					delBullet(obj);
					continue;
				}
				Point nextPoint = obj->position() + Point(1, 0);
				if (isZombie(nextPoint))
				{
					std::vector<Base*> zombies;
					if (obj->bulletType() == BULLET_MELON)
					{
						if (isZombie(nextPoint + Point(0, -1))) zombies.push_back(getZombie(nextPoint + Point(0, -1)));
						if (isZombie(nextPoint + Point(0, 1))) zombies.push_back(getZombie(nextPoint + Point(0, 1)));
					}
					zombies.push_back(getZombie(nextPoint));

					for (unsigned int i = 0; i < zombies.size(); ++i)
					{
						if (zombies[i]->position() == nextPoint)
						{
							for (int j = 0; j < 4; ++j)
							{
								zombies[i]->BeAttacked(obj);
							}
						}
						else
							zombies[i]->BeAttacked(obj);
						if (zombies[i]->isNegative() == true)
						{
							m_vObjMap[zombies[i]->position().Y][zombies[i]->position().X]->repaint = true;
						}
						if (zombies[i]->isDead())
						{
							m_vObjMap[zombies[i]->position().Y][zombies[i]->position().X]->repaint = true;
							m_score += zombies[i]->scroe();
							delZombie(zombies[i]);
						}
					}
					delBullet(obj);
					m_vObjMap[nextPoint.Y][nextPoint.X - 1]->negative = true;
				}
				else
				{
					obj->Move(MOVE_RIGHT);
				}
			}
		}
	}
	for (unsigned int i = 0; i < m_vStoreMap.size(); ++i)
	{
		if (m_vStoreMap[i]->cnt < m_vStoreMap[i]->coolingTime)
		{
			m_vStoreMap[i]->cnt++;
		}
	}
	cnt1++;
	if (start)
	{
		for (unsigned int i = 0; i < m_vZombieMap.size(); ++i)
		{
			m_vZombieMap[i]->cnt--;
			if (m_vZombieMap[i]->cnt <= 0)
			{
				m_vZombieMap[i]->cnt = m_vZombieMap[i]->coolingTime;
				int y = rand() % 5;
				if(isPlant(Point(MAXBLOCK_X - 2, y)))
					NewObject(m_vZombieMap[i]->objType, Point(MAXBLOCK_X - 1, y));
				else
					NewObject(m_vZombieMap[i]->objType, Point(MAXBLOCK_X - 2, y));
			}
		}
	}
	else
	{
		if (cnt1 >= 15 * 1000 / CYCLETIME) start = true;
	}
	cnt++;
	if (cnt >= 5 * 1000 / CYCLETIME)
	{
		cnt = 0;
		m_sunCnt += 25;
	}
	return 0;
}

void Model::SetStoreMapItemNegative(unsigned int index)
{
	index = max(0, index);
	index = min(index, m_vStoreMap.size() - 1);
	ResetStoreMapItemNegative();
	m_vStoreMap[index]->negative = true;
}

void Model::ResetStoreMapItemNegative()
{
	for (unsigned int i = 0; i < m_vStoreMap.size(); ++i)
	{
		m_vStoreMap[i]->negative = false;
	}
}

void Model::SetObjectMapItemNegative(const Point &p)
{
	ResetObjectMapItemNegative();
	m_vObjMap[p.Y][p.X]->negative = true;
}

void Model::ResetObjectMapItemNegative()
{
	for (unsigned int j = 0; j < MAXBLOCK_X - 1; ++j)
	{
		for (unsigned int i = 0; i < MAXBLOCK_Y; ++i)
		{
			m_vObjMap[i][j]->negative = false;
		}
	}
}

void Model::InitObjMap()
{
	PObjMapItem pMapItem;
	for (unsigned int j = 0; j < MAXBLOCK_X; ++j)
	{
		for (unsigned int i = 0; i < MAXBLOCK_Y; ++i)
		{
			pMapItem = new ObjMapItem(NONE_TYPE, false);
			m_vObjMap[i][j] = pMapItem;
		}
	}
}

void Model::GenerateStoreMap()
{
	PObjInfo pInfo;
	PObjMapItem pObjMapItem;
	for (int type = PLANT_SUNNER; type <= PLANT_GARLIC; ++type)
	{
		pInfo = Base::GetInfo((OBJECT_TYPE)type);
		if (!pInfo)
		{
			std::cout << "Type " << type << " Info NOT FOUND." << std::endl;
			system("pause");
			exit(-3);
		}
		pObjMapItem = new ObjMapItem((OBJECT_TYPE)type, names[type], pInfo->coolingTime * 1000 / CYCLETIME, pInfo->cost, false, false);
		m_vStoreMap.push_back(pObjMapItem);
	}
}

void Model::GenerateZombieMap()
{
	PObjInfo pInfo;
	PObjMapItem pObjMapItem;
	for (int type = ZOMBIE_NORMAL; type <= ZOMBIE_CATAPULT; ++type)
	{
		pInfo = Base::GetInfo((OBJECT_TYPE)type);
		if (!pInfo)
		{
			std::cout << "Type " << type << " Info NOT FOUND." << std::endl;
			system("pause");
			exit(-3);
		}
		pObjMapItem = new ObjMapItem((OBJECT_TYPE)type, names[type], pInfo->coolingTime * 1000 / CYCLETIME, pInfo->cost, false, false);
		m_vZombieMap.push_back(pObjMapItem);
	}
}


void Model::delPlant(const Base * obj)
{
	unsigned int i;
	for (i = 0; i < m_vPlantsObj.size(); ++i)
	{
		if (m_vPlantsObj[i] == obj) break;
	}
	for (; i < m_vPlantsObj.size() - 1; ++i)
	{
		m_vPlantsObj[i] = m_vPlantsObj[i + 1];
	}
	m_vPlantsObj.pop_back();
	for (i = 0; i < m_vPlantsObj.size(); ++i)
	{
		if (m_vPlantsObj[i]->position() == obj->position()) break;
	}
	if (i == m_vPlantsObj.size()) m_vObjMap[obj->position().Y][obj->position().X]->objType = NONE_TYPE;
	m_vObjMap[obj->position().Y][obj->position().X]->repaint = true;
	delete obj;
}

void Model::delZombie(const Base * obj)
{
	unsigned int i;
	for (i = 0; i < m_vZombieObj.size(); ++i)
	{
		if (m_vZombieObj[i] == obj) break;
	}
	for (; i < m_vZombieObj.size() - 1; ++i)
	{
		m_vZombieObj[i] = m_vZombieObj[i + 1];
	}
	m_vZombieObj.pop_back();
	for (i = 0; i < m_vZombieObj.size(); ++i)
	{
		if (m_vZombieObj[i]->position() == obj->position()) break;
	}
	if(i == m_vZombieObj.size()) m_vObjMap[obj->position().Y][obj->position().X]->objType = NONE_TYPE;
	m_vObjMap[obj->position().Y][obj->position().X]->repaint = true;
	delete obj;
}

void Model::delBullet(const Base * obj)
{
	unsigned int i;
	for (i = 0; i < m_vBulletObj.size(); ++i)
	{
		if (m_vBulletObj[i] == obj) break;
	}
	for (; i < m_vBulletObj.size() - 1; ++i)
	{
		m_vBulletObj[i] = m_vBulletObj[i + 1];
	}
	m_vBulletObj.pop_back();
	m_vObjMap[obj->position().Y][obj->position().X]->negative = true;
	delete obj;
}

bool Model::Readin(void)
{
	PObjInfo pObj = NULL;
	FILE *fp = fopen("./data/modelInfo.csv", "r");
	if (!fp)
	{
		std::cout << "Open file " << "./data/modelInfo.csv" << "failed." << std::endl;
		exit(0);
	}
	char line[128];
	int hp;
	int moveSpeed;
	int attack;
	int coolingTime;
	int cost;
	int score;
	while (EOF != fscanf(fp, "%[^,],%d,%d,%d,%d,%d,%d%*c", line, &hp, &moveSpeed, &attack, &coolingTime, &cost, &score))
	{
		if (!strcmp(line, "ZOMBIE_NORMAL"))
		{
			pObj = new ObjInfo(ZOMBIE_NORMAL, hp, moveSpeed, attack, coolingTime, cost, score);
		}
		else if (!strcmp(line, "ZOMBIE_CONEHEAD"))
		{
			pObj = new ObjInfo(ZOMBIE_CONEHEAD, hp, moveSpeed, attack, coolingTime, cost, score);
		}
		else if (!strcmp(line, "ZOMBIE_BUCKETHEAD"))
		{
			pObj = new ObjInfo(ZOMBIE_BUCKETHEAD, hp, moveSpeed, attack, coolingTime, cost, score);
		}
		else if (!strcmp(line, "ZOMBIE_SCREENDOOR"))
		{
			pObj = new ObjInfo(ZOMBIE_SCREENDOOR, hp, moveSpeed, attack, coolingTime, cost, score);
		}
		else if (!strcmp(line, "ZOMBIE_FLAG"))
		{
			pObj = new ObjInfo(ZOMBIE_FLAG, hp, moveSpeed, attack, coolingTime, cost, score);
		}
		else if (!strcmp(line, "ZOMBIE_FOOTBALL"))
		{
			pObj = new ObjInfo(ZOMBIE_FOOTBALL, hp, moveSpeed, attack, coolingTime, cost, score);
		}
		else if (!strcmp(line, "ZOMBIE_JACKINTHEBOX"))
		{
			pObj = new ObjInfo(ZOMBIE_JACKINTHEBOX, hp, moveSpeed, attack, coolingTime, cost, score);
		}
		else if (!strcmp(line, "ZOMBIE_NEWSPAPER"))
		{
			pObj = new ObjInfo(ZOMBIE_NEWSPAPER, hp, moveSpeed, attack, coolingTime, cost, score);
		}
		else if (!strcmp(line, "ZOMBIE_POLEVAULTING"))
		{
			pObj = new ObjInfo(ZOMBIE_POLEVAULTING, hp, moveSpeed, attack, coolingTime, cost, score);
		}
		else if (!strcmp(line, "ZOMBIE_CATAPULT"))
		{
			pObj = new ObjInfo(ZOMBIE_CATAPULT, hp, moveSpeed, attack, coolingTime, cost, score);
		}
		else if (!strcmp(line, "ZOMBIE_DANCING"))
		{
			pObj = new ObjInfo(ZOMBIE_DANCING, hp, moveSpeed, attack, coolingTime, cost, score);
		}
		else if (!strcmp(line, "ZOMBIE_BACKUP"))
		{
			pObj = new ObjInfo(ZOMBIE_BACKUP, hp, moveSpeed, attack, coolingTime, cost, score);
		}
		else if (!strcmp(line, "PLANT_SNOWPEA"))
		{
			pObj = new ObjInfo(PLANT_SNOWPEA, hp, moveSpeed, attack, coolingTime, cost, score);
		}
		else if (!strcmp(line, "PLANT_PEASHOOTER"))
		{
			pObj = new ObjInfo(PLANT_PEASHOOTER, hp, moveSpeed, attack, coolingTime, cost, score);
		}
		else if (!strcmp(line, "PLANT_REPEATER"))
		{
			pObj = new ObjInfo(PLANT_REPEATER, hp, moveSpeed, attack, coolingTime, cost, score);
		}
		else if (!strcmp(line, "PLANT_MELONPULT"))
		{
			pObj = new ObjInfo(PLANT_MELONPULT, hp, moveSpeed, attack, coolingTime, cost, score);
		}
		else if (!strcmp(line, "PLANT_SPIKEROCK"))
		{
			pObj = new ObjInfo(PLANT_SPIKEROCK, hp, moveSpeed, attack, coolingTime, cost, score);
		}
		else if (!strcmp(line, "PLANT_WALLNUT"))
		{
			pObj = new ObjInfo(PLANT_WALLNUT, hp, moveSpeed, attack, coolingTime, cost, score);
		}
		else if (!strcmp(line, "PLANT_TALLNUT"))
		{
			pObj = new ObjInfo(PLANT_TALLNUT, hp, moveSpeed, attack, coolingTime, cost, score);
		}
		else if (!strcmp(line, "PLANT_SQUASH"))
		{
			pObj = new ObjInfo(PLANT_SQUASH, hp, moveSpeed, attack, coolingTime, cost, score);
		}
		else if (!strcmp(line, "PLANT_POTATOMINE"))
		{
			pObj = new ObjInfo(PLANT_POTATOMINE, hp, moveSpeed, attack, coolingTime, cost, score);
		}
		else if (!strcmp(line, "PLANT_CHERRYBOMB"))
		{
			pObj = new ObjInfo(PLANT_CHERRYBOMB, hp, moveSpeed, attack, coolingTime, cost, score);
		}
		else if (!strcmp(line, "PLANT_JALAPENO"))
		{
			pObj = new ObjInfo(PLANT_JALAPENO, hp, moveSpeed, attack, coolingTime, cost, score);
		}
		else if (!strcmp(line, "PLANT_SUNNER"))
		{
			pObj = new ObjInfo(PLANT_SUNNER, hp, moveSpeed, attack, coolingTime, cost, score);
		}
		else if (!strcmp(line, "PLANT_GARLIC"))
		{
			pObj = new ObjInfo(PLANT_GARLIC, hp, moveSpeed, attack, coolingTime, cost, score);
		}
		else if (!strcmp(line, "BULLET_ZOMBIE"))
		{
			pObj = new ObjInfo(BULLET_ZOMBIE, hp, moveSpeed, attack, coolingTime, cost, score);
		}
		else if (!strcmp(line, "BULLET_BASKETBALL"))
		{
			pObj = new ObjInfo(BULLET_BASKETBALL, hp, moveSpeed, attack, coolingTime, cost, score);
		}
		else if (!strcmp(line, "BULLET_SUN"))
		{
			pObj = new ObjInfo(BULLET_SUN, hp, moveSpeed, attack, coolingTime, cost, score);
		}
		else if (!strcmp(line, "BULLET_NORMAL"))
		{
			pObj = new ObjInfo(BULLET_NORMAL, hp, moveSpeed, attack, coolingTime, cost, score);
		}
		else if (!strcmp(line, "BULLET_TWO"))
		{
			pObj = new ObjInfo(BULLET_TWO, hp, moveSpeed, attack, coolingTime, cost, score);
		}
		else if (!strcmp(line, "BULLET_SNOW"))
		{
		pObj = new ObjInfo(BULLET_SNOW, hp, moveSpeed, attack, coolingTime, cost, score);
		}
		else if (!strcmp(line, "BULLET_MELON"))
		{
			pObj = new ObjInfo(BULLET_MELON, hp, moveSpeed, attack, coolingTime, cost, score);
		}
		else if (!strcmp(line, "BULLET_SECKILL"))
		{
			pObj = new ObjInfo(BULLET_SECKILL, hp, moveSpeed, attack, coolingTime, cost, score);
		}
		else
		{
			std::cout << "ERROR TYPE: " << line << std::endl;
			fclose(fp);
			system("pause");
			exit(0);
		}
		g_vObjInfo.push_back(pObj);
	}
	return true;
}