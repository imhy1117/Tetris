#pragma once
#include <Windows.h>
enum PACKET_HEADER
{
	PACKET_HEADER_CONNECT,
	PACKET_HEADER_LOGIN,
	PACKET_HEADER_LOGOUT,
	PACKET_HEADER_READY,
	PACKET_HEADER_STARTGAME,
	PACKET_HEADER_INPUT,
	PACKET_HEADER_ATTACK,
	PACKET_HEADER_GAMEOVER,
	PACKET_HEADER_ROOM_IN,
	PACKET_HEADER_ROOM_OUT,
	PACKET_HEADER_ROOM_INFO,
	PACKET_HEADER_LOBBY_CHAT,
	PACKET_HEADER_LOBBY_INFO,
	PACKET_HEADER_REQUEST_INFO
};
enum CONNECT_INDEX
{
	CONNECT_ACCEPT,
	CONNECT_WAIT
};
enum INPUT_INDEX
{
	UP,
	LEFT,
	RIGHT,
	DOWN,
	SPACE
};
enum ROOM_STAT
{
	ROOM_WAIT,
	ROOM_PLAYING
};
#pragma pack(1)
struct HEADER
{
	WORD name;
	WORD len;
};
struct PACKET_USERINFO
{
	HEADER header;
};
struct PACKET_GAMEOVER
{
	HEADER header;
};
struct PACKET_CONNECT
{
	HEADER header;
};
struct PACKET_LOGIN
{
	HEADER header;
	WORD sock;
	WORD name_len;
};
struct PACKET_ATTACK
{
	HEADER header;
	WORD sock;
	WORD seed;
	WORD line;
};
struct PACKET_LOGOUT
{
	HEADER header;
	WORD sock;
};
struct PACKET_READY
{
	HEADER header;
	WORD ready_info;
	WORD sock;
};
struct PACKET_STARTGAME
{
	HEADER header;
	WORD seed;
};
struct PACKET_ROOM_IN
{
	HEADER header;
	WORD room_num;
};
struct PACKET_ROOM_OUT
{
	HEADER header;
};
struct PACKET_ROOM_INFO
{
	HEADER header;
	WORD sock;
	WORD ready;
	WORD name_len;
};
struct PACKET_LOBBY_INFO
{
	HEADER header;
	WORD room_index;
	WORD room_stat;
	WORD room_playernum;
};
struct PACKET_LOBBY_CHAT
{
	HEADER header;
	WORD sock;
	WORD chat_len;
};
struct PACKET_INPUT
{
	HEADER header;
	WORD input;
	WORD sock;
};
struct PACKET_REQUEST_INFO
{
	HEADER header;
};
#pragma pack()