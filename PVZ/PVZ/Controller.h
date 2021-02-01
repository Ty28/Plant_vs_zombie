#pragma once
#include "Model.h"
#include "View.h"

#define KEY_STORE ((char)0x62)		//b
#define KEY_SHOVEL ((char)0x63)		//c
#define KEY_ESC ((char)0x1b)
#define KEY_ENTER ((char)0xd)
#define KEY_UP ((char)0x48)
#define KEY_DOWN ((char)0x50)
#define KEY_LEFT ((char)0x4b)
#define KEY_RIGHT ((char)0x4d)
#define KEY_COOLING ((char)0x75)	//u
#define KEY_SUN ((char)0x69)		//i
#define KEY_ZOMBIE ((char)0x7a)		//z

enum SELECT_TYPE{
	NONE_SELECT,
	SEL_PLANT,
	NEW_PLANT,
	DEL_PLANT
};

class Controller
{
public:
	Controller();
	virtual ~Controller();
	int StartGame();
private:
	View m_cui;
	Model m_data;
};

