#include "Block.h"



Block::Block()
{
	m_turn = 0;
}

void Block::Return()
{
	m_turn--;
	if (m_turn < BLOCK_TURN_0)
		m_turn = BLOCK_TURN_270;
	SetBlock();
}
Block::~Block()
{
}
