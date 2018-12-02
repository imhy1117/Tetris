#pragma once
#pragma comment(lib,"ws2_32")
#include <WinSock2.h>
#include <Windows.h>
#include "SingleTon.h"
#include "Macro.h"
#include <stdlib.h>
#include <crtdbg.h>
#include <string>
#define WM_SOCKET (WM_USER+1)

using namespace std;
#define CRTDBG_MAP_ALLOC
#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

 #ifdef NDEBUG
 #ifdef _DEBUG
#undef _DEBUG
 #endif
 #endif

enum ANCHOR
{
	ANCHOR_NORMAL,
	ANCHOR_CENTER
};