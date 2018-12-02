#pragma once
#include "EngineDefine.h"
#include <string>
class NetManager:public SingleTon<NetManager>
{
private:
	SOCKET sock;
	HWND m_hWnd;
	string m_strNickname;
	int retval;
public:
	NetManager();
	void Init(HWND hWnd);
	bool Connect(string serverip, int serverport);
	void ProcessSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	inline void SetNickname(string nickname)
	{
		m_strNickname = nickname;
	}
	inline string GetNickName()
	{
		return m_strNickname;
	}
	inline SOCKET GetSocket()
	{
		return sock;
	}
	void err_quit(char *msg);
	void err_display(char *msg);
	void err_display(int errcode);
	void Release();
	~NetManager();
};

