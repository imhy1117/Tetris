#pragma once
#include "EngineDefine.h"
#include <vector>

class InputManager :public SingleTon<InputManager>
{
	struct KEY_STATE
	{
		int keyCode;
		bool keyPress;
		bool keyDown;
		bool keyUp;
	};
private:
	POINT m_mousePt;
	HWND m_hWnd;
	bool m_bCursor;
	bool m_bActivate;
	vector<KEY_STATE> m_vecKeyState;
public:
	InputManager();
	void Init(HWND hWnd);
	void Update();
	void RegistKey(int key);
	void ActivateWindow(bool activate);
	void DisplayCursor(bool show);
	POINT GetMousePt();
	bool CheckKeyRegist(int key);
	bool CheckKeyUp(int key);
	bool CheckKeyDown(int key);
	~InputManager();
};


