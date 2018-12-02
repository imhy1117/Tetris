#pragma once
#include <vector>
#include "SingleTon.h"
#include "Define.h"
#include "..\\..\\Com\Packet.h"
using namespace std;
class Field;
class Block;
class Block_Piece;
class GameManager:public SingleTon<GameManager>
{
private:
	float m_fDropTime,m_fInputTime,m_fWaitTime;
	bool m_bCombo;
	int m_iBlockCount,m_iBlockNum, m_iDeleteCount,m_iRoomNum;
	vector<Field*> m_vecField;
	vector<Block_Piece*> m_vecBlockPieces;
	vector<Block*>m_vecBlock;
	vector<int>m_vecBlockIndex;
	GAME_STAT gamestat;
	Block* m_pCurBlock;
	RECT m_rcWall[4];
	SOCKET m_sock;
public:
	
	void Init();
	inline void SetRoomNum(int index)
	{
		m_iRoomNum = index;
	}
	inline int GetBlockIndex(int index)
	{
		if (m_iBlockNum <= index)
			GenBlock();
		return m_vecBlockIndex[index];
	};
	void GenBlock();
	void DropBlock();
	void Fall();
	void Stack();
	void SetNext();
	bool CheckHit();
	void Dead();
	inline void GameOver()
	{
		gamestat = GAMEOVER;
	};
	void SendInput(INPUT_INDEX input);
	void Input();
	void Draw();
	bool CheckLine();
	void Chaincheck();
	void DeleteLine(int line);
	void PushLine(int line);
	void PullLine(int line);
	bool FastDrop();
	void Update();
	void Release();
	void StartGame(int seed);
	void Reset();
	GameManager();
	~GameManager();
};

