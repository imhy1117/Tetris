#include "Block_square.h"



Block_square::Block_square()
{
	m_turn = 0;
	m_iCount = 0;
}

void Block_square::Init()
{
	m_turn = BLOCK_TURN_0;
	for (int i = 0; i < 4; i++)
	{
		Block_Piece* pNew = new Block_Piece();
		pNew->Init(BLOCK_YELLOW);
		m_vecBlock.push_back(pNew);
		BlockManager::GetInstance()->AddBlock(pNew);
	}
	
}
void Block_square::Turn()
{
	m_turn = BLOCK_TURN_0;
	SetBlock();
}
void Block_square::SetBlock()
{
	m_vecBlock[0]->SetPoint(m_pt.x + BLOCK_SIZE, m_pt.y);
	m_vecBlock[1]->SetPoint(m_pt.x + BLOCK_SIZE*2, m_pt.y);
	m_vecBlock[2]->SetPoint(m_pt.x + BLOCK_SIZE, m_pt.y + BLOCK_SIZE);
	m_vecBlock[3]->SetPoint(m_pt.x + BLOCK_SIZE*2, m_pt.y + BLOCK_SIZE);
}

void Block_square::SetPoint(int x, int y)
{
	m_pt.x = x;
	m_pt.y = y;
	SetBlock();
}
bool Block_square::CheakHit(RECT rcBlock)
{
	for (int i = 0; i < 4; i++)
	{
		if (m_vecBlock[i]->Check(rcBlock) == true)
			return true;
	}
	return false;
}
void Block_square::Move(int x, int y)
{
	m_pt.x += x;
	m_pt.y += y;
	SetBlock();
}
void Block_square::Draw()
{
	for (int i = 0; i < 4; i++)
	{
		m_vecBlock[i]->Draw();
	}
}
void Block_square::Stack()
{
	for (int i = 0; i < 4; i++)
	{
		m_vecBlock[i] = NULL;
	}
	
}
void Block_square::Release()
{
	for (int i = 0; i < 4; i++)
	{
		SAFE_DELETE(m_vecBlock[i]);
	}
	m_vecBlock.clear();
}
Block_Piece* Block_square::GetBlock()
{
	if (m_iCount <4)
	{
		m_iCount++;
		return m_vecBlock[m_iCount - 1];
	}
	else
		return NULL;
}
Block_square::~Block_square()
{
}
