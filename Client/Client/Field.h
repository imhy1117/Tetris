#pragma once
#include "Block_Piece.h"
class Field
{
private:
	Block_Piece* m_pBlock;
	POINT m_pt;
	RECT m_rc;
public:
	Field();
	void Init(int x, int y);
	void SetBlock(Block_Piece* block);
	inline RECT GetRect()
	{
		if (m_pBlock != NULL)
			return m_pBlock->GetRect();
		else
			return m_rc;
	}
	inline bool Check()
	{
		if (m_pBlock != NULL)
			return true;
		else
			return false;
	}
	void SetGray();
	inline Block_Piece* GetBlock()
	{
		return m_pBlock;
	}
	inline void Clear()
	{
		m_pBlock = NULL;
	}
	void AddBlock();
	bool StackBlock(Block_Piece* block);
	void Draw();
	~Field();
};

