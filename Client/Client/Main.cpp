#include <Enginemain.h>
#include "SceneManager.h"
#include "Scene_game.h"
#include "Scene_title.h"
#include "Scene_lobby.h"
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc();
	SceneManager::GetInstance()->RegistScene(new Scene_title);
	SceneManager::GetInstance()->RegistScene(new Scene_lobby);
	SceneManager::GetInstance()->RegistScene(new Scene_game);
	Enginemain engine("Tetris", 1280, 720);
	return engine.StartEngine(hInstance);
}