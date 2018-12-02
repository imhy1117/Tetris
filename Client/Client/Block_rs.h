#pragma once
#include "Block.h"
class Block_rs :
	public Block
{
private:
	vector<Block_Piece*> m_vecBlock;
	POINT m_pt;
	int m_iCount;
public:
	void Init();
	void SetBlock();
	void SetPoint(int x, int y);
	void Move(int x, int y);
	bool CheakHit(RECT rcBlock);
	void Draw();
	void Release();
	void Turn();
	void Stack();
	Block_Piece* GetBlock();
	Block_rs();
	~Block_rs();
};

