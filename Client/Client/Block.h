#pragma once
#include "Block_Piece.h"
#include "Define.h"
#include <vector>
#include "BlockManager.h"
using namespace std;
class Block
{
protected:
	short m_turn;
public:
	Block();
	virtual void Init()=0;
	virtual void SetBlock()=0;
	virtual void SetPoint(int x, int y) = 0;
	virtual void Move(int x, int y)=0;
	virtual void Draw()=0;
	virtual bool CheakHit(RECT rcBlock) = 0;
	virtual void Release()=0;
	virtual void Turn()=0;
	virtual void Return();
	virtual void Stack() = 0;
	virtual Block_Piece* GetBlock() = 0;
	virtual ~Block();
};

