#include "TimeManager.h"

TimeManager::TimeManager()
{
	m_dwCurTime = GetTickCount();
	m_dwLastTime = GetTickCount();
}

void TimeManager::Update()
{
	m_dwCurTime = GetTickCount();
	m_fElapseTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;
	m_dwLastTime = m_dwCurTime;
}
TimeManager::~TimeManager()
{
}
