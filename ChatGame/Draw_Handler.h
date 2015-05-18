#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "GameState.h"



class Draw_Handler
{
public:
	Draw_Handler(GameState &state, sf::RenderWindow &window);
	~Draw_Handler();

	void startRenderLoop();

	void draw(sf::Time timeSinceLastFrame);
	

private:
	void preLoad();

	GameState *m_gs;
	sf::RenderWindow *m_wnd;

	sf::View Castle;
	sf::View Playerlist;
	sf::View PlayerStats;

	sf::Font m_font_robo;
	sf::Font m_font_robo_b;
	sf::Font m_font_azure;

	sf::Sprite m_s_background;
	sf::Sprite m_s_grass;

	sf::Sprite m_s_CastleBridgeMid;
	sf::Sprite m_s_CastleBridgeLeft;
	sf::Sprite m_s_CastleBridgeRight;

	sf::Sprite m_s_CastleTowerMidLeft;
	sf::Sprite m_s_CastleTowerMidRight;

	sf::Sprite m_s_MineEntranceGold;
	sf::Sprite m_s_MineEntranceIron;
	
	std::vector<sf::Sprite*> m_collidables;

	//sf::Sprite m_player;

	sf::Text m_t_playerText;
	sf::Text m_t_playerStats;
	sf::Text m_t_title;
	sf::Text m_t_ttnr;
	sf::Text m_t_res;

	sf::Text* m_ttnr_array[GAME_TIME_BETWEEN_RAIDS+10];
	sf::Text* m_t_plNo[GAME_PLAYERS_MAX];
	float m_t_plNoXoOffset[GAME_PLAYERS_MAX];

	//sf::Texture m_text_tileset_characters;
	sf::Texture m_text_tileset_background;
	sf::Texture m_text_background;
	sf::Texture m_text_tile_grass;
	sf::Texture m_text_collidables;

	sf::Texture m_tmp;
	sf::Texture m_tmp2;

	sf::Texture m_text_CastleBridge;
	sf::Texture m_text_CastleBridgeLeftRight;

	//Test
	sf::Texture m_tex_test;

	
	//draws playernames in a list on the view playerlist
	void draw_Playerlist(sf::Time _tslf);
	sf::Time m_timeTillNextListUpdate = sf::milliseconds(500);
	sf::Time m_timeTillNextListMove = sf::seconds(1.0f / 60.0f);
	std::string m_playerlistString = "";
	int m_playercount = 0;

	//draws all Players depending on vars in gamestate
	void draw_Players();

	//draws all Buildings
	void draw_Buildings();

	void draw_Collidables();

	//draws all Enemies
	void draw_EnemiesOutside();
	void draw_EnemiesInside();

	//drawing of a specific players stats
	sf::Time timeSinceLastStatUpdate = sf::Time::Zero;
	sf::Time updateStat = sf::Time(sf::seconds(5.0f));
	std::shared_ptr<Player2> featuredPlayer = NULL;
	std::string m_statString = "";
	void draw_PlayerStats();

	void resetDrawVars();
};

