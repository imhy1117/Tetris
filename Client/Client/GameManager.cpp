#include "GameManager.h"
#include "InputManager.h"
#include "ResManager.h"
#include "TimeManager.h"
#include "DuelManager.h"
#include "NetManager.h"
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
GameManager::GameManager()
{
	
}

void GameManager::Init()
{
	m_iDeleteCount = 0;
	m_fDropTime = 0;
	m_fInputTime = 0;
	m_iBlockCount = 0;
	m_iBlockNum = 0;
	m_fWaitTime = 0;
	m_bCombo = false;
	gamestat = WAIT;
	m_sock = NetManager::GetInstance()->GetSocket();
	for (int i = 0; i < FIELD_HEIGHT; i++)
	{
		for (int j = 0; j < FIELD_WIDTH; j++)
		{
			Field* pNew = new Field();
			pNew->Init((j*BLOCK_SIZE) + GAMESCREEN_X, (i*BLOCK_SIZE) + GAMESCREEN_Y);
			m_vecField.push_back(pNew);
		}
	}
	for (int i = 0; i < 4; i++)
	{
		InputManager::GetInstance()->RegistKey(VK_LEFT + i);
	}
	InputManager::GetInstance()->RegistKey(VK_SPACE);
	
	SetRect(&m_rcWall[0], GAMESCREEN_X, GAMESCREEN_Y - 40, GAMESCREEN_X + GAMESCREEN_W, GAMESCREEN_Y);
	SetRect(&m_rcWall[1], GAMESCREEN_X-20, GAMESCREEN_Y-20, GAMESCREEN_X, GAMESCREEN_Y+GAMESCREEN_H);
	SetRect(&m_rcWall[2], GAMESCREEN_X+GAMESCREEN_W, GAMESCREEN_Y-20, GAMESCREEN_X + GAMESCREEN_W+20, GAMESCREEN_Y + GAMESCREEN_H);
	SetRect(&m_rcWall[3], GAMESCREEN_X, GAMESCREEN_Y+GAMESCREEN_H, GAMESCREEN_X + GAMESCREEN_W, GAMESCREEN_Y + GAMESCREEN_H + 20);
	PACKET_ROOM_IN roomin;
	roomin.header.name = PACKET_HEADER_ROOM_IN;
	roomin.header.len = sizeof(roomin);
	roomin.room_num = m_iRoomNum;
	send(m_sock, (char*)&roomin, roomin.header.len,0);
}
void GameManager::StartGame(int seed)
 {
	srand(seed);
	for (int i = 0; i < 4; i++)
	{
		GenBlock();
	}
	DropBlock();
	gamestat = RUN;
}
void GameManager::SetNext()
{
	for (int i = 0; i < 4; i++)
	{
		m_vecBlock[m_iBlockCount + i]->SetPoint(NEXTSCREEN_X, NEXTSCREEN_Y + (NEXTSCREEN_BLANK*i));
	}
}
void GameManager::PullLine(int line)
{
	for (int i = line; i > 1; i--)
	{
		for (int j = 0; j < FIELD_WIDTH; j++)
		{
			m_vecField[i * 10 + j]->SetBlock(m_vecField[(i - 1) * 10 + j]->GetBlock());
		}
	}
}
void GameManager::PushLine(int line)
{
	for (int i = 0; i <(FIELD_HEIGHT-1); i++)
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
		m_vecField[(FIELD_HEIGHT-1) * 10 + j]->AddBlock();
	}
}
void GameManager::DeleteLine(int line)
{
	for (int j = 0; j < FIELD_WIDTH; j++)
	{
		m_vecField[line * 10 + j]->Clear();
	}
	PullLine(line);
	Chaincheck();
}
void GameManager::Chaincheck()
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
		{
			DeleteLine(i);
			m_iDeleteCount++;
		}
	}
}
bool GameManager::CheckLine()
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
		{
			if (m_bCombo == true)
				m_iDeleteCount++;
			DeleteLine(i);
			return true;
		}	
	}
	return false;
}
void GameManager::GenBlock()
{
	
	Block* pNew;
	int index = rand() % 7;
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
	m_iBlockNum++;
	m_vecBlockIndex.push_back(index);
	pNew->Init();
	m_vecBlock.push_back(pNew);
}
void GameManager::DropBlock()
{
	m_pCurBlock = m_vecBlock[m_iBlockCount];
	m_pCurBlock->SetPoint(GAMESCREEN_X + 80, GAMESCREEN_Y - 20);
	m_iBlockCount++;
	while (1)
	{
		Block_Piece* pNew = m_pCurBlock->GetBlock();
		if (pNew == NULL)
			break;
		m_vecBlockPieces.push_back(pNew);
	}
	GenBlock();
	SetNext();
}
bool GameManager::CheckHit()
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
void GameManager::Fall()
{
	m_pCurBlock->Move(0, BLOCK_SIZE);
	if (CheckHit() == true)
	{
		m_pCurBlock->Move(0, -BLOCK_SIZE);
		Stack();
	}
}
bool GameManager::FastDrop()
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
void GameManager::Dead()
{
	gamestat = GAMEOVER;
	for (auto iter = m_vecField.begin(); iter != m_vecField.end(); iter++)
	{
		(*iter)->SetGray();
	}
	PACKET_GAMEOVER gameover;
	gameover.header.name = PACKET_HEADER_GAMEOVER;
	gameover.header.len = sizeof(gameover);
	send(m_sock, (char*)&gameover, gameover.header.len, 0);
}
void GameManager::Stack()
{
	if (m_pCurBlock->CheakHit(m_rcWall[0]) == true)
	{
		if(gamestat==RUN)
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
	m_pCurBlock->Stack();
	m_vecBlockPieces.clear();
	if (CheckLine() == true)
	{
		m_bCombo = true;
		PACKET_ATTACK packet;
		packet.header.name = PACKET_HEADER_ATTACK;
		packet.header.len = sizeof(packet);
		packet.line = m_iDeleteCount;
		send(m_sock, (char*)&packet, packet.header.len, 0);
		m_iDeleteCount = 0;
	}
	else 
	{
		m_bCombo = false;
	}
	DropBlock();
}

void GameManager::SendInput(INPUT_INDEX input)
{
	int retval;
	PACKET_INPUT packet;
	packet.header.name = PACKET_HEADER_INPUT;
	packet.header.len = sizeof(packet);
	packet.input = input;
	retval= send(m_sock, (char*)&packet, sizeof(packet), 0);
	if (retval == SOCKET_ERROR)
	{
		NetManager::GetInstance()->err_display("send()");
	}
}
void GameManager::Input()
{
	if (gamestat == RUN)
	{
		m_fInputTime += TimeManager::GetInstance()->GetElapseTime();
		if (InputManager::GetInstance()->CheckKeyUp(VK_UP))
		{
			SendInput(UP);
			m_pCurBlock->Turn();
			for (auto iter = m_vecField.begin(); iter != m_vecField.end(); iter++)
			{
				if (m_pCurBlock->CheakHit((*iter)->GetRect()) == true)
					m_pCurBlock->Return();
			}
			while (1)
			{
				if (m_pCurBlock->CheakHit(m_rcWall[1]) == true)
					m_pCurBlock->Move(BLOCK_SIZE, 0);
				else if (m_pCurBlock->CheakHit(m_rcWall[2]) == true)
					m_pCurBlock->Move(-BLOCK_SIZE, 0);
				else
					break;
			}
		}
		else if (InputManager::GetInstance()->CheckKeyUp(VK_SPACE))
		{
			SendInput(SPACE);
			m_fInputTime = 0;
			while (1)
			{
				if (FastDrop())
					break;
			}
		}

		else if (InputManager::GetInstance()->CheckKeyUp(VK_LEFT))
		{
			m_pCurBlock->Move(-BLOCK_SIZE, 0);
			SendInput(LEFT);
			if (CheckHit() == true)
				m_pCurBlock->Move(BLOCK_SIZE, 0);

		}
		else if (InputManager::GetInstance()->CheckKeyUp(VK_RIGHT))
		{
			m_pCurBlock->Move(BLOCK_SIZE, 0);
			SendInput(RIGHT);
			if (CheckHit() == true)
				m_pCurBlock->Move(-BLOCK_SIZE, 0);

		}
		else if (InputManager::GetInstance()->CheckKeyUp(VK_DOWN))
		{
			SendInput(DOWN);
			Fall();
		}
		m_fInputTime = 0;
	}
}

void GameManager::Update()
{
	float ElapseTime = TimeManager::GetInstance()->GetElapseTime();
	if (gamestat == RUN)
	{
		m_fDropTime += ElapseTime;
		if (m_fDropTime > 1)
		{
			SendInput(DOWN);
			Fall();
			m_fDropTime = 0;
		}
	}
}
void GameManager::Draw()
{
	ResManager::GetInstance()->GetBitMap(DIR_FIELD)->DrawBitblt(GAMESCREEN_X, GAMESCREEN_Y);
	for (auto iter = m_vecField.begin(); iter != m_vecField.end(); iter++)
	{
		(*iter)->Draw();
	}
	if (gamestat == RUN)
	{
		m_pCurBlock->Draw();
		ResManager::GetInstance()->GetBitMap(DIR_NEXT)->Draw(NEXTSCREEN_X - 5, NEXTSCREEN_Y - 50);
		for (int i = 0; i < 4; i++)
		{
			m_vecBlock[m_iBlockCount + i]->Draw();
		}
	}
}
void GameManager::Reset()
{
	for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	m_vecBlock.clear();
	m_vecBlockPieces.clear();
	m_vecBlockIndex.clear();
	for (auto iter = m_vecField.begin(); iter != m_vecField.end(); iter++)
	{
		(*iter)->Clear();
	}
	m_fDropTime = 0;
	m_iBlockCount = 0;
	m_iBlockNum = 0;
	m_fWaitTime = 0;
	gamestat = WAIT;
	m_bCombo = false;
}
void GameManager::Release()
{
	for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	m_vecBlock.clear();
	for (auto iter = m_vecField.begin(); iter != m_vecField.end(); iter++)
	{
		(*iter)->Clear();
		SAFE_DELETE(*iter);
	}
	m_vecField.clear();
	m_vecBlockPieces.clear();
	DestroyInstance();
}
GameManager::~GameManager()
{
}
