#include "Button.h"
#include "ResManager.h"


Button::Button()
{
}
void Button::Init(int x, int y, string strFilename)
{
	m_pt.x = x;
	m_pt.y = y;
	m_strButton = strFilename;
	SIZE size=ResManager::GetInstance()->GetBitMap(strFilename)->GetSize();
	SetRect(&m_rcButton, x, y, x + size.cx, y + size.cy);
}
void Button::Draw()
{
	ResManager::GetInstance()->GetBitMap(m_strButton)->Draw(m_pt.x, m_pt.y);
}
bool Button:: CheckClick(POINT pt)
{
	if (PtInRect(&m_rcButton, pt) == TRUE)
		return true;
	else
	return false;
}
Button::~Button()
{
}
