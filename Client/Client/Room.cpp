#include "Room.h"
#include "ResManager.h"
#include "GameManager.h"
#include "SceneManager.h"
Room::Room()
{
}
void Room::Init(int x, int y, int roomIndex)
{
	m_pt.x = x;
	m_pt.y = y;
	m_iRoomIndex = roomIndex;
	m_iPlayerNum=0;
	m_bOpen =true;
	m_bPlaying = false;
	entrance.Init(m_pt.x + 135, m_pt.y + 90,"Res\\GetIn.bmp");
	wsprintf(&m_chPlayerNum, "%d / 4", m_iPlayerNum);
}

void Room::Draw()
{
	ResManager::GetInstance()->GetBitMap("Res\\Room.bmp")->Draw(m_pt.x,m_pt.y);
	if (m_bOpen)
	{
		entrance.Draw();
	}
	TextOut(ResManager::GetInstance()->GetBackDC(), m_pt.x + 20, m_pt.y + 20, &m_chPlayerNum, lstrlen(&m_chPlayerNum));
}
bool Room::Click(POINT pt)
{
	if (m_bOpen)
	{
		if (entrance.CheckClick(pt) == true)
		{
			GameManager::GetInstance()->SetRoomNum(m_iRoomIndex);
			SceneManager::GetInstance()->LoadScene(SCENE_GAME);
			return true;
		}
	}
	return false;
}

Room::~Room()
{
}
