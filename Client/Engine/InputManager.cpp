#include "InputManager.h"
InputManager::InputManager()
{
}
InputManager::~InputManager()
{
}
void InputManager::Init(HWND hWnd)
{
	m_hWnd = hWnd;
	m_bCursor = true;
	m_bActivate = true;
}
void InputManager::RegistKey(int key)
{
	if (CheckKeyRegist(key) == true)
		return;
	else
	{
		KEY_STATE regikey;
		memset(&regikey, 0, sizeof(regikey));
		regikey.keyCode = key;
		m_vecKeyState.push_back(regikey);
	}
}
void InputManager::DisplayCursor(bool show)
{
	if (m_bCursor == true)
	{
		if (show == false)
		{
			m_bCursor = false;
			ShowCursor(FALSE);
			return;
		}
	}
	else if (m_bCursor == false)
	{
		if (show == true)
		{
			m_bCursor = true;
			ShowCursor(TRUE);
			return;
		}
	}
}
void InputManager::ActivateWindow(bool activate)
{
	m_bActivate = activate;
}
void InputManager::Update()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(m_hWnd, &pt);
	m_mousePt.x = pt.x;
	m_mousePt.y = pt.y;
	for (auto iter = m_vecKeyState.begin(); iter != m_vecKeyState.end(); iter++)
	{
		if (GetKeyState((*iter).keyCode) & 0x8000 && m_bActivate)
		{
			(*iter).keyPress=true;
			(*iter).keyDown = true;
		}
		else
		{
			if ((*iter).keyPress==true)
			{
				(*iter).keyPress = false;
				(*iter).keyDown = false;
				(*iter).keyUp = true;
			}
			else if ((*iter).keyPress == false)
			{
				(*iter).keyUp = false;
			}
		}
	}
}
bool InputManager::CheckKeyRegist(int key)
{
	for (auto iter = m_vecKeyState.begin(); iter != m_vecKeyState.end(); iter++)
	{
		if ((*iter).keyCode == key)
			return true;
	}
	return false;
}
bool InputManager::CheckKeyUp(int key)
{
	for (auto iter = m_vecKeyState.begin(); iter != m_vecKeyState.end(); iter++)
	{
		if ((*iter).keyCode == key&&m_bActivate)
			return (*iter).keyUp;
	}
	return false;
}
bool InputManager::CheckKeyDown(int key)
{
	for (auto iter = m_vecKeyState.begin(); iter != m_vecKeyState.end(); iter++)
	{
		if ((*iter).keyCode == key&&m_bActivate)
			return (*iter).keyDown;
	}
	return false;
}
POINT InputManager::GetMousePt()
{
	return m_mousePt;
}
	

