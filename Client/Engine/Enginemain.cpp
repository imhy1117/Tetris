#include "Enginemain.h"
#include "SceneManager.h"
#include "NetManager.h"
#include "InputManager.h"
#include "Scene.h"
Enginemain::Enginemain(string winName, int size_cx, int size_cy)
{
	m_iSize_cx = size_cx;
	m_iSize_cy = size_cy;
	m_strWinName = winName;
}
Enginemain::~Enginemain()
{

}
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int Enginemain::StartEngine(HINSTANCE hInstance)
{
	MSG Message;
	WNDCLASS WndClass;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = m_strWinName.c_str();
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	HWND hWnd = CreateWindow(m_strWinName.c_str(), m_strWinName.c_str(), WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	SceneManager::GetInstance()->Init(hWnd,hInstance,m_iSize_cx,m_iSize_cy);
	while (true)
	{
		if (PeekMessage(&Message, NULL, 0U, 0U, PM_REMOVE))
		{
			if (Message.message == WM_QUIT)
				break;
			if (Message.message == WM_KEYUP&&Message.wParam == VK_RETURN)
				SceneManager::GetInstance()->GetCurScene()->SendtoServer();
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else
		{
			SceneManager::GetInstance()->Update();
			SceneManager::GetInstance()->Draw();
		}
	}
	SceneManager::GetInstance()->Release();
	return (int)Message.wParam;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	switch (iMessage)
	{
	case WM_SOCKET:
		NetManager::GetInstance()->ProcessSocketMessage(hWnd, iMessage, wParam, lParam);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
			InputManager::GetInstance()->ActivateWindow(false);
		else 
			InputManager::GetInstance()->ActivateWindow(true);
		return 0;
	case WM_LBUTTONDOWN:
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		SceneManager::GetInstance()->Click(pt);
		return 0;

	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
	
