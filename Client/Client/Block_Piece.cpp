#include "Block_Piece.h"
#include "ResManager.h"


Block_Piece::Block_Piece()
{
}

void Block_Piece::Init(string color)
{
	m_strColor = color;
}
void Block_Piece::SetPoint(int x, int y)
{
	m_pt.x = x;
	m_pt.y = y;
	SetRect(&m_rcBlock, m_pt.x, m_pt.y, m_pt.x + BLOCK_SIZE, m_pt.y + BLOCK_SIZE);
}
void Block_Piece::Move(int x, int y)
{
	m_pt.x += x;
	m_pt.y += y;
	SetRect(&m_rcBlock, m_pt.x, m_pt.y, m_pt.x + BLOCK_SIZE, m_pt.y + BLOCK_SIZE);
}
bool Block_Piece::Check(RECT rcBlock)
{
	RECT tmp;
	if (IntersectRect(&tmp, &rcBlock, &m_rcBlock) == TRUE)
		return true;
	else
		return false;
}
void Block_Piece::Draw()
{
	ResManager::GetInstance()->GetBitMap(m_strColor)->DrawBitblt(m_pt.x,m_pt.y);
}
Block_Piece::~Block_Piece()
{
}
