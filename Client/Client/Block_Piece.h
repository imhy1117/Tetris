#pragma once
#include "Define.h"
class Block_Piece
{
private:
	string m_strColor;
	RECT m_rcBlock;
	POINT m_pt;
public:
	void Init(string color);
	void SetPoint(int x,int y);
	void Move(int x, int y);
	inline RECT GetRect()
	{
		return m_rcBlock;
	};
	bool Check(RECT rcBlock);
	void Draw();
	inline POINT GetPt()
	{
		return m_pt;
	};
	Block_Piece();
	~Block_Piece();
};

