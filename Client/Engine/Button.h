#pragma once
#include "EngineDefine.h"
#include <string>
using namespace std;
class Button
{
private:
	RECT m_rcButton;
	POINT m_pt;
	string m_strButton;
public:
	Button();
	void Init(int x,int y,string strFilename);
	bool CheckClick(POINT pt);
	void Draw();
	~Button();
};

