#include "Guage.h"
#include "ResManager.h"

Guage::Guage()
{
}
void Guage::Init(int x, int y,int cx, int cy, int r, int g, int b)
{
	m_pt.x = x;
	m_pt.y = y;
	m_iWidth = cx;
	m_iHeight = cy;
	m_iPercent = 0;
	m_hCurBrush = CreateSolidBrush(RGB(r, g, b));
	m_hOldBrush = (HBRUSH)SelectObject(ResManager::GetInstance()->GetBackDC(), m_hCurBrush);
}
void Guage::Update(int value)
{
	m_iPercent = value;
	float tmp = (float)m_iWidth / 100 * m_iPercent;
	SetRect(&m_rc, m_pt.x, m_pt.y, (int)(m_pt.x+tmp), m_pt.y+ m_iHeight);
}
void Guage::Draw()
{
	FillRect(ResManager::GetInstance()->GetBackDC(), &m_rc, m_hCurBrush);
}
void Guage::Release()
{
	SelectObject(ResManager::GetInstance()->GetBackDC(), m_hOldBrush);
	DeleteObject(m_hCurBrush);
}
Guage::~Guage()
{
}
