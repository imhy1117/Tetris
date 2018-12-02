#pragma once
#include "SingleTon.h"
#include <vector>
#include "SingleTon.h"
#include "Define.h"
#include <string>
using namespace std;
class Field;
class Block;
class Block_Piece;
class DuelManager
{
private:
	float m_fDropTime, m_fWaitTime ;
	bool m_bReady, m_bUsing;
	int m_iScreenX;
	unsigned int m_iBlockCount;
	vector<Field*> m_vecField;
	vector<Block_Piece*> m_vecBlockPieces;
	vector<Block*>m_vecBlock;
	TCHAR m_chUserName[128];
	Block* m_pCurBlock;
	GAME_STAT gamestat;
	RECT m_rcWall[4];
public:
	void Init(int x);
	inline void SetUserName(char* username)
	{
		wsprintf(m_chUserName, "%s", username);
	};
	void GenBlock();
	void DropBlock();
	void Fall();
	void Stack();
	inline void Ready(BOOL ready)
	{
		if (ready == TRUE)
			m_bReady = true;
		else if (ready == FALSE)
			m_bReady=false;
	};
	inline void Connect()
	{
		m_bUsing = true;
	};
	inline void Disconnect()
	{
		m_bUsing = false;
		wsprintf(m_chUserName, "¿¬°á²÷±è");
	};
	inline bool CheckUsing()
	{
		return m_bUsing;
	};
	inline void GameOver()
	{
		gamestat = GAMEOVER;
	};
	bool CheckHit();
	void Input(WORD input);
	void Draw();
	void DrawName();
	void CheckLine();
	void DeleteLine(int line);
	void PullLine(int line);
	void PushLine(int line);
	void Dead();
	bool FastDrop();
	void Update();
	void Release();
	void StartGame();
	void Reset();
public:
	DuelManager();
	~DuelManager();
};

