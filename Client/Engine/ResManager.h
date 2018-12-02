#pragma once
#include "EngineDefine.h"
#include "BitMap.h"
#include <string>
#include <map>
class ResManager:public SingleTon<ResManager>
{
private:
	map<string, BitMap*> m_mapBitMap;
	HDC m_backHdc;
	HBITMAP m_hOldBitMap, m_hBitMap;
	int m_iBackSizeX, m_iBackSizeY;
	BitMap* InitBitMap(string strFilename);
public:
	void Init(HWND hWnd,int size_cx,int size_cy);
	void Release();
	BitMap* GetBitMap(string strFilename);
	inline HDC GetBackDC()
	{
		return m_backHdc;
	}
	
	void DrawBack();
	void Draw(HDC hdc);
	ResManager();
	~ResManager();
};
