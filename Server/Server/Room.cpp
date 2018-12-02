#include "Room.h"
#include "UserManager.h"
#include "PacketManager.h"

Room::Room()
{
}
void Room::Init()
{
	m_iPlayerNum=0;
	m_bGameStart = false;
	m_iGameOverCount = 0;
	m_bOpen=true;
}
void Room::AddPlayer(SOCKET sock)
{
	USERINFO* ptr = UserManager::GetInstance()->GetUser(sock);
	m_mapPlayer.insert(make_pair(sock, ptr));
	ptr->pCurRoom = this;
	m_iPlayerNum++;
	char buf[BUFSIZE + 1];
	char* pName = ptr->ch_username;
	PACKET_ROOM_INFO packet;
	packet.header.name = PACKET_HEADER_ROOM_INFO;
	packet.sock = sock;
	packet.name_len = (WORD)strlen(pName);
	packet.header.len = sizeof(packet) + (WORD)strlen(pName);
	for (auto iter = m_mapPlayer.begin(); iter != m_mapPlayer.end(); iter++)
	{
		if (iter->first == sock)
			continue;
		
		packet.ready = FALSE;
		memcpy(buf, &packet, sizeof(packet));
		memcpy(&buf[sizeof(packet)], pName, packet.name_len);
		send(iter->first, buf, packet.header.len, 0);
	}
	for (auto iter = m_mapPlayer.begin(); iter != m_mapPlayer.end(); iter++)
	{
		if (iter->first == sock)
			continue;
		if (iter->second->ready == true)
		{
			packet.ready = TRUE;
		}
		else if (iter->second->ready == false)
		{
			packet.ready = FALSE;
		}
		packet.sock = iter->first;
		packet.name_len = (WORD)strlen(iter->second->ch_username);
		packet.header.len = sizeof(packet) + packet.name_len;
		memcpy(buf, &packet, sizeof(packet));
		memcpy(&buf[sizeof(packet)], iter->second->ch_username, packet.name_len);
		send(sock, buf, packet.header.len, 0);
	}

}
void Room::RemovePlayer(SOCKET sock)
{
	auto iter = m_mapPlayer.find(sock);
	if (iter != m_mapPlayer.end())
	{
		iter->second->pCurRoom = NULL;
		iter->second->ready = false;
		m_mapPlayer.erase(sock);
		m_iPlayerNum--;
	}
	PACKET_LOGOUT packet;
	packet.header.name = PACKET_HEADER_LOGOUT;
	packet.header.len = sizeof(packet);
	packet.sock = sock;
	for (auto iter = m_mapPlayer.begin(); iter != m_mapPlayer.end(); iter++)
	{
		send(iter->first, (char*)&packet, packet.header.len, 0);;
	}
}
void Room::Ready(SOCKET sock, BOOL ready)
{
	auto iter = m_mapPlayer.find(sock);
	if (iter != m_mapPlayer.end())
	{
		if (ready == TRUE)
		{
			iter->second->ready = true;
		}
		else if (ready == FALSE)
		{
			iter->second->ready = false;
		}
	}
	else
	{
		return;
	}
	PACKET_READY packet;
	packet.header.name = PACKET_HEADER_READY;
	packet.header.len = sizeof(packet);
	packet.ready_info = ready;
	packet.sock = sock;
	for (auto iter = m_mapPlayer.begin(); iter != m_mapPlayer.end(); iter++)
	{
		if (iter->first == sock)
			continue;
		else
		{
			send(iter->first, (char*)&packet, packet.header.len, 0);
		}
	}
	StartGame();
}
void Room::StartGame()
{
	int seed = (int)GetTickCount();
	int tmp = 0;
	for (auto iter = m_mapPlayer.begin(); iter != m_mapPlayer.end(); iter++)
	{
		if (iter->second->ready == true)
			tmp++;
	}
	PACKET_STARTGAME start;
	start.header.name = PACKET_HEADER_STARTGAME;
	start.header.len = sizeof(start);
	start.seed = seed;
	if (m_iPlayerNum>1 && tmp == m_iPlayerNum)
	{
		for (auto iter = m_mapPlayer.begin(); iter != m_mapPlayer.end(); iter++)
		{
			iter->second->ready = false;
			send(iter->first, (char*)&start, sizeof(start), 0);
		}
		m_bOpen = false;
		m_bGameStart = true;
	}
}
void Room::SendInput(SOCKET sock, WORD index)
{
	int retval;
	PACKET_INPUT packet;
	packet.header.name = PACKET_HEADER_INPUT;
	packet.header.len = sizeof(packet);
	packet.input = index;
	packet.sock = sock;
	printf("소켓 %d 전송 %d \n", sock, index);
	for (auto iter = m_mapPlayer.begin(); iter != m_mapPlayer.end(); iter++)
	{
		if (iter->first == sock)
			continue;
		else
		{
			retval = send(iter->first, (char*)&packet, packet.header.len, 0);
			if (retval == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSA_IO_PENDING)
				{
					PacketManager::GetInstance()->err_display("WSASend()");
				}
			}
		}
	}
}
void Room::Attack(SOCKET sock, int line)
{
	PACKET_ATTACK packet;
	packet.header.name = PACKET_HEADER_ATTACK;
	packet.header.len = sizeof(packet);
	packet.line = line;
	packet.sock = sock;
	packet.seed = rand() % 10;
	for (auto iter = m_mapPlayer.begin(); iter != m_mapPlayer.end(); iter++)
	{
		send(iter->first, (char*)&packet, packet.header.len, 0);
	}
}
void Room::GameOver(SOCKET sock)
{
	m_iGameOverCount++;
	if (m_iGameOverCount >= m_iPlayerNum - 1)
	{
		PACKET_GAMEOVER packet;
		packet.header.name = PACKET_HEADER_GAMEOVER;
		packet.header.len = sizeof(packet);
		for (auto iter = m_mapPlayer.begin(); iter != m_mapPlayer.end(); iter++)
		{
			send(iter->first, (char*)&packet, packet.header.len, 0);
		}
		m_bGameStart = false;
		m_iGameOverCount = 0;
	}
}
bool Room::FindPlayer(SOCKET sock)
{
	auto iter = m_mapPlayer.find(sock);
	if (iter != m_mapPlayer.end())
	{
		return true;
	}
	else
		return false;
}
void Room::Release()
{
	m_mapPlayer.clear();
}
Room::~Room()
{
}
