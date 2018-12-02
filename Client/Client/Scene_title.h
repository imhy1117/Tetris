#pragma once
#include "Scene.h"
#include "Define.h"
#include "..\\..\\Com\Packet.h"
#include "Button.h"
class Scene_title:public Scene
{
private:
	int retval, len;
	HEADER m_packet;
	HWND m_hWnd, m_hInput;
	HINSTANCE m_hInst;
	Button startButton;
	TCHAR m_chNotice[128];
	char m_buf[BUFSIZE + 1];
public:
	Scene_title();
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
	~Scene_title();
};

