#define _CRT_SECURE_NO_WARNINGS
#include "View.h"
#include <iostream>
#include <vector>
#include <cstdio>

extern LPCWSTR names[];

View::View():
m_mapPos(0, 0),
m_preStorePos(-1, -1),
m_preMapPos(0, 0)

{
	InitPainter();
}

View::~View()
{
	if(m_hOutput)
		CloseHandle(m_hOutput);
}

void View::DrawFrame()
{
	int x = m_mapPos.X;
	int y = m_mapPos.Y;
	int precolor = getColor();
	setColor(EDGE_COLOR);
	for (y = 0; y < MAPSIZE_Y; ++y)
	{
		for (x = 0; x < MAPSIZE_X - BLOCKWIDTH - 1; ++x)
		{
			if (y % (BLOCKHEIGHT + 1) == 0 || x % (BLOCKWIDTH + 1) == 0)
			{
				drawtext(x * 2, y, L"█");
			}
		}
		if (y == 0 || y == MAPSIZE_Y - 1)
		{
			for (; x < MAPSIZE_X; ++x)
			{
				drawtext(x * 2, y, L"█");
			}
		}
	}

	x = 0;
	y += 3;
	setColor(STORE_COLOR);
	for (x = 0; x < MAPSIZE_X * 2; ++x)
	{
		drawtext(x, y, L"=");
		drawtext(x, y + 7, L"=");
	}
	x = 0;
	y += 1;
	x = 1;
	//信息第一行：[商店]	阳光总数：xxx | [得分]
	drawtext(x, y, L"[商店]");
	x = (int)(0.7 * MAPSIZE_X * 2);
	drawtext(x, y, L"阳光总数：");
	m_sunCntPos.X = (int)(0.7 * MAPSIZE_X * 2) + lstrlenW(L"阳光总数：") * 2 + 1;
	m_sunCntPos.Y = y;
	drawtext(m_sunCntPos.X, m_sunCntPos.Y, L"0");
	x = (int)(0.82 * MAPSIZE_X * 2);
	drawtext(x, y, L"|");
	x = (int)(0.88 * MAPSIZE_X * 2);
	drawtext(x, y, L"[得分]");
	y += 1;
	//第二行分割线
	for (x = 0; x < MAPSIZE_X * 2; ++x)
	{
		drawtext(x, y, L"-");
	}
	y += 1;
	m_storePos.X = 0;
	m_storePos.Y = y;
	//商店信息首行，包括得分
	for (int i = 0; i < 4; ++i)
	{
		x = (int)(0.82 * MAPSIZE_X * 2);
		drawtext(x, y + i, L"|");
	}
	m_scorePos.X = (int)(0.88 * MAPSIZE_X * 2) + 1;
	m_scorePos.Y = y;
	drawtext(m_scorePos.X, m_scorePos.Y, L"0");
	
	x = 0;
	y += 6;
	gotoxy(x, y);
	drawtext(x, y, L"操作指南： b：商店, c：铲子, ESC：取消, 回车：确认");
	y += 2;
	gotoxy(x, y);
	setColor(precolor);
}

void View::DrawScore(int score)
{
	Point pos = GetCursorPos();
	InvalidateBlock(m_scorePos, m_scorePos + Point(5, 0));
	WCHAR szbuf[16];
	wsprintf(szbuf, L"%d", score);
	drawtext(m_scorePos.X, m_scorePos.Y, szbuf);
	SetCursorPos(pos);
}

void View::DrawSunCnt(int cnt)
{
	Point pos = GetCursorPos();
	InvalidateBlock(m_sunCntPos, m_sunCntPos + Point(4, 0));
	WCHAR szbuf[16];
	wsprintf(szbuf, L"%d", cnt);
	drawtext(m_sunCntPos.X, m_sunCntPos.Y, szbuf);
	SetCursorPos(pos);
}

void View::DrawStore(const std::vector<PObjMapItem>& storeMap)
{
	Point pos = GetCursorPos();
	unsigned int index = 0;
	WCHAR buf[64];
	for (int y = m_storePos.Y; y < m_storePos.Y + STOREBLOCK_Y; ++y)
	{
		for (int x = 0.05 * MAPSIZE_X * 2; x < 0.2 * STOREBLOCK_X * MAPSIZE_X * 2; x += 0.2 * MAPSIZE_X * 2)
		{
			if (storeMap[index]->cnt < storeMap[index]->coolingTime)
			{
				int persent = storeMap[index]->coolingTime - storeMap[index]->cnt;
				wsprintf(buf, L"%s(冷却中): %d%%", storeMap[index]->name, persent);
			}
			else
			{
				wsprintf(buf, L"%s: %d", storeMap[index]->name, storeMap[index]->cost);
			}
			int preColor = getColor();
			if (index++ == m_preStorePos.X + m_preStorePos.Y * STOREBLOCK_X) setColor(CHOSEN_COLOR);
			else setColor(STORE_COLOR);
			InvalidateBlock(Point(x, y), Point(x + 24, y));
			drawtext(x, y, buf);
			setColor(preColor);
			if (index >= storeMap.size()) break;
		}
		if (index >= storeMap.size()) break;
	}
	SetCursorPos(pos);
}

void View::DrawMap(const Model &model)
{
	for (unsigned int i = 0; i < model.ObjMap().size(); ++i)
	{
		for (unsigned int j = 0; j < model.ObjMap()[0].size(); ++j)
		{
			if (model.ObjMap()[i][j]->negative)
			{
				InvalidateBulletTrace(Point(j, i));
				model.ObjMap()[i][j]->negative = false;
			}
			if (model.ObjMap()[i][j]->repaint)
			{
				Base *obj;
				if (model.ObjMap()[i][j]->objType == NONE_TYPE)
				{
					InvalidateMap(Point(j, i));
				}
				else if (model.ObjMap()[i][j]->objType >= PLANT_SUNNER)
				{
					obj = model.getPlant(Point(j, i));
					drawObj(obj, Point(j, i));
				}
				else
				{
					std::vector<Base*> zombies;
					for (unsigned k = 0; k < model.ZombieObj().size(); ++k)
					{
						if (model.ZombieObj()[k]->position() == Point(j, i))
							zombies.push_back(model.ZombieObj()[k]);
					}
					InvalidateMap(Point(j, i));
					drawZombies(zombies, Point(j, i));
				}
				model.ObjMap()[i][j]->repaint = false;
			}
		}
	}
	for (unsigned int i = 0; i < model.BulletObj().size(); ++i)
	{
		drawObj(model.BulletObj()[i], model.BulletObj()[i]->position());
	}
}

void View::DrawStoreSelect(const std::vector<PObjMapItem> &storeMap, const Point & p)
{
	drawStoreUnselect(storeMap);
	drawStoreSelect(storeMap, p);
	m_preStorePos = p;
}

void View::ClearStoreSelect(const std::vector<PObjMapItem> &storeMap)
{
	drawStoreUnselect(storeMap);
	m_preStorePos = Point(-1, -1);
}

void View::DrawMapSelect(const Point &p)
{
	drawMapUnselect();
	drawMapSelect(p);
	m_preMapPos = p;
}

void View::ClearMapSelect()
{
	drawMapUnselect();
	m_preMapPos = Point(0, 0);
}

void View::InvalidateBlock(Point leftTop, Point rightBottom)
{
	Point src = GetCursorPos();
	int preColor = getColor();
	setColor(EDGE_COLOR);
	for (int y = leftTop.Y; y <= rightBottom.Y; ++y)
	{
		for (int x = leftTop.X; x <= rightBottom.X; ++x)
		{
			drawtext(x, y, L" ");
		}
	}
	setColor(preColor);
	SetCursorPos(src);
}

void View::InvalidateMap(const Point & p)
{
	Point point = m_mapPos + Point(1, 1);
	point += p * Point(BLOCKWIDTH * 2 + 2, BLOCKHEIGHT + 1);
	InvalidateBlock(Point(point.X + 1, point.Y + 1), Point(point.X + BLOCKWIDTH * 2, point.Y - 2 + BLOCKHEIGHT));
}

void View::InvalidateBulletTrace(const Point & p)
{
	Point pos = GetCursorPos();
	Point point = m_mapPos + Point(1, 1);
	point += p * Point(BLOCKWIDTH * 2 + 2, BLOCKHEIGHT + 1);
	point += Point(0, 3);
	point += Point(1, 0);
	drawtext(point.X, point.Y, L"            ");
	SetCursorPos(pos);
}

void View::SetCursorPos(short x, short y)
{
	if(x >= 0) m_cursorPos.X = x;
	if(y >= 0) m_cursorPos.Y = y;
	gotoxy(m_cursorPos.X, m_cursorPos.Y);
}

void View::SetCursorPos(const Point & p)
{
	if (p.X >= 0) m_cursorPos.X = p.X;
	if (p.Y >= 0) m_cursorPos.Y = p.Y;
	gotoxy(m_cursorPos.X, m_cursorPos.Y);
}

void View::SetCursorPosRe(short reX, short reY)
{
	gotoxy(m_cursorPos.X + reX, m_cursorPos.Y + reY);
}

int View::getColor()
{
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	GetConsoleScreenBufferInfo(m_hOutput, &bInfo);
	return bInfo.wAttributes;
}

/*
0 = 黑色       8 = 灰色
1 = 蓝色       9 = 淡蓝色
2 = 绿色       A = 淡绿色
3 = 浅绿色     B = 淡浅绿色
4 = 红色       C = 淡红色
5 = 紫色       D = 淡紫色
6 = 黄色       E = 淡黄色
7 = 白色       F = 亮白色
*/
void View::setColor(int c)
{
	SetConsoleTextAttribute(m_hOutput, c);
}
void View::gotoxy(int x, int y)
{
	COORD cr;
	cr.X = m_cursorPos.X = x;
	cr.Y = m_cursorPos.Y = y;
	SetConsoleCursorPosition(m_hOutput, cr);
}
void View::drawtext(int x, int y, LPCWSTR s)
{
	gotoxy(x, y);
	wprintf(s);
}

void View::modeset(int w, int h)
{
	COORD size;
	size.X = w;
	size.Y = h;
	SetConsoleScreenBufferSize(m_hOutput, size);
	SMALL_RECT rc;
	rc.Left = 1;
	rc.Top = 1;
	rc.Right = w;
	rc.Bottom = h;
	SetConsoleWindowInfo(m_hOutput, true, &rc);
	system("cls");
}

void View::InitPainter()
{
	system("color F0");
	m_hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(m_hOutput, &cci);
	cci.bVisible = 0;
	SetConsoleCursorInfo(m_hOutput, &cci);
	modeset(MAPSIZE_X * 2, MAPSIZE_Y * 2);
	char smode[64];
	sprintf(smode, "mode con cols=%d lines=%d", MAPSIZE_X * 2, MAPSIZE_Y * 3 / 2);
	system(smode);
	setlocale(LC_CTYPE, "");
	SetConsoleTitleW(L"植物大战僵尸");
	system("cls");
}

void View::drawObj(const Base *obj, const Point & p)
{
	Point pos = GetCursorPos();
	int preColor = getColor();
	Point point = m_mapPos + Point(1, 1);
	if (obj->objectType() >= BULLET_ZOMBIE)
	{
		point += p * Point(BLOCKWIDTH * 2 + 2, BLOCKHEIGHT + 1);

		point += Point(0, 3);
		//InvalidateBlock(point, point + Point(BLOCKWIDTH * 2, 0));
		point += Point(2 * obj->cnt() + 1, 0);
		if (obj->objectType() == BULLET_BASKETBALL)
		{
			drawtext(point.X - 2, point.Y, names[obj->objectType()]);
			if (obj->cnt() == obj->frames())
			{
				drawtext(point.X + 2, point.Y, L"  ");
			}
			else
			{
				drawtext(point.X, point.Y, L"  ");
			}
		}
		else
		{
			drawtext(point.X, point.Y, names[obj->objectType()]);
			if (obj->cnt() == 0)
			{
					drawtext(point.X - 4, point.Y, L"  ");
			}
			else
			{
					drawtext(point.X - 2, point.Y, L"  ");
			}
		}
	}
	else if (obj->objectType() >= PLANT_SUNNER)
	{
		point += p * Point(BLOCKWIDTH * 2 + 2, BLOCKHEIGHT + 1);
		point += Point(2, 2 * 1);
		InvalidateBlock(point, point + Point(BLOCKWIDTH, 0));
		drawtext(point.X, point.Y, names[obj->objectType()]);
	}
	else
	{
		point += p * Point(BLOCKWIDTH * 2 + 2, BLOCKHEIGHT + 1);
		point += Point(2, 2 * 1);
		InvalidateBlock(point, point + Point(BLOCKWIDTH, 0));
		drawtext(point.X, point.Y, names[obj->objectType()]);
	}
	setColor(preColor);
	SetCursorPos(pos);
}

void View::drawZombies(const std::vector<Base*>& zombies, const Point & p)
{
	Point pos = GetCursorPos();
	int preColor = getColor();
	if (zombies.size() >= 5)
	{
		Point point = m_mapPos + Point(1, 1);

		point += p * Point(BLOCKWIDTH * 2 + 2, BLOCKHEIGHT + 1);
		point += Point(3, 2);
		InvalidateBlock(point, point + Point(8, 0));
		WCHAR buf[16];
		wsprintf(buf, L"僵尸(x%u)", zombies.size());
		drawtext(point.X, point.Y, buf);
	}
	else
	{
		Point point = m_mapPos + Point(1, 1);

		point += p * Point(BLOCKWIDTH * 2 + 2, BLOCKHEIGHT + 1);
		point += Point(3, 1);
		int x = point.X;
		for (unsigned int i = 0; i < zombies.size(); ++i)
		{
			InvalidateBlock(point, point + Point(9, 0));
			drawtext(point.X, point.Y, names[zombies[i]->objectType()]);
			
			if (zombies[i]->objectType() == ZOMBIE_NEWSPAPER)
			{
				if (zombies[i]->isNegative() == true)
					drawtext(point.X + 8, point.Y, L"N");
				else
					drawtext(point.X + 8, point.Y, L"Y");
			}
			point.X = x;
			point.Y++;
		}
	}
	setColor(preColor);
	SetCursorPos(pos);
}

void View::drawStoreSelect(const std::vector<PObjMapItem> &storeMap, const Point & p)
{
	Point pos = GetCursorPos();
	unsigned int index = 0;
	WCHAR buf[64];
	for (int y = m_storePos.Y; y < m_storePos.Y + STOREBLOCK_Y; ++y)
	{
		for (int x = 0.05 * MAPSIZE_X * 2; x < 0.2 * STOREBLOCK_X * MAPSIZE_X * 2; x += 0.2 * MAPSIZE_X * 2)
		{
			if (index != p.X + p.Y * STOREBLOCK_X)
			{
				index++;
				continue;
			}
			if (index >= storeMap.size()) break;
			if (storeMap[index]->cnt < storeMap[index]->coolingTime)
			{
				int persent = (int)((storeMap[index]->coolingTime - storeMap[index]->cnt) / (double)(storeMap[index]->coolingTime));
				wsprintf(buf, L"%s(冷却中): %d%%", storeMap[index]->name, persent);
			}
			else
			{
				wsprintf(buf, L"%s: %d", storeMap[index]->name, storeMap[index]->cost);
			}
			int preColor = getColor();
			setColor(CHOSEN_COLOR);
			InvalidateBlock(Point(x, y), Point(x + 24, y));
			drawtext(x, y, buf);
			setColor(preColor);
			SetCursorPos(pos);
			return;
		}
		if (index >= storeMap.size()) break;
	}
	SetCursorPos(pos);
}

void View::drawStoreUnselect(const std::vector<PObjMapItem> &storeMap)
{
	if (m_storePos.X < 0) return;
	Point pos = GetCursorPos();
	unsigned int index = 0;
	WCHAR buf[64];
	for (int y = m_storePos.Y; y < m_storePos.Y + STOREBLOCK_Y; ++y)
	{
		for (int x = 0.05 * MAPSIZE_X * 2; x < 0.2 * STOREBLOCK_X * MAPSIZE_X * 2; x += 0.2 * MAPSIZE_X * 2)
		{
			if (index != m_preStorePos.X + m_preStorePos.Y * STOREBLOCK_X)
			{
				index++;
				continue;
			}
			if (index >= storeMap.size()) break;
			if (storeMap[index]->cnt < storeMap[index]->coolingTime)
			{
				int persent = (int)((storeMap[index]->coolingTime - storeMap[index]->cnt) / (double)(storeMap[index]->coolingTime));
				wsprintf(buf, L"%s(冷却中): %d%%", storeMap[index]->name, persent);
				//wsprintf(buf, L"%s(冷却中): %d", storeMap[index]->name, storeMap[index]->cost);
			}
			else
			{
				wsprintf(buf, L"%s: %d", storeMap[index]->name, storeMap[index]->cost);
			}
			int preColor = getColor();
			setColor(STORE_COLOR);
			InvalidateBlock(Point(x, y), Point(x + 24, y));
			drawtext(x, y, buf);
			setColor(preColor);
			SetCursorPos(pos);
			return;
		}
		if (index >= storeMap.size()) break;
	}
	SetCursorPos(pos);
}

void View::drawMapSelect(const Point & p)
{
	Point prePos = GetCursorPos();
	int preColor = getColor();
	setColor(POS_COLOR);
	Point point = m_mapPos + Point(1, 1);
	point += p * Point(BLOCKWIDTH * 2 + 2, BLOCKHEIGHT + 1); 
	drawtext(point.X + 1, point.Y, L"◤");
	drawtext(point.X + 1 + (BLOCKWIDTH - 1) * 2, point.Y, L"◥");
	drawtext(point.X + 1, point.Y - 1 + BLOCKHEIGHT, L"◣");
	drawtext(point.X + 1 + (BLOCKWIDTH - 1) * 2, point.Y - 1 + BLOCKHEIGHT, L"◢");
	setColor(preColor);
	SetCursorPos(prePos);
}

void View::drawMapUnselect()
{
	Point prePos = GetCursorPos();
	int preColor = getColor();
	setColor(POS_COLOR);
	Point point = m_mapPos + Point(1, 1);
	point += m_preMapPos * Point(BLOCKWIDTH * 2 + 2, BLOCKHEIGHT + 1);
	drawtext(point.X + 1, point.Y, L"  ");
	drawtext(point.X + 1 + (BLOCKWIDTH - 1) * 2, point.Y, L"  ");
	drawtext(point.X + 1, point.Y - 1 + BLOCKHEIGHT, L"  ");
	drawtext(point.X + 1 + (BLOCKWIDTH - 1) * 2, point.Y - 1 + BLOCKHEIGHT, L"  ");
	setColor(preColor);
	SetCursorPos(prePos);
}

