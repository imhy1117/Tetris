#pragma once
#include "EngineDefine.h"
#include <string>
using namespace std;
class BitMap
{
private:
	HDC m_hMemDC;
	HBITMAP m_hOldBitMap;
	HBITMAP m_hBitMap;
	SIZE m_size;
	ANCHOR m_anchor;
public:
	void Init(HDC hdc, string filename);
	void Release();
	inline void SetAnchor(ANCHOR anchor)
	{
		m_anchor = anchor;
	}
	void Draw(int x, int y );
	void ApplyAnchor(int& x, int& y);
	void DrawBitblt(int x, int y);
	inline SIZE GetSize()
	{
		return m_size;
	}
	BitMap();
	~BitMap();
};

