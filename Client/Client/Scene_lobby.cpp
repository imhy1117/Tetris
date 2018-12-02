#include "Scene_lobby.h"
#include "SceneManager.h"
#include "NetManager.h"
#include "ResManager.h"
#include "Room.h"
#define ID_EDIT 100
Scene_lobby::Scene_lobby()
{
}
void Scene_lobby::Init()
{
	m_iLen = 0;
	m_iChatlen = 0;
	m_strChat = new string;
	m_strPlayer = new string;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Room* pNew = new Room();
			pNew->Init(65+(j*260), 70+(180*i), (i*3)+j);
			m_vecRoom.push_back(pNew);
		}
	}
	m_hWnd = SceneManager::GetInstance()->GethWnd();
	m_hInst= SceneManager::GetInstance()->GethInst();
	m_hChat = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE |ES_READONLY| WS_VSCROLL|ES_LEFT | ES_MULTILINE |ES_AUTOVSCROLL|WS_DLGFRAME, 50, 600, 450, 100, m_hWnd, NULL, m_hInst, NULL);
	m_hInput = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |ES_MULTILINE|WS_VSCROLL, 50, 700, 450, 20, m_hWnd, (HMENU)ID_EDIT, m_hInst, NULL);
	m_hPlayerList= CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_READONLY | ES_MULTILINE | ES_AUTOHSCROLL | WS_DLGFRAME, 890, 50, 360, 400, m_hWnd, NULL, m_hInst, NULL);
	ShowWindow(m_hChat, SW_SHOW);
	ShowWindow(m_hInput, SW_SHOW);
	ShowWindow(m_hPlayerList, SW_SHOW);
	PACKET_REQUEST_INFO	info;
	m_sock = NetManager::GetInstance()->GetSocket();
	info.header.name = PACKET_HEADER_REQUEST_INFO;
	info.header.len = sizeof(info);
	send(m_sock, (char*)&info, info.header.len, 0);
}
void Scene_lobby::Input()
{

}
void Scene_lobby::Update()
{
	
}
void Scene_lobby::Draw()
{
	ResManager::GetInstance()->GetBitMap("Res\\Lobby.bmp")->Draw(0, 0);
	for (auto iter = m_vecRoom.begin(); iter != m_vecRoom.end(); iter++)
	{
		(*iter)->Draw();
	}
}
void Scene_lobby::Release()
{
	ShowWindow(m_hChat, SW_HIDE);
	ShowWindow(m_hInput, SW_HIDE);
	ShowWindow(m_hPlayerList, SW_HIDE);
	for (auto iter = m_mapUser.begin(); iter != m_mapUser.end(); iter++)
	{
		SAFE_DELETE(iter->second);
	}
	m_mapUser.clear();
	SAFE_DELETE(m_strChat);
	SAFE_DELETE(m_strPlayer);
	for (auto iter = m_vecRoom.begin();iter != m_vecRoom.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	m_vecRoom.clear();
}
void Scene_lobby::Click(POINT pt)
{
	for (auto iter = m_vecRoom.begin(); iter != m_vecRoom.end(); iter++)
	{
		if ((*iter)->Click(pt) == true)
			break;
	}
}
bool Scene_lobby::ProcessPacket(WPARAM wParam)
{
	char buf[BUFSIZE + 1];
	int retval = recv(wParam, buf, BUFSIZE, 0);
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
	memcpy(&m_buf[m_iLen], buf, retval);
	memcpy(&m_packet, m_buf, sizeof(m_packet));
	m_iLen += retval;
	if (retval > m_packet.len)
	{
		m_iLen = retval;
	}
	if (m_iLen < m_packet.len)
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
				case PACKET_HEADER_LOGIN:
				{
					PACKET_LOGIN packet;
					memcpy(&packet, m_buf, sizeof(packet));
					USERINFO* pNew = new USERINFO;
					memcpy(pNew->UserName, &m_buf[sizeof(packet)], packet.name_len);
					pNew->UserName[packet.name_len] = '\0';
					pNew->sock = packet.sock;
					m_mapUser.insert(make_pair(pNew->sock, pNew));
					m_strPlayer->append(pNew->UserName, strlen(pNew->UserName));
					m_strPlayer->append("\r\n");
					SetWindowText(m_hPlayerList, m_strPlayer->c_str());
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
						SAFE_DELETE(iter->second);
						m_mapUser.erase(sock);
					}
					m_strPlayer->clear();
					for (iter = m_mapUser.begin(); iter != m_mapUser.end(); iter++)
					{
						m_strPlayer->append(iter->second->UserName, strlen(iter->second->UserName));
						m_strPlayer->append("\r\n");
					}
					SetWindowText(m_hPlayerList, m_strPlayer->c_str());
				}
				break;
				case PACKET_HEADER_LOBBY_INFO:
				{
					PACKET_LOBBY_INFO packet;
					memcpy(&packet, m_buf, sizeof(packet));
					m_vecRoom[packet.room_index]->RecvRoomInfo(packet.room_playernum, packet.room_stat);
				}
				break;
				case PACKET_HEADER_LOBBY_CHAT:
				{
					char ch_chat[256];
					PACKET_LOBBY_CHAT packet;
					TCHAR ch_tmp[256];
					memcpy(&packet, m_buf,sizeof(packet));
					memcpy(ch_chat,&m_buf[sizeof(packet)], packet.chat_len);
					ch_chat[packet.chat_len] = '\0';
					SOCKET sock = packet.sock;
					auto iter = m_mapUser.find(sock);
					if (iter != m_mapUser.end())
					{
						wsprintf(ch_tmp, "%s : %s\r\n", iter->second->UserName, ch_chat);
					}
					m_strChat->append(ch_tmp,strlen(ch_tmp));
					SetWindowText(m_hChat, m_strChat->c_str());
				}
				break;
				default:
					break;
			}
			if (m_iLen > m_packet.len)
			{
				memcpy(m_buf, &m_buf[m_packet.len], retval);
				m_iLen -= m_packet.len;
				memcpy(&m_packet, m_buf, sizeof(m_packet));
				continue;
			}
			m_iLen = 0;
			break;
		}
	}
	return true;
}

void Scene_lobby::StopScene()
{
}
void Scene_lobby::RunScene()
{
}
void Scene_lobby ::SendtoServer()
{
	char ch_msg[256];
	char buf[BUFSIZE + 1];
	int len = GetWindowTextLength(m_hInput);
	if (len <= 2)
	{
		SetWindowText(m_hInput, "");
		return;
	}
	GetWindowText(m_hInput, ch_msg, len-1);
	PACKET_LOBBY_CHAT packet;
	packet.header.name = PACKET_HEADER_LOBBY_CHAT;
	packet.sock = 0;
	packet.chat_len = len - 2;
	packet.header.len = packet.chat_len + sizeof(packet);
	memcpy(buf, &packet, sizeof(packet));
	memcpy(&buf[sizeof(packet)], ch_msg, packet.chat_len);
	send(m_sock, buf, packet.header.len, 0);
	SetWindowText(m_hInput, "");
}
Scene_lobby::~Scene_lobby()
{
}

