#include "UserManager.h"
#include "PacketManager.h"
#include "Room.h"
UserManager::UserManager()
{
	
}
void UserManager::Init()
{
	m_iUserNum = 0;
	m_iPlayerNum = 0;
	m_bGameStart = false;
	m_iGameOverCount = 0;
	for (int i = 0; i < 9; i++)
	{
		Room* pNew = new Room();
		pNew->Init();
		m_vecRoom.push_back(pNew);
	}
}
void UserManager::AddUser(SOCKET sock)
{
	USERINFO* pNew = new USERINFO();
	ZeroMemory(&pNew->overlapped, sizeof(pNew->overlapped));
	pNew->sock = sock;
	pNew->ready = false;
	pNew->len = 0;
	pNew->pCurRoom = NULL;
	m_mapUser.insert(std::make_pair(sock, pNew));
	PACKET_CONNECT connect;
	connect.header.name = PACKET_HEADER_CONNECT;
	connect.header.len = sizeof(connect);
	retval = send(pNew->sock, (char*)&connect, sizeof(connect), 0);
	if (retval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			PacketManager::GetInstance()->err_display("WSASend()");
		}
	}
}

USERINFO* UserManager::GetUser(SOCKET sock)
{
	auto iter = m_mapUser.find(sock);
	if (iter != m_mapUser.end())
	{
		return iter->second;
	}
	else
	{
		return 0;
	}
}
void UserManager::Ready(SOCKET sock,BOOL ready)
{
	for (auto iter = m_vecRoom.begin(); iter != m_vecRoom.end(); iter++)
	{
		if ((*iter)->FindPlayer(sock) == true)
		{
			(*iter)->Ready(sock, ready);
			break;
		}
	}
}
void UserManager::LogIn(SOCKET sock)
{
	char buf[BUFSIZE + 1];
	char* pName = GetUser(sock)->ch_username;
	PACKET_LOGIN packet;
	packet.header.name = PACKET_HEADER_LOGIN;
	packet.header.len = sizeof(packet)+ (WORD)strlen(pName);
	packet.sock = sock;
	packet.name_len = (WORD)strlen(pName);
	memcpy(buf, &packet, sizeof(packet));
	memcpy(&buf[sizeof(packet)], pName, strlen(pName));
	for (auto iter = m_mapUser.begin(); iter != m_mapUser.end(); iter++)
	{
		if (iter->first == sock)
			continue;
		else
		{
			send(iter->first, buf, packet.header.len, 0);
		}
	}
}
void UserManager::EchoChat(SOCKET sock)
{
	USERINFO* ptr = GetUser(sock);
	char buf[BUFSIZE + 1];
	PACKET_LOBBY_CHAT packet;
	packet.header.name = PACKET_HEADER_LOBBY_CHAT;
	packet.header.len = sizeof(packet)+ptr->i_chatlen;
	packet.sock = sock;
	packet.chat_len= ptr->i_chatlen;
	memcpy(buf, &packet, sizeof(packet));
	memcpy(&buf[sizeof(packet)], ptr->ch_chat, packet.chat_len);
	for (auto iter = m_mapUser.begin(); iter != m_mapUser.end(); iter++)
	{
		send(iter->first, buf, packet.header.len, 0);
	}
}
void UserManager::RoomIn(SOCKET sock, int roomnum)
{
	m_vecRoom[roomnum]->AddPlayer(sock);
	for (auto iter1 = m_mapUser.begin(); iter1 != m_mapUser.end(); iter1++)
	{
		if (iter1->first == sock)
			continue;
		else
		{
			PACKET_LOBBY_INFO lobbyinfo;
			lobbyinfo.header.name = PACKET_HEADER_LOBBY_INFO;
			lobbyinfo.header.len = sizeof(lobbyinfo);
			lobbyinfo.room_index = roomnum;
			lobbyinfo.room_playernum = m_vecRoom[roomnum]->GetRoomPlayerNum();
			lobbyinfo.room_stat = m_vecRoom[roomnum]->GetRoomStat();
			send(iter1->first, (char*)&lobbyinfo, lobbyinfo.header.len, 0);
		}
	}
}
void UserManager::LobbyIn(SOCKET sock)
{
	char buf[BUFSIZE + 1];
	PACKET_LOGIN packet;
	packet.header.name = PACKET_HEADER_LOGIN;
	for (auto iter = m_mapUser.begin(); iter != m_mapUser.end(); iter++)
	{
		packet.sock = iter->first;
		packet.header.len = sizeof(packet) + (WORD)strlen(iter->second->ch_username);
		packet.name_len = (WORD)strlen(iter->second->ch_username);
		memcpy(buf, &packet, sizeof(packet));
		memcpy(&buf[sizeof(packet)], iter->second->ch_username, packet.name_len);
		send(sock, buf, packet.header.len, 0);
	}
	int tmp = 0;
	PACKET_LOBBY_INFO lobbyinfo;
	lobbyinfo.header.name = PACKET_HEADER_LOBBY_INFO;
	lobbyinfo.header.len = sizeof(lobbyinfo);
	for (auto iter = m_vecRoom.begin(); iter != m_vecRoom.end(); iter++, tmp++)
	{
		lobbyinfo.room_index = tmp;
		lobbyinfo.room_playernum = (*iter)->GetRoomPlayerNum();
		lobbyinfo.room_stat = (*iter)->GetRoomStat();
		send(sock, (char*)&lobbyinfo, lobbyinfo.header.len, 0);
	}
}
void UserManager::RoomOut(SOCKET sock)
{
	int tmp = 0;
	for (auto iter = m_vecRoom.begin(); iter != m_vecRoom.end(); iter++,tmp++)
	{
		if ((*iter)->FindPlayer(sock) == true)
		{
			(*iter)->RemovePlayer(sock);
			break;
		}
	}
	for (auto iter1 = m_mapUser.begin(); iter1 != m_mapUser.end(); iter1++)
	{
		if (iter1->first == sock)
			continue;
		else
		{
			PACKET_LOBBY_INFO lobbyinfo;
			lobbyinfo.header.name = PACKET_HEADER_LOBBY_INFO;
			lobbyinfo.header.len = sizeof(lobbyinfo);
			lobbyinfo.room_index = tmp;
			lobbyinfo.room_playernum = m_vecRoom[tmp]->GetRoomPlayerNum();
			lobbyinfo.room_stat = m_vecRoom[tmp]->GetRoomStat();
			send(iter1->first, (char*)&lobbyinfo, lobbyinfo.header.len, 0);
		}
	}
}
void UserManager::GameOver(SOCKET sock)
{
	for (auto iter = m_vecRoom.begin(); iter != m_vecRoom.end(); iter++)
	{
		if ((*iter)->FindPlayer(sock) == true)
		{
			(*iter)->GameOver(sock);
			break;
		}
	}
}
void UserManager::LogOut(SOCKET sock)
{
	PACKET_LOGOUT packet;
	packet.header.name = PACKET_HEADER_LOGOUT;
	packet.header.len = sizeof(packet);
	packet.sock = sock;
	for (auto iter = m_mapUser.begin(); iter != m_mapUser.end(); iter++)
	{
		send(iter->first, (char*)&packet, packet.header.len, 0);;
	}
}
void UserManager::Attack(SOCKET sock, int line)
{
	for (auto iter = m_vecRoom.begin(); iter != m_vecRoom.end(); iter++)
	{
		if ((*iter)->FindPlayer(sock) == true)
		{
			(*iter)->Attack(sock,line);
			break;
		}
	}
}
void UserManager::Remove(SOCKET sock)
{
	for (auto iter = m_vecRoom.begin(); iter != m_vecRoom.end(); iter++)
	{
		if ((*iter)->FindPlayer(sock) == true)
		{
			(*iter)->RemovePlayer(sock);
			break;
		}
	}
	auto iter2 = m_mapUser.find(sock);
	if (iter2 != m_mapUser.end())
	{
		delete iter2->second;
		closesocket(sock);
		m_mapUser.erase(sock);
		LogOut(sock);
		m_iUserNum--;
	}
}

void UserManager::Release()
{
	for (auto iter = m_vecRoom.begin(); iter != m_vecRoom.end(); iter++)
	{
		(*iter)->Release();
		SAFE_DELETE(*iter);
	}
	for (auto iter = m_mapUser.begin(); iter != m_mapUser.end(); iter++)
	{
		delete iter->second;
	}
	m_mapUser.clear();
	DestroyInstance();
}
void UserManager::SendInput(SOCKET sock, WORD index)
{
	for (auto iter = m_vecRoom.begin(); iter != m_vecRoom.end(); iter++)
	{
		if ((*iter)->FindPlayer(sock) == true)
		{
			(*iter)->SendInput(sock,index);
			break;
		}
	}
}
UserManager::~UserManager()
{
}
