#pragma once
#include "EngineDefine.h"
class TimeManager:public SingleTon<TimeManager>
{
private:
	DWORD m_dwLastTime, m_dwCurTime;
	float m_fElapseTime;
public:
	TimeManager();
	inline float GetElapseTime()
	{
		return m_fElapseTime;
	}
	void Update();
	~TimeManager();
};

