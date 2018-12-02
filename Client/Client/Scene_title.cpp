#include "Scene_title.h"
#include "SceneManager.h"
#include "NetManager.h"
#include "ResManager.h"
Scene_title::Scene_title()
{
}
void Scene_title::Init()
{
	SetBkMode(ResManager::GetInstance()->GetBackDC(), TRANSPARENT);
	startButton.Init(400, 300, "Res\\Start.bmp");
	len = 0;
	retval = 0;
	wsprintf(m_chNotice, "닉네임을 3글자 이상 입력해주세요");
	m_hWnd = SceneManager::GetInstance()->GethWnd();
	m_hInst = SceneManager::GetInstance()->GethInst();
	m_hInput = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 400, 200, 450, 40, m_hWnd, NULL, m_hInst, NULL);
}
void Scene_title::Input()
{

}
void Scene_title::Update()
{

}
void Scene_title::Draw()
{
	startButton.Draw();
	TextOut(ResManager::GetInstance()->GetBackDC(), 400, 100, m_chNotice, lstrlen(m_chNotice));
}
void Scene_title::Release()
{
}
void Scene_title::Click(POINT pt)
{
	if (startButton.CheckClick(pt))
	{
		int len = GetWindowTextLength(m_hInput);
		if (len <= 2)
		{
			return;
		}
		NetManager::GetInstance()->Connect(SERVERIP, SERVERPORT);
	}
}
bool Scene_title::ProcessPacket(WPARAM wParam)
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
			case PACKET_HEADER_CONNECT:
			{
				char ch_msg[256];
				string strName;
				int len = GetWindowTextLength(m_hInput);
				GetWindowText(m_hInput,ch_msg, len+1);
				strName.append(ch_msg, len);
				NetManager::GetInstance()->SetNickname(strName);
				PACKET_LOGIN login;
				login.header.name = PACKET_HEADER_LOGIN;
				login.header.len = sizeof(login);
				login.name_len = (WORD)strName.length();
				memcpy(buf, &login, login.header.len);
				memcpy(&buf[login.header.len], strName.c_str(), login.name_len);
				send(NetManager::GetInstance()->GetSocket(), buf, login.header.len + login.name_len, 0);
				ShowWindow(m_hInput, SW_HIDE);
				SceneManager::GetInstance()->LoadScene(SCENE_LOBBY);
			}
			break;
			}
			break;
		}
	}
	return true;
}
void Scene_title::StopScene()
{
}
void Scene_title::RunScene()
{
}
void Scene_title::SendtoServer()
{

}
Scene_title::~Scene_title()
{
}
