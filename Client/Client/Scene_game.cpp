#include "Scene_game.h"
#include "GameManager.h"
#include "DuelManager.h"
#include "TimeManager.h"
#include "ResManager.h"
#include "BlockManager.h"
#include "NetManager.h"
#include "SceneManager.h"
Scene_game::Scene_game()
{
}
void Scene_game::Init()
{
	len = 0;
	retval = 0;
	gamestat =WAIT;
	m_fGameoverTime = 0;
	readyButton.Init(50, 550, "Res\\Ready.bmp");
	cancleButton.Init(50, 550, "Res\\Cancle.bmp");
	outButton.Init(1000, 550,"Res\\Out.bmp");
	m_sock = NetManager::GetInstance()->GetSocket();
	ResManager::GetInstance()->GetBitMap(DIR_READY_STAT)->SetAnchor(ANCHOR_CENTER);
	m_hFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("¸¼Àº °íµñ"));
	m_hOldFont = (HFONT)SelectObject(ResManager::GetInstance()->GetBackDC(), m_hFont);
	for (int i = 0; i < 3; i++)
	{
		DuelManager* pNew = new DuelManager();
		pNew->Init(450 + (290 * i));
		m_vecDuelManager.push_back(pNew);
	}
	GameManager::GetInstance()->Init();
}
void Scene_game::Input()
{
	if (gamestat != GAMEOVER)
	GameManager::GetInstance()->Input();
}
void Scene_game::Update()
{
	if (gamestat != GAMEOVER)
	{
		Input();
		GameManager::GetInstance()->Update();
	
	}
	if (gamestat == GAMEOVER)
	{
		m_fGameoverTime += TimeManager::GetInstance()->GetElapseTime();
		if (m_fGameoverTime > 3)
		{
			m_fGameoverTime = 0;
			StopScene();
		}
	}
	
}
void Scene_game::Draw()
{
	GameManager::GetInstance()->Draw();
	for (auto iter = m_vecDuelManager.begin(); iter != m_vecDuelManager.end(); iter++)
	{
		(*iter)->Draw();
	}
	ResManager::GetInstance()->GetBitMap("Res\\Background.bmp")->Draw(0, 0);
	for (auto iter = m_vecDuelManager.begin(); iter != m_vecDuelManager.end(); iter++)
	{
		(*iter)->DrawName();
	}
	if (gamestat == WAIT)
	{
		readyButton.Draw();
		outButton.Draw();
	}
	else if (gamestat == READY)
	{
		ResManager::GetInstance()->GetBitMap(DIR_READY_STAT)->Draw(GAMESCREEN_X + 100, GAMESCREEN_Y + 200);
		cancleButton.Draw();
	}
}
void Scene_game::SendtoServer()
{

}
void Scene_game::Release()
{
	SelectObject(ResManager::GetInstance()->GetBackDC(), m_hOldFont);
	DeleteObject(m_hFont);
	GameManager::GetInstance()->Release();
	for (auto iter = m_vecDuelManager.begin(); iter != m_vecDuelManager.end(); iter++)
	{
		(*iter)->Release();
		SAFE_DELETE(*iter);
	}
	for (auto iter = m_mapUser.begin(); iter != m_mapUser.end(); iter++)
	{
		SAFE_DELETE(iter->second);
	}
	m_vecDuelManager.clear();
	m_mapUser.clear();
	BlockManager::GetInstance()->Release();
}
void Scene_game::Click(POINT pt)
{
	PACKET_READY packet;
	packet.header.name = PACKET_HEADER_READY;
	packet.header.len = sizeof(packet);
	if (gamestat == WAIT)
	{
		if (readyButton.CheckClick(pt) == true)
		{
			packet.ready_info = TRUE;
			send(m_sock, (char*)&packet, sizeof(packet), 0);
			gamestat = READY;
		}
		if (outButton.CheckClick(pt) == true)
		{
			PACKET_ROOM_OUT roomout;
			roomout.header.name = PACKET_HEADER_ROOM_OUT;
			roomout.header.len = sizeof(roomout);
			send(m_sock, (char*)&roomout, sizeof(roomout), 0);
			SceneManager::GetInstance()->LoadScene(SCENE_LOBBY);
			return;
		}
	}
	else if (gamestat == READY)
	{
		if (cancleButton.CheckClick(pt) == true )
		{
			packet.ready_info = FALSE;
			send(m_sock, (char*)&packet, sizeof(packet), 0);
			gamestat = WAIT;
		}
	}
	
}
bool Scene_game::ProcessPacket(WPARAM wParam)
{
	char buf[BUFSIZE + 1];
	retval = recv(wParam, buf, BUFSIZE, 0);
	if (retval == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAEWOULDBLOCK)
		{
			return true;
		}
		else
		{
			NetManager::GetInstance()->err_display("recv()");
			return true;
		}
	}
	memcpy(&m_buf[len], buf, retval);
	memcpy(&m_packet, m_buf, sizeof(m_packet));	
	len += retval;
	if (retval > m_packet.len)
	{
		len = retval;
	}
	if (len < m_packet.len)
	{
		memcpy(m_buf, buf, retval);
		return false;
	}
	else
	{
		while (1)
		{
			switch (m_packet.name)
			{
			case PACKET_HEADER_STARTGAME:
			{
				PACKET_STARTGAME packet;
				memcpy(&packet, m_buf, sizeof(packet));
				GameManager::GetInstance()->StartGame(packet.seed);
				for (auto iter = m_mapUser.begin(); iter != m_mapUser.end(); iter++)
				{
					iter->second->pDuelManager->StartGame();
				}
				gamestat = RUN;

			}
			break;
			case PACKET_HEADER_READY:
			{
				PACKET_READY packet;
				memcpy(&packet, m_buf, sizeof(packet));
				SOCKET sock = packet.sock;
				auto iter = m_mapUser.find(sock);
				if (iter != m_mapUser.end())
				{
					iter->second->pDuelManager->Ready(packet.ready_info);
				}
			}
			break;
			case PACKET_HEADER_ROOM_INFO:
			{
				PACKET_ROOM_INFO packet;
				memcpy(&packet, m_buf, sizeof(packet));
				USERINFO* pNew = new USERINFO;
				memcpy(pNew->UserName, &m_buf[sizeof(packet)], packet.name_len);
				pNew->UserName[packet.name_len] = '\0';
				pNew->sock = packet.sock;
				for (auto iter = m_vecDuelManager.begin(); iter != m_vecDuelManager.end(); iter++)
				{
					if ((*iter)->CheckUsing() == false)
					{
						pNew->pDuelManager = *iter;
						(*iter)->Connect();
						(*iter)->SetUserName(pNew->UserName);
						m_mapUser.insert(make_pair(pNew->sock, pNew));
						if (packet.ready == TRUE)
						{
							pNew->pDuelManager->Ready(TRUE);
						}
						break;
					}
				}
			}
			break;
			case PACKET_HEADER_INPUT:
			{
				PACKET_INPUT packet;
				memcpy(&packet, m_buf, sizeof(packet));
				SOCKET sock = packet.sock;
				auto iter = m_mapUser.find(sock);
				if (iter != m_mapUser.end())
				{
					iter->second->pDuelManager->Input(packet.input);
				}
			}
			break;
			case PACKET_HEADER_LOGOUT:
			{
				PACKET_LOGOUT packet;
				memcpy(&packet, m_buf, sizeof(packet));
				SOCKET sock = packet.sock;
				auto iter = m_mapUser.find(sock);
				if (iter != m_mapUser.end())
				{
					iter->second->pDuelManager->Disconnect();
					SAFE_DELETE(iter->second);
					m_mapUser.erase(sock);
				}
			}
			break;
			case PACKET_HEADER_ATTACK:
			{
				PACKET_ATTACK packet;
				memcpy(&packet, m_buf, sizeof(packet));
				SOCKET sock = packet.sock;
				int line = packet.line;
				bool b_find = false;
				for (auto iter = m_mapUser.begin(); iter != m_mapUser.end(); iter++)
				{
					if (iter->first == sock)
					{
						b_find = true;
						continue;
					}
					else
					{
						for (int i = 0; i < line; i++)
							iter->second->pDuelManager->PushLine(packet.seed);
					}

				}
				if (b_find == true)
				{
					for (int i = 0; i < line; i++)
						GameManager::GetInstance()->PushLine(packet.seed);
				}
			}
			break;
			case PACKET_HEADER_GAMEOVER:
			{
				gamestat = GAMEOVER;
				GameManager::GetInstance()->GameOver();
				for (auto iter = m_vecDuelManager.begin(); iter != m_vecDuelManager.end(); iter++)
				{
					(*iter)->GameOver();
				}

			}
			break;
			}
			if (len > m_packet.len)
			{
				memcpy(m_buf, &m_buf[m_packet.len], retval);
				len -= m_packet.len;
				memcpy(&m_packet, m_buf, sizeof(m_packet));
				continue;
			}
			len = 0;
			break;
		}
	}
	return true;
}
void Scene_game::StopScene()
{
	gamestat = WAIT;
	GameManager::GetInstance()->Reset();
	for (auto iter = m_vecDuelManager.begin(); iter != m_vecDuelManager.end(); iter++)
	{
		(*iter)->Reset();
	}
	BlockManager::GetInstance()->Release();
}
void Scene_game::RunScene()
{
}

Scene_game::~Scene_game()
{
}
