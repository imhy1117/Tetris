#pragma once
#include "EngineDefine.h"

//리소스 정의
#define BLOCK_GRAY "Res\\cell_0.bmp"
#define BLOCK_RED "Res\\cell_1.bmp"
#define BLOCK_BLUE "Res\\cell_2.bmp"
#define BLOCK_ORANGE "Res\\cell_3.bmp"
#define BLOCK_YELLOW "Res\\cell_4.bmp"
#define BLOCK_GREEN "Res\\cell_5.bmp"
#define BLOCK_PURPLE "Res\\cell_6.bmp"
#define BLOCK_PINK "Res\\cell_7.bmp"
#define DIR_FIELD "Res\\Field.bmp"
#define DIR_NEXT "Res\\Next.bmp"
#define DIR_INTERFACE "Res\\interface.bmp"
#define DIR_READY_STAT "Res\\ReadyStat.bmp"
//기타 정의
#define BLOCK_SIZE 20

#define GAMESCREEN_X 50
#define GAMESCREEN_Y 140
#define GAMESCREEN_W 200
#define GAMESCREEN_H 400
#define NEXTSCREEN_X 275
#define NEXTSCREEN_Y 180
#define NEXTSCREEN_W 90
#define NEXTSCREEN_BLANK 100
#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20

#define COMBO_TIME 3
#define BUFSIZE 512
//IP
#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define MY_NICKNAME "LHY"
enum SCENE_INDEX
{
	SCENE_TITLE,
	SCENE_LOBBY,
	SCENE_GAME
};
enum GAME_STAT
{
	WAIT,
	READY,
	RUN,
	GAMEOVER
};
enum BLOCK_TURN
{
	BLOCK_TURN_0,
	BLOCK_TURN_90,
	BLOCK_TURN_180,
	BLOCK_TURN_270,
};
class DuelManager;
struct USERINFO
{
	SOCKET sock;
	char UserName[128];
	DuelManager* pDuelManager;
};