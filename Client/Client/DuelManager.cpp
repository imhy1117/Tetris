#include "DuelManager.h"
#include "InputManager.h"
#include "ResManager.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "Scene_game.h"
#include "Block.h"
#include "Block_s.h"
#include "Block_rs.h"
#include "Block_i.h"
#include "Block_l.h"
#include "Block_rl.h"
#include "Block_t.h"
#include "Block_square.h"
#include "Field.h"
#include "GameManager.h"

DuelManager::DuelManager()
{
}
void DuelManager::Init(int x)
{
	m_fDropTime = 0;
	m_iBlockCount = 0;
	m_iScreenX = x;
	m_fWaitTime = 0;
	gamestat = WAIT;
	m_bReady = false;
	m_bUsing =false;
	for (int i = 0; i < FIELD_HEIGHT; i++)
	{
		for (int j = 0; j < FIELD_WIDTH; j++)
		{
			Field* pNew = new Field();
			pNew->Init((j*BLOCK_SIZE) + m_iScreenX, (i*BLOCK_SIZE) + GAMESCREEN_Y);
			m_vecField.push_back(pNew);
		}
	}
	wsprintf(m_chUserName, "연결없음");
	SetRect(&m_rcWall[0], m_iScreenX, GAMESCREEN_Y - 40, m_iScreenX + GAMESCREEN_W, GAMESCREEN_Y );
	SetRect(&m_rcWall[1], m_iScreenX - 20, GAMESCREEN_Y-20, m_iScreenX, GAMESCREEN_Y + GAMESCREEN_H);
	SetRect(&m_rcWall[2], m_iScreenX + GAMESCREEN_W, GAMESCREEN_Y-20, m_iScreenX + GAMESCREEN_W + 20, GAMESCREEN_Y + GAMESCREEN_H);
	SetRect(&m_rcWall[3], m_iScreenX, GAMESCREEN_Y + GAMESCREEN_H, m_iScreenX + GAMESCREEN_W, GAMESCREEN_Y + GAMESCREEN_H + 20);
	
}
void DuelManager::StartGame()
{
	GenBlock();
	DropBlock();
	m_bReady = false;
	gamestat = RUN;
}

void DuelManager::PullLine(int line)
{
	for (int i = line; i > 1; i--)
	{
		for (int j = 0; j < FIELD_WIDTH; j++)
		{
			m_vecField[i * 10 + j]->SetBlock(m_vecField[(i - 1) * 10 + j]->GetBlock());
		}
	}
}
void DuelManager::PushLine(int line)
{
	for (int i = 0; i <(FIELD_HEIGHT - 1); i++)
	{
		for (int j = 0; j < FIELD_WIDTH; j++)
		{
			m_vecField[i * 10 + j]->SetBlock(m_vecField[(i + 1) * 10 + j]->GetBlock());
		}
	}
	for (int i = 0; i < FIELD_WIDTH; i++)
	{
		m_vecField[(FIELD_HEIGHT - 1) * 10 + i]->Clear();
	}
	for (int j = 0; j < FIELD_WIDTH; j++)
	{
		if (j == line)
			continue;
		m_vecField[(FIELD_HEIGHT - 1) * 10 + j]->AddBlock();
	}
}
void DuelManager::DeleteLine(int line)
{
	for (int j = 0; j < FIELD_WIDTH; j++)
	{
		m_vecField[line * 10 + j]->Clear();
	}
	PullLine(line);
	CheckLine();
}
void DuelManager::CheckLine()
{
	for (int i = 0; i < FIELD_HEIGHT; i++)
	{
		bool b_delete = true;
		for (int j = 0; j < FIELD_WIDTH; j++)
		{
			if (m_vecField[i * 10 + j]->Check() == false)
			{
				b_delete = false;
				break;
			}
		}
		if (b_delete)
			DeleteLine(i);
	}
}
void DuelManager::GenBlock()
{
	
	Block* pNew;
	int index = GameManager::GetInstance()->GetBlockIndex(m_iBlockCount);
	switch (index)
	{
	case 0:
		pNew = new Block_s();
		break;
	case 1:
		pNew = new Block_rs();
		break;
	case 2:
		pNew = new Block_l();
		break;
	case 3:
		pNew = new Block_rl();
		break;
	case 4:
		pNew = new Block_t();
		break;
	case 5:
		pNew = new Block_i();
		break;
	case 6:
		pNew = new Block_square();
		break;
	}
	pNew->Init();
	m_vecBlock.push_back(pNew);
}
void DuelManager::DropBlock()
{
	m_pCurBlock = m_vecBlock[m_iBlockCount];
	m_pCurBlock->SetPoint(m_iScreenX + 80, GAMESCREEN_Y - 20);
	m_iBlockCount++;
	while (1)
	{
		Block_Piece* pNew = m_pCurBlock->GetBlock();
		if (pNew == NULL)
			break;
		m_vecBlockPieces.push_back(pNew);
	}
	GenBlock();
}
bool DuelManager::CheckHit()
{
	for (auto iter = m_vecField.begin(); iter != m_vecField.end(); iter++)
	{
		if (m_pCurBlock->CheakHit((*iter)->GetRect()) == true)
			return true;
	}
	for (int i = 1; i < 4; i++)
	{
		if (m_pCurBlock->CheakHit(m_rcWall[i]) == true)
			return true;
	}
	return false;
}
void DuelManager::Fall()
{
	m_pCurBlock->Move(0, BLOCK_SIZE);
	if (CheckHit() == true)
	{
		m_pCurBlock->Move(0, -BLOCK_SIZE);
		Stack();
	}
}
bool DuelManager::FastDrop()
{
	m_pCurBlock->Move(0, BLOCK_SIZE);
	if (CheckHit() == true)
	{
		m_pCurBlock->Move(0, -BLOCK_SIZE);
		Stack();
		return true;
	}
	return false;
}
void DuelManager::Dead()
{
	gamestat = GAMEOVER;
	for (auto iter = m_vecField.begin(); iter != m_vecField.end(); iter++)
	{
		(*iter)->SetGray();
	}
}
void DuelManager::Stack()
{
	if (m_pCurBlock->CheakHit(m_rcWall[0]) == true)
	{
		Dead();
		return;
	}
	for (auto iter1 = m_vecBlockPieces.begin(); iter1 != m_vecBlockPieces.end(); iter1++)
	{
		for (auto iter2 = m_vecField.begin(); iter2 != m_vecField.end(); iter2++)
		{
			if ((*iter2)->StackBlock(*iter1) == true)
				break;
		}
	}
	m_vecBlockPieces.clear();
	CheckLine();
	DropBlock();
}
void DuelManager::Input(WORD input)
{
	if (gamestat == RUN)
	{
		if (input==UP)
		{
			m_pCurBlock->Turn();
			while (1)
			{
				if (m_pCurBlock->CheakHit(m_rcWall[1]) == true)
					m_pCurBlock->Move(BLOCK_SIZE, 0);
				else if (m_pCurBlock->CheakHit(m_rcWall[2]) == true)
					m_pCurBlock->Move(-BLOCK_SIZE, 0);
				else
					break;
			}
			return;
		}
	
			if (input == LEFT)
			{
				m_pCurBlock->Move(-BLOCK_SIZE, 0);
				if (CheckHit() == true)
					m_pCurBlock->Move(BLOCK_SIZE, 0);
				return;
			}
			else if (input == RIGHT)
			{
				m_pCurBlock->Move(BLOCK_SIZE, 0);
				if (CheckHit() == true)
					m_pCurBlock->Move(-BLOCK_SIZE, 0);
				return;
			}
			if (input == DOWN)
			{
				Fall();
				return;
			}
		
		
		if (input == SPACE)
		{
			while (1)
			{
				if (FastDrop())
					break;
			}
		}
	}
}
void DuelManager::Update()
{
	float ElapseTime = TimeManager::GetInstance()->GetElapseTime();
	if (gamestat == RUN)
	{
		m_fDropTime += ElapseTime;
		if (m_fDropTime > 1)
		{
			Fall();
			m_fDropTime = 0;
		}
	}
}
void DuelManager::Draw()
{
	ResManager::GetInstance()->GetBitMap(DIR_FIELD)->DrawBitblt(m_iScreenX, GAMESCREEN_Y);
	
	for (auto iter = m_vecField.begin(); iter != m_vecField.end(); iter++)
	{
		(*iter)->Draw();
	}
	if (gamestat == RUN)
	{
		m_pCurBlock->Draw();
	}
	if (m_bReady == true)
		ResManager::GetInstance()->GetBitMap(DIR_READY_STAT)->Draw(m_iScreenX + 100, GAMESCREEN_Y + 200);
	
}
void DuelManager::DrawName()
{
	TextOut(ResManager::GetInstance()->GetBackDC(), m_iScreenX + 45, GAMESCREEN_Y - 50, m_chUserName, lstrlen(m_chUserName));
};
void DuelManager::Reset()
{
	for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	m_vecBlock.clear();
	m_vecBlockPieces.clear();
	for (auto iter = m_vecField.begin(); iter != m_vecField.end(); iter++)
	{
		(*iter)->Clear();
	}
	m_fDropTime = 0;
	m_iBlockCount = 0;
	m_fWaitTime = 0;
	gamestat = WAIT;
}
void DuelManager::Release()
{
	for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	for (auto iter = m_vecField.begin(); iter != m_vecField.end(); iter++)
	{
		(*iter)->Clear();
		SAFE_DELETE(*iter);
	}
	m_vecField.clear();
	m_vecBlockPieces.clear();
}

DuelManager::~DuelManager()
{
}
