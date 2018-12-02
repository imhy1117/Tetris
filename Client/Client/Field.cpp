#include "Field.h"
#include "BlockManager.h"


Field::Field()
{
}

void Field::Init(int x, int y)
{
	m_pt.x = x;
	m_pt.y = y;
	m_pBlock = NULL;
	SetRect(&m_rc, 0, 0, 0, 0);
}
void Field::SetBlock(Block_Piece* block)
{
	m_pBlock = block;
	if(m_pBlock!=NULL)
	block->SetPoint(m_pt.x,m_pt.y);
}
void Field:: Draw()
 {
	if(m_pBlock!=NULL)
	m_pBlock->Draw();
	
}
void Field::SetGray()
{
	if (m_pBlock != NULL)
		m_pBlock->Init(BLOCK_GRAY);
}
bool Field::StackBlock(Block_Piece* block)
{
	POINT tmp = block->GetPt();
	if (m_pBlock == NULL)
	{
		if (tmp.x == m_pt.x&&tmp.y == m_pt.y)
		{
			m_pBlock = block;
			m_pBlock->SetPoint(m_pt.x, m_pt.y);
			return true;
		}
	}
	return false;
}
void Field::AddBlock()
{
	m_pBlock = new Block_Piece();
	m_pBlock->Init(BLOCK_GRAY);
	m_pBlock->SetPoint(m_pt.x, m_pt.y);
	BlockManager::GetInstance()->AddBlock(m_pBlock);
}

Field::~Field()
{
}
