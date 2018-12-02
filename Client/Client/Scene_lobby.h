#pragma once
#include "Scene.h"
#include "Define.h"
#include "..\\..\\Com\Packet.h"
#include "Button.h"
#include <map>
#include <vector>
#include <string>
using namespace std;
class Room;
class Scene_lobby :
	public Scene
{
private:
	SOCKET m_sock;
	int m_iLen;
	map<SOCKET, USERINFO*>m_mapUser;
	HEADER m_packet;
	HWND m_hWnd, m_hChat, m_hInput,m_hPlayerList;
	WNDPROC m_editProc;
	HINSTANCE m_hInst;
	string* m_strChat;
	string* m_strPlayer;
	int m_iChatlen;
	vector<Room*>m_vecRoom;
	char m_buf[BUFSIZE + 1];
public:
	virtual void Init();
	virtual void Input();
	virtual void Update();
	virtual void Draw();
	virtual void Release();
	virtual void Click(POINT pt);
	virtual bool ProcessPacket(WPARAM wParam);
	virtual void StopScene();
	virtual void RunScene();
	virtual void SendtoServer();
	Scene_lobby();
	~Scene_lobby();
};

