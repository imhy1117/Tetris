#pragma once
#include "Scene.h"
#include "Define.h"
#include "Button.h"
#include "..\\..\\Com\Packet.h"
#include <map>	
#include <string>
#include <vector>
using namespace std;
class DuelManager;
class Scene_game:public Scene
{
private:
	GAME_STAT gamestat;
	Button readyButton;
	Button cancleButton;
	Button outButton;
	HEADER m_packet;
	SOCKET m_sock;
	int retval,len;
	float m_fGameoverTime;
	HFONT m_hFont, m_hOldFont;
	vector<DuelManager*>m_vecDuelManager;
	map<SOCKET,USERINFO*>m_mapUser;
	char m_buf[BUFSIZE + 1];
public:
	Scene_game();
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
	~Scene_game();
};

