#pragma once
#include "SingleTon.h"
#include <vector>
class Block_Piece;
using namespace std;
class BlockManager:public SingleTon<BlockManager>
{
private:
	vector<Block_Piece*>m_vecBlock;
public:
	BlockManager();
	void AddBlock(Block_Piece* block);
	void Release();
	~BlockManager();
};

