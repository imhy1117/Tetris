#include "ResManager.h"
ResManager::ResManager()
{
}
void ResManager::Init(HWND hWnd, int size_cx, int size_cy)
{
	HDC hdc = GetDC(hWnd);
	m_backHdc = CreateCompatibleDC(hdc);
	m_hBitMap = CreateCompatibleBitmap(hdc, size_cx, size_cy);
	m_hOldBitMap = (HBITMAP)SelectObject(m_backHdc, m_hBitMap);
	ReleaseDC(hWnd, hdc);
	m_iBackSizeX=size_cx;
	m_iBackSizeY=size_cy;
}
void ResManager::Release()
{
	SelectObject(m_backHdc, m_hOldBitMap);
	DeleteObject(m_hBitMap);
	DeleteDC(m_backHdc);
	for (auto iter = m_mapBitMap.begin(); iter != m_mapBitMap.end(); iter++)
	{
		auto del = (*iter).second;
		del->Release();
		SAFE_DELETE(del);
	}
	m_mapBitMap.clear();
	DestroyInstance();
}
BitMap* ResManager::InitBitMap(string strFilename)
{
	auto iter = m_mapBitMap.find(strFilename);
	if (iter == m_mapBitMap.end())
	{
		BitMap* pBitMap = new BitMap();
		pBitMap->Init(m_backHdc, strFilename);
		m_mapBitMap.insert(make_pair(strFilename, pBitMap));
		return pBitMap;
	}
	else
	{
		return iter->second;
	}
}
BitMap* ResManager::GetBitMap(string strFilename)
{
	auto iter = m_mapBitMap.find(strFilename);
	if (iter != m_mapBitMap.end())
	{
		return iter->second;
	}
	else
	{
		return InitBitMap(strFilename);
	}
}
void ResManager::DrawBack()
{
	PatBlt(m_backHdc, 0, 0, m_iBackSizeX, m_iBackSizeY, WHITENESS);
}
void ResManager::Draw(HDC hdc)
{
	BitBlt(hdc, 0, 0, m_iBackSizeX, m_iBackSizeY, m_backHdc, 0, 0, SRCCOPY);
}
ResManager::~ResManager()
{
}