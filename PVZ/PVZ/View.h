#pragma once
#include "Base.h"
#include "Model.h"
#include <vector>

enum {
	EDGE_COLOR = 0xF2,
	STORE_COLOR = 0xF3,
	POS_COLOR = 0xF4,
	CHOSEN_COLOR = 0x4F,
	PLANT_COLOR = 0xF2,
	ZOMBIE_COLOR = 0xF2
};

class View
{
public:
	View();
	virtual ~View();
	void DrawFrame();
	void DrawScore(int score);
	void DrawSunCnt(int cnt);
	void DrawStore(const std::vector<PObjMapItem> &storeMap);
	void DrawMap(const Model &model);
	void DrawStoreSelect(const std::vector<PObjMapItem> &storeMap, const Point &p);
	void ClearStoreSelect(const std::vector<PObjMapItem> &storeMap);
	void DrawMapSelect(const Point &p);
	void ClearMapSelect();
	void InvalidateBlock(Point leftTop, Point rightBottom);
	void InvalidateMap(const Point &p);
	void InvalidateBulletTrace(const Point &p);
public:
	void SetCursorPos(short x, short y);
	void SetCursorPos(const Point &p);
	void SetCursorPosRe(short reX, short reY);
	Point GetCursorPos() const { return m_cursorPos; }
private:
	HANDLE m_hOutput;
	Point m_cursorPos;
	Point m_mapPos;
	Point m_storePos;
	Point m_scorePos;
	Point m_sunCntPos;
	Point m_preStorePos;
	Point m_preMapPos;
	int getColor();
	void setColor(int c);
	void gotoxy(int x, int y);
	void drawtext(int x, int y, LPCWSTR s);
	void modeset(int w, int h);
	void InitPainter();
	void drawObj(const Base *model, const Point &p);
	void drawZombies(const std::vector<Base*> &zombies, const Point &p);
	void drawStoreSelect(const std::vector<PObjMapItem> &storeMap, const Point &p);
	void drawStoreUnselect(const std::vector<PObjMapItem> &storeMap);
	void drawMapSelect(const Point &p);
	void drawMapUnselect();
};

