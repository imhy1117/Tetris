#include "Block_i.h"



Block_i::Block_i()
{
	m_turn = 0;
	m_iCount = 0;
}

void Block_i::Init()
{
	m_turn = BLOCK_TURN_0;
	for (int i = 0; i < 4; i++)
	{
		Block_Piece* pNew = new Block_Piece();
		pNew->Init(BLOCK_PINK);
		m_vecBlock.push_back(pNew);
		BlockManager::GetInstance()->AddBlock(pNew);
	}
	
}
void Block_i::Turn()
{
	m_turn++;
	if (m_turn > BLOCK_TURN_270)
		m_turn = BLOCK_TURN_0;
	SetBlock();
}
void Block_i::SetBlock()
{
	switch (m_turn)
	{
	case BLOCK_TURN_0:
		for (int i = 0; i < 4; i++)
		{
			m_vecBlock[i]->SetPoint(m_pt.x+(i*BLOCK_SIZE), m_pt.y+BLOCK_SIZE);
		}
		break;
	case BLOCK_TURN_90:
		for (int i = 0; i < 4; i++)
		{
			m_vecBlock[i]->SetPoint(m_pt.x , m_pt.y + BLOCK_SIZE - (i*BLOCK_SIZE));
		}
		break;
	case BLOCK_TURN_180:
		for (int i = 0; i < 4; i++)
		{
			m_vecBlock[i]->SetPoint(m_pt.x + (i*BLOCK_SIZE), m_pt.y + BLOCK_SIZE);
		}
		break;
	case BLOCK_TURN_270:
		for (int i = 0; i < 4; i++)
		{
			m_vecBlock[i]->SetPoint(m_pt.x, m_pt.y + BLOCK_SIZE - (i*BLOCK_SIZE));
		}
		break;
	}
}

void Block_i::SetPoint(int x, int y)
{
	m_pt.x = x;
	m_pt.y = y;
	SetBlock();
}
bool Block_i::CheakHit(RECT rcBlock)
{
	for (int i = 0; i < 4; i++)
	{
		if (m_vecBlock[i]->Check(rcBlock) == true)
			return true;
	}
	return false;
}
void Block_i::Move(int x, int y)
{
	m_pt.x += x;
	m_pt.y += y;
	SetBlock();
}
void Block_i::Draw()
{
	for (int i = 0; i < 4; i++)
	{
		m_vecBlock[i]->Draw();
	}
}
void Block_i::Stack()
{
	for (int i = 0; i < 4; i++)
	{
		m_vecBlock[i] = NULL;
	}
}
void Block_i::Release()
{
	for (int i = 0; i < 4; i++)
	{
		SAFE_DELETE(m_vecBlock[i]);
	}
	m_vecBlock.clear();
}
Block_Piece* Block_i::GetBlock()
{
	if (m_iCount <4)
	{
		m_iCount++;
		return m_vecBlock[m_iCount - 1];
	}
	else
		return NULL;
}
Block_i::~Block_i()
{
}
