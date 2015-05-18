#pragma once

#include <string>
#include <vector>
#include <sstream>

#define WINDOW_SIZE_X 1280
#define WINDOW_SIZE_Y 720

#define VIEW_PLAYERLIST_OFFSET 10
#define VIEW_PLAYERSTATS_OFFSET 200

#define TILESIZE 16
#define TILESIZE_CHAR_X 16
#define TILESIZE_CHAR_Y 18

//LogStates
#define LOG_SEND false
#define LOG_CHAT true
#define LOG_PATHS false
#define LOG_PLAYERS false
#define LOG_PLAYERS_NEW false
#define LOG_LOAD true
#define LOG_SAVE true
#define LOG_IDLEPOINT false
#define LOG_COMMANDS false
#define LOG_COMMAND_VALID true
#define LOG_DT false
#define LOG_WAVESTART true

//File Locations
#define FILE_LOCATION "D:/Data/SFML/"
//#define FILE_LOCATION_CHATLOG_OLD "D:/Data/SFML/oldchatlogs"
#define FILE_LOCATION_CHATLOG "D:/Data/SFML/chatlog.log"
#define INI_LOCATION_STARTUP "D:/Data/SFML/startup.ini"
#define INI_LOCATION_PLAYERS "D:/Data/SFML/players.ini"
#define FILE_LOCATION_PLAYERS "D:/Data/SFML/Chatgame/players.dat"
#define INI_LOCATION_WAYPOINTS "D:/Data/SFML/waypoints.ini"
#define FILE_LOCATION_HTML "D:/Data/Dropbox/Public/TwitchCastle/playerstats.htm"

//IRC Stuff
#define IRC_OPERATING_CHANNEL "#iluyasmith"
#define IRC_MAX_MSG_PER_30 20
#define IRC_USE true

//Rendervals
#define GFX_TEXT_PLAYERLIST_FONT_SIZE 10
#define GFX_TEXT_PLAYERNUMBER_FONT_SIZE 11 

#define GFX_ANIMATION_TIME_BETWEEN_STATES 150
#define GFX_ANIMATION_STATE_RUN1 0
#define GFX_ANIMATION_STATE_STAND 1
#define GFX_ANIMATION_STATE_RUN2 2

#define GFX_CLASS_NONE_X 32
#define GFX_CLASS_NONE_Y 72
#define GFX_CLASS_WARRIOR_X 16
#define GFX_CLASS_WARRIOR_Y 180

//Waypoints
#define WP_AREA 0
#define WP_PATH 1

#define WP_COUNT_AREA 16
#define WP_COUNT_PATHS 15
#define WP_COUNT_ENEMY_WPS 9

//Startvalues
#define PLAYER_SVAL_XP 0
#define PLAYER_SVAL_LVL 1
#define PLAYER_SVAL_STR 50
#define PLAYER_SVAL_DEX 50
#define PLAYER_SVAL_MAT 50
#define PLAYER_SVAL_BTY 50

#define PLAYER_SVAL_MS 1.5f
#define PLAYER_SVAL_CLASS "none"

#define ENEMY_SVAL_MS 2.0f

//Gamesettings
#define GAME_TIME_BETWEEN_RAIDS 180 //sec
#define GAME_TIME_BETWEEN_VOTINGS 600 //sec
#define GAME_PLAYERS_MAX 150

//Replymessages
#define MSG_HELP_MSG "Commands: !oath to join || !betray to leave || !train <str/dex/mat/bty> || !work <gold/iron/wood/stone> || !rep <lg/mg/rg> || !fight <lg/mg/rg>"

struct cost{
	float gold = 0;
	float iron = 0;
	float stone = 0;
	float wood = 0;
};