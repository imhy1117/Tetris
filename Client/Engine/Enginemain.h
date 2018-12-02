#pragma once
#include "EngineDefine.h"
#include <string>

class Enginemain
{
private:
	int m_iSize_cx, m_iSize_cy;
	string m_strWinName;
public:
	Enginemain(string winName, int size_cx, int size_cy);
	~Enginemain();
	int StartEngine(HINSTANCE hInstance);
};


