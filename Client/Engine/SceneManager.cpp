#include "SceneManager.h"
#include "Scene.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "ResManager.h"
#include "NetManager.h"
SceneManager::SceneManager()
{
	m_pCurScene = NULL;
}

void SceneManager::Init(HWND hWnd, HINSTANCE hInst,int winSizeCx,int winSizeCy)
{
	m_hWnd = hWnd;
	m_hInst = hInst;
	ResManager::GetInstance()->Init(hWnd, winSizeCx, winSizeCy);
	InputManager::GetInstance()->Init(hWnd);
	NetManager::GetInstance()->Init(hWnd);
	RECT rcClient, rcWindow;
	GetClientRect(hWnd, &rcClient);
	GetWindowRect(hWnd, &rcWindow);

	int CXFrame = (rcWindow.right - rcWindow.left) - (rcClient.right - rcClient.left);
	int CYFrame = (rcWindow.bottom - rcWindow.top) - (rcClient.bottom - rcClient.top);
	MoveWindow(hWnd, 0, 0, winSizeCx + CXFrame, winSizeCy + CYFrame, true);
	
	m_pCurScene = m_vecScene[0];
	m_pCurScene->Init();
}
void SceneManager::RegistScene(Scene* pNew)
{
	m_vecScene.push_back(pNew);

}
void SceneManager::LoadScene(int index)
{
	m_pCurScene->Release();
	m_pCurScene = m_vecScene[index];
	m_pCurScene->Init();
	
}
void SceneManager::Update()
{
	TimeManager::GetInstance()->Update();
	InputManager::GetInstance()->Update();
	m_pCurScene->Update();
}
void SceneManager::Draw()
{
	HDC hdc = GetDC(m_hWnd);
	ResManager::GetInstance()->DrawBack();
	m_pCurScene->Draw();
	ResManager::GetInstance()->Draw(hdc);
	ReleaseDC(m_hWnd, hdc);
}
void SceneManager::Release()
{
	m_pCurScene->Release();
	for (auto iter = m_vecScene.begin(); iter != m_vecScene.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}

	m_vecScene.clear();
	ResManager::GetInstance()->Release();
	InputManager::GetInstance()->DestroyInstance();
	TimeManager::GetInstance()->DestroyInstance();
	NetManager::GetInstance()->Release();
	DestroyInstance();
}
bool SceneManager::ProcessPacket(WPARAM wParam)
{
	return m_pCurScene->ProcessPacket(wParam);
}
void SceneManager::Click(POINT pt)
{
	m_pCurScene->Click(pt);
}
SceneManager::~SceneManager()
{
}
