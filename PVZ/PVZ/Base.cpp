#include "Base.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>

extern std::vector<PObjInfo> g_vObjInfo;

Base::Base(LPCWSTR name, const Point &point, OBJECT_TYPE otype, OBJECT_TYPE btype, int frames, int cnt):
m_name(name),
m_point(point),
m_objectType(otype),
m_bulletType(btype),
m_frames(frames),
m_cnt(cnt),
m_negative(false)
{
	PObjInfo pInfo = GetInfo(m_objectType);
	if (!pInfo)
	{
		std::cout << "Object Type " << m_objectType << " Not Found." << std::endl;
		exit(-1);
	}
	m_hp = pInfo->hp;
	m_speed = pInfo->speed;
	m_attack = pInfo->attack;
	m_coolingTime = pInfo->coolingTime;
	m_cost = pInfo->cost;
	m_score = pInfo->score;
}

const PObjInfo Base::GetInfo(OBJECT_TYPE type)
{
	for (unsigned int i = 0; i < g_vObjInfo.size(); ++i)
	{
		if (g_vObjInfo[i]->type == type) return g_vObjInfo[i];
	}
	return NULL;
}




