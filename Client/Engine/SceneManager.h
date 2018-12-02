#pragma once
#include "EngineDefine.h"
#include <vector>


class Scene;
class SceneManager:public SingleTon<SceneManager>
{
private:
	vector<Scene*>m_vecScene;
	HWND m_hWnd;
	HINSTANCE m_hInst;
	Scene* m_pCurScene;
public:
	SceneManager();
	void Init(HWND hWnd, HINSTANCE hInst,int winSizeCx, int winSizeCy);
	inline Scene* GetCurScene()
	{
		return m_pCurScene;
	}
	inline HINSTANCE GethInst()
	{
		return m_hInst;
	}
	inline HWND GethWnd()
	{
		return m_hWnd;
	}
	bool ProcessPacket(WPARAM wParam);
	void RegistScene(Scene* pNew);
	void Update();
	void Draw();
	void Release();
	void LoadScene(int index);
	void Click(POINT pt);
	~SceneManager();
};

