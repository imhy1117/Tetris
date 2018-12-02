#include "BlockManager.h"
#include "Block_Piece.h"


BlockManager::BlockManager()
{
}
void BlockManager::AddBlock(Block_Piece* block)
{
	m_vecBlock.push_back(block);
}
void BlockManager::Release()
{
	for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
	{
		delete *iter;
	}
	m_vecBlock.clear();
	DestroyInstance();
}

BlockManager::~BlockManager()
{
}
