#pragma once
#include "EngineDefine.h"
class Guage
{
private:
	POINT m_pt;
	RECT m_rc;
	int m_iPercent,m_iWidth, m_iHeight;
	HBRUSH m_hCurBrush, m_hOldBrush;
public:
	void Init(int x, int y, int cx, int cy,int r ,int g ,int b);
	void Update(int value);
	void Draw();
	void Release();
	Guage();
	~Guage();
};

