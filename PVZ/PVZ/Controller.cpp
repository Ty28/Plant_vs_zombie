#include "Controller.h"
#include <iostream>
#include <conio.h>


Controller::Controller()
{
	m_cui.DrawFrame();
	m_cui.DrawStore(m_data.StoreMap());
	m_cui.DrawSunCnt(m_data.sunCount());
	m_cui.DrawScore(m_data.scoreCount());
}


Controller::~Controller()
{
}

int Controller::StartGame()
{
	bool state = true;
	SELECT_TYPE type = NONE_SELECT;
	Point storeSelectPos;
	Point selectPos;
	int cnt = 0;
	int cycle = 0;
	int ch = '\0';
	while (state)
	{
		Sleep(CYCLETIME);
		m_cui.DrawMap(m_data);
		m_cui.DrawStore(m_data.StoreMap());
		m_cui.DrawScore(m_data.scoreCount());
		m_cui.DrawSunCnt(m_data.sunCount());
		
		if (m_data.Main())
		{
			printf("Game Over.\n");
			system("pause");
			exit(0);
		}
		if (_kbhit())
		{
			ch = _getch();
			if (ch == 0xe0)
			{
				ch = _getch();
			}
			Point begin = m_cui.GetCursorPos();
			Point end = begin + Point(16, 0);
			m_cui.InvalidateBlock(begin, end);
			switch (ch)
			{
			case KEY_UP:
				//printf("Press up: 0x%x\n", ch);
				if (type == NONE_SELECT) break;
				else if (type == SEL_PLANT)
				{
					if(selectPos.Y) selectPos.Y--;
					m_cui.DrawStoreSelect(m_data.StoreMap(), selectPos);
				}
				else
				{
					if (selectPos.Y) selectPos.Y--;
					m_cui.DrawMapSelect(selectPos);
				}
				break;
			case KEY_DOWN:
				//printf("Press down: 0x%x\n", ch);
				if (type == NONE_SELECT) break;
				else if (type == SEL_PLANT)
				{
					if (selectPos.Y < STOREBLOCK_Y - 1)
					{
						selectPos.Y++;
						if (selectPos.Y == STOREBLOCK_Y - 1) selectPos.X = 0;
					}
					m_cui.DrawStoreSelect(m_data.StoreMap(), selectPos);
				}
				else
				{
					if (selectPos.Y < MAXBLOCK_Y - 1) selectPos.Y++;
					m_cui.DrawMapSelect(selectPos);
				}
				break;
			case KEY_LEFT:
				//printf("Press left: 0x%x\n", ch);
				if (type == NONE_SELECT) break;
				else if (type == SEL_PLANT)
				{
					if (selectPos.Y == STOREBLOCK_Y - 1) break;
					if (selectPos.X) selectPos.X--;
					m_cui.DrawStoreSelect(m_data.StoreMap(), selectPos);
				}
				else
				{
					if (selectPos.X) selectPos.X--;
					m_cui.DrawMapSelect(selectPos);
				}
				break;
			case KEY_RIGHT:
				//printf("Press right: 0x%x\n", ch);
				if (type == NONE_SELECT) break;
				else if (type == SEL_PLANT)
				{
					if (selectPos.Y == STOREBLOCK_Y - 1) break;
					if (selectPos.X < STOREBLOCK_X - 1) selectPos.X++;
					m_cui.DrawStoreSelect(m_data.StoreMap(), selectPos);
				}
				else
				{
					if (selectPos.X < MAXBLOCK_X - 2) selectPos.X++;
					m_cui.DrawMapSelect(selectPos);
				}
				break;
			case KEY_ENTER:
				//printf("Press enter: 0x%x\n", ch);
				if (type == NONE_SELECT);
				else if (type == SEL_PLANT)
				{
					type = NEW_PLANT;
					storeSelectPos = selectPos;
					selectPos = Point(0, 0);
					m_cui.DrawMapSelect(selectPos);
				}
				else if(type == NEW_PLANT)
				{
					type = NONE_SELECT;
					m_cui.ClearStoreSelect(m_data.StoreMap());

					m_cui.ClearMapSelect();

					int index = SPTI(storeSelectPos);
					m_data.NewObject(m_data.StoreMap()[index]->objType, selectPos);
				}
				else
				{
					type = NONE_SELECT;
					m_cui.ClearMapSelect();
					if (m_data.isPlant(selectPos))
					{
						m_data.DelPlants(selectPos);
						m_cui.InvalidateMap(selectPos);
					}
				}
				break;
			case KEY_ESC:
				//printf("Press enter: 0x%x\n", ch);
				if (type == NONE_SELECT);
				else if (type == SEL_PLANT)
				{
					type = NONE_SELECT;
					m_cui.ClearStoreSelect(m_data.StoreMap());
				}
				else if (type == NEW_PLANT)
				{
					type = NONE_SELECT;
					m_cui.ClearStoreSelect(m_data.StoreMap());

					m_cui.ClearMapSelect();
				}
				else
				{
					type = NONE_SELECT;
					m_cui.ClearMapSelect();
				}
				break;
			case KEY_STORE:
				//printf("Press enter: 0x%x\n", ch);
				if (type != NONE_SELECT) break;
				type = SEL_PLANT;
				selectPos = Point(0, 0);
				m_cui.DrawStoreSelect(m_data.StoreMap(), selectPos);
				break;
			case KEY_SHOVEL:
				//printf("Press enter: 0x%x\n", ch);
				if (type != NONE_SELECT) break;
				type = DEL_PLANT;
				selectPos = Point(0, 0);
				m_cui.DrawMapSelect(selectPos);
				break;
			case KEY_COOLING:
				m_data.ClearCoolingState();
				break;
			case KEY_SUN:
				m_data.incSunCount(200);
				break;
			default:
				//printf("Press else: 0x%x\n", ch);
				break;
			}
		}
	}

	return 0;
}


