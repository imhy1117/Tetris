#pragma once
#include "EngineDefine.h"
class Scene
{
public:
	 Scene();
	virtual void Init() = 0;
	virtual void Input() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Release() = 0;
	virtual void Click(POINT pt) = 0;
	virtual bool ProcessPacket(WPARAM wParam) = 0;
	virtual void StopScene() = 0;
	virtual void RunScene() =0 ;
	virtual void SendtoServer() = 0;
	virtual~Scene();
};

