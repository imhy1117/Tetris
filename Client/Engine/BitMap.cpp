#include "BitMap.h"
#include "ResManager.h"


BitMap::BitMap()
{
	m_anchor = ANCHOR_NORMAL;
}

void BitMap::Init(HDC hdc, string filename)
{
	m_hMemDC = CreateCompatibleDC(hdc);

	m_hBitMap = (HBITMAP)LoadImage(NULL, filename.c_str(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);

	m_hOldBitMap = (HBITMAP)SelectObject(m_hMemDC, m_hBitMap);
	BITMAP bit;
	GetObject(m_hBitMap, sizeof(BITMAP), &bit);
	m_size.cx = (int)bit.bmWidth;
	m_size.cy = (int)bit.bmHeight;
}
void  BitMap::Release()
{
	SelectObject(m_hMemDC, m_hOldBitMap);
	DeleteObject(m_hBitMap);
	DeleteDC(m_hMemDC);
}
void BitMap::ApplyAnchor(int& x, int& y)
{
	switch (m_anchor)
	{
	case ANCHOR_NORMAL:
		break;
	case ANCHOR_CENTER:
		x -=(int)(m_size.cx*0.5f);
		y -=(int)(m_size.cy*0.5f);
		break;
	}
}
void BitMap::Draw(int x, int y)
{
	ApplyAnchor(x, y);
	TransparentBlt(ResManager::GetInstance()->GetBackDC(), x, y, m_size.cx, m_size.cy, m_hMemDC, 0, 0, m_size.cx, m_size.cy, RGB(255, 0, 255));
}
void BitMap::DrawBitblt(int x, int y)
{
	ApplyAnchor(x, y);
	BitBlt(ResManager::GetInstance()->GetBackDC(), x, y, m_size.cx, m_size.cy, m_hMemDC, 0, 0, SRCCOPY);
}
BitMap::~BitMap()
{
}
