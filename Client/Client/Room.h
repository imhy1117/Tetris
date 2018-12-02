#pragma once
#include "Define.h"
#include "Button.h"
#include "..\\..\\Com\Packet.h"
class Room
{
private:
	POINT m_pt;
	Button entrance;
	TCHAR m_chPlayerNum;
	int m_iPlayerNum,m_iRoomIndex;
	//HFONT m_hFont, m_hOldFont;
	bool m_bOpen,m_bPlaying;
public:
	Room();
	void Init(int x, int y,int roomIndex);
	inline void RecvRoomInfo(int playerNum, int roomstat)
	{
		m_iPlayerNum = playerNum;
		if (m_iPlayerNum <4)
		{
			m_bOpen = true;
		}
		if (m_iPlayerNum > 3)
		{
			m_bOpen = false;
		}
		if (roomstat == ROOM_PLAYING)
		{
			m_bPlaying = true;
			m_bOpen = false;
		}
		wsprintf(&m_chPlayerNum, "%d / 4", m_iPlayerNum);
	}
	void Draw();
	bool Click(POINT pt);
	~Room();
};

