#include "Block_rs.h"



Block_rs::Block_rs()
{
	m_turn = 0;
	m_iCount = 0;
}
void Block_rs::Init()
{
	m_turn = BLOCK_TURN_0;
	for (int i = 0; i < 4; i++)
	{
		Block_Piece* pNew = new Block_Piece();
		pNew->Init(BLOCK_RED);
		m_vecBlock.push_back(pNew);
		BlockManager::GetInstance()->AddBlock(pNew);
	}

}
void Block_rs::Turn()
{
	m_turn++;
	if (m_turn > BLOCK_TURN_270)
		m_turn = BLOCK_TURN_0;
	SetBlock();
}
void Block_rs::SetBlock()
{
	switch (m_turn)
	{
	case BLOCK_TURN_0:
		m_vecBlock[0]->SetPoint(m_pt.x, m_pt.y);
		m_vecBlock[1]->SetPoint(m_pt.x + BLOCK_SIZE, m_pt.y);
		m_vecBlock[2]->SetPoint(m_pt.x + BLOCK_SIZE, m_pt.y + BLOCK_SIZE);
		m_vecBlock[3]->SetPoint(m_pt.x + BLOCK_SIZE*2, m_pt.y + BLOCK_SIZE);
		break;
	case BLOCK_TURN_90:
		m_vecBlock[0]->SetPoint(m_pt.x + BLOCK_SIZE, m_pt.y-BLOCK_SIZE);
		m_vecBlock[1]->SetPoint(m_pt.x + BLOCK_SIZE, m_pt.y );
		m_vecBlock[2]->SetPoint(m_pt.x, m_pt.y );
		m_vecBlock[3]->SetPoint(m_pt.x, m_pt.y + BLOCK_SIZE);
		break;
	case BLOCK_TURN_180:
		m_vecBlock[0]->SetPoint(m_pt.x, m_pt.y);
		m_vecBlock[1]->SetPoint(m_pt.x + BLOCK_SIZE, m_pt.y);
		m_vecBlock[2]->SetPoint(m_pt.x + BLOCK_SIZE, m_pt.y + BLOCK_SIZE);
		m_vecBlock[3]->SetPoint(m_pt.x + BLOCK_SIZE * 2, m_pt.y + BLOCK_SIZE);
		break;
	case BLOCK_TURN_270:
		m_vecBlock[0]->SetPoint(m_pt.x + BLOCK_SIZE, m_pt.y - BLOCK_SIZE);
		m_vecBlock[1]->SetPoint(m_pt.x + BLOCK_SIZE, m_pt.y);
		m_vecBlock[2]->SetPoint(m_pt.x, m_pt.y);
		m_vecBlock[3]->SetPoint(m_pt.x, m_pt.y + BLOCK_SIZE);
		break;
	}
}

void Block_rs::SetPoint(int x, int y)
{
	m_pt.x = x;
	m_pt.y = y;
	SetBlock();
}
bool Block_rs::CheakHit(RECT rcBlock)
{
	for (int i = 0; i < 4; i++)
	{
		if (m_vecBlock[i]->Check(rcBlock) == true)
			return true;
	}
	return false;
}
void Block_rs::Move(int x, int y)
{
	m_pt.x += x;
	m_pt.y += y;
	SetBlock();
}
void Block_rs::Draw()
{
	for (int i = 0; i < 4; i++)
	{
		m_vecBlock[i]->Draw();
	}
}
void Block_rs::Stack()
{
	for (int i = 0; i < 4; i++)
	{
		m_vecBlock[i] = NULL;
	}

}
void Block_rs::Release()
{
	for (int i = 0; i < 4; i++)
	{
		SAFE_DELETE(m_vecBlock[i]);
	}
	m_vecBlock.clear();
}
Block_Piece* Block_rs::GetBlock()
{
	if (m_iCount <4)
	{
		m_iCount++;
		return m_vecBlock[m_iCount - 1];
	}
	else
		return NULL;
}

Block_rs::~Block_rs()
{
}
