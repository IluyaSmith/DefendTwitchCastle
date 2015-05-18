#include "stdafx.h"
#include "Draw_Handler.h"


Draw_Handler::Draw_Handler(GameState &state, sf::RenderWindow &window)
{
	this->m_gs = &state;
	this->m_wnd = &window;

	preLoad();
}


Draw_Handler::~Draw_Handler()
{
}


/*		As i'm not the best with gfx (yet), i will rely on ressources that are freely available for the moment..
*		
*		Character Tileset - http://opengameart.org/content/twelve-16x18-rpg-sprites-plus-base License: CC-BY 3.0 (16x16)
*		Going with a basic 16x16 Tileset for now - Credits: http://opengameart.org/content/16x16-town-remix License: CC-BY 3.0 (16x18)
*		
*		http://www.dafont.com/de/pixel-azure-bonds.font License: free for non-commercial (http://www.pixelsagas.com/?download=pixel-azure-bonds)
*		http://www.google.com/fonts/specimen/Roboto+Slab License: Apache 2.0
//*/

void Draw_Handler::startRenderLoop()
{

	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Time updateRate = sf::seconds(1.0f / 60.0f);

	while (m_wnd->isOpen())
	{
		timeSinceLastUpdate += clock.restart();
		if (timeSinceLastUpdate >= updateRate)
		{
			timeSinceLastUpdate -= updateRate;
			draw(timeSinceLastUpdate);
		}
	}
}

//Main Render Loading
void Draw_Handler::preLoad()
{
	//Texts
	m_font_robo.loadFromFile("./assets/RobotoSlab-Regular.ttf");
	m_font_robo_b.loadFromFile("./assets/RobotoSlab-Bold.ttf");
	m_font_azure.loadFromFile("./assets/Pixel Azure Bonds.otf");


	m_t_playerText.setCharacterSize(GFX_TEXT_PLAYERLIST_FONT_SIZE);
	m_t_playerText.setFont(m_font_azure);
	m_t_playerText.setPosition(WINDOW_SIZE_X + VIEW_PLAYERLIST_OFFSET + 10, 40);
	m_t_playerText.setColor(sf::Color(213, 213, 213, 255));

	m_t_playerStats.setCharacterSize(GFX_TEXT_PLAYERLIST_FONT_SIZE);
	m_t_playerStats.setFont(m_font_azure);
	m_t_playerStats.setPosition(WINDOW_SIZE_X + VIEW_PLAYERSTATS_OFFSET + 5, 40);
	m_t_playerStats.setColor(sf::Color(213, 213, 213, 255));
	
	m_t_title.setString("Defend TwitchCastle");
	m_t_title.setFont(m_font_azure);
	m_t_title.setCharacterSize(25);
	m_t_title.setColor(sf::Color::Black);
	m_t_title.setPosition(m_wnd->getSize().x / 2 - m_t_title.getLocalBounds().width / 2, 40);

	m_t_ttnr.setString("The Castle is under attack!");
	m_t_ttnr.setFont(m_font_azure);
	m_t_ttnr.setCharacterSize(22);
	m_t_ttnr.setColor(sf::Color::Black);
	m_t_ttnr.setPosition(m_wnd->getSize().x / 2 - m_t_ttnr.getLocalBounds().width / 2, 80);

	// Set up TTNR Preload
	for (int i = 0; i < GAME_TIME_BETWEEN_RAIDS+10; i++)
	{
		m_ttnr_array[i] = new sf::Text;
		m_ttnr_array[i]->setString("Next Wave in " + std::to_string(i) + " seconds.");
		m_ttnr_array[i]->setFont(m_font_azure);
		m_ttnr_array[i]->setCharacterSize(22);
		m_ttnr_array[i]->setColor(sf::Color::Black);
		m_ttnr_array[i]->setPosition(m_wnd->getSize().x / 2 - m_ttnr_array[i]->getLocalBounds().width / 2, 80);
	}

	//Set up PlayerNo Preload m_t_plNo
	for (int i = 0; i <= GAME_PLAYERS_MAX; i++)
	{
		m_t_plNo[i] = new sf::Text;
		m_t_plNo[i]->setString(std::to_string(i));
		m_t_plNo[i]->setFont(m_font_azure);
		m_t_plNo[i]->setCharacterSize(GFX_TEXT_PLAYERNUMBER_FONT_SIZE);
		m_t_plNo[i]->setColor(sf::Color::Black);

		m_t_plNoXoOffset[i] = TILESIZE_CHAR_X/2 - (m_t_plNo[i]->getLocalBounds().width / 2);
	}

	m_t_res.setFont(m_font_azure);
	m_t_res.setCharacterSize(14);
	m_t_res.setColor(sf::Color(213,213,213,255));

	//Texture::Tileset
	//m_text_tileset_background.loadFromFile("./assets/16oga.png");

	m_text_tile_grass.loadFromFile("./assets/background_grass.png");
	m_text_tile_grass.setRepeated(true);

	m_text_background.loadFromFile("./assets/background.png");

	m_text_collidables.loadFromFile("./assets/collidables.png");

	m_tmp.loadFromFile("./assets/mineentrancegold.png");
	m_tmp2.loadFromFile("./assets/mineentranceiron.png");

	m_text_CastleBridge.loadFromFile("./assets/gatebrigde.png");
	m_text_CastleBridgeLeftRight.loadFromFile("./assets/gatebrigdeLeftRight.png");

	//Test
	m_tex_test.loadFromFile("./assets/test.png");

	//m_text_tileset_characters.loadFromFile("./assets/charsets_12_m-f_complete_by_antifarea.png");



	//Sprites
	m_s_grass.setTexture(m_text_tile_grass);
	m_s_grass.setPosition(0, 0);
	m_s_grass.setTextureRect(sf::IntRect(0,0,m_wnd->getSize().x, m_wnd->getSize().y));

	m_s_background.setTexture(m_text_background);
	m_s_background.setPosition(0, 0);

	//Collidable Sprites
	m_s_CastleBridgeMid.setTexture(m_text_collidables); 
	m_s_CastleBridgeMid.setTextureRect(sf::IntRect(166, 0, 128, 48));
	m_s_CastleBridgeMid.setPosition(sf::Vector2f(576,566));

	m_s_CastleBridgeLeft.setTexture(m_text_CastleBridgeLeftRight);
	m_s_CastleBridgeLeft.setPosition(sf::Vector2f(127, 561));
	m_s_CastleBridgeRight.setTexture(m_text_CastleBridgeLeftRight);
	m_s_CastleBridgeRight.setPosition(sf::Vector2f(1041, 561));

	m_s_CastleTowerMidLeft.setTexture(m_text_collidables);
	m_s_CastleTowerMidLeft.setTextureRect(sf::IntRect(0, 0, 80, 72));
	m_s_CastleTowerMidLeft.setPosition(sf::Vector2f(496, 542));
	m_s_CastleTowerMidRight.setTexture(m_text_collidables);
	m_s_CastleTowerMidRight.setTextureRect(sf::IntRect(83, 0, 80, 72));
	m_s_CastleTowerMidRight.setPosition(sf::Vector2f(704, 542));

	m_s_MineEntranceGold.setTexture(m_tmp);
	m_s_MineEntranceGold.setPosition(0,444);

	m_s_MineEntranceIron.setTexture(m_tmp2);
	m_s_MineEntranceIron.setPosition(283, 432);

	m_collidables.push_back(&m_s_CastleTowerMidLeft);
	m_collidables.push_back(&m_s_CastleTowerMidRight);

	m_collidables.push_back(&m_s_MineEntranceGold);
	m_collidables.push_back(&m_s_MineEntranceIron);



	

	//Main View
	Castle.reset(sf::FloatRect(0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y));

	//Constructing the view on a place outside of the usual view
	Playerlist.reset(sf::FloatRect(WINDOW_SIZE_X + VIEW_PLAYERLIST_OFFSET + 10, 40, 150, 240));
	//actual place it takes on the screen 
	Playerlist.setViewport(sf::FloatRect(10.0f / WINDOW_SIZE_X, 40.0f / WINDOW_SIZE_Y,
		150.0f / WINDOW_SIZE_X, 240.0f / WINDOW_SIZE_Y));

	PlayerStats.reset(sf::FloatRect(WINDOW_SIZE_X + VIEW_PLAYERSTATS_OFFSET, 40, 140, 100));
	PlayerStats.setViewport(sf::FloatRect((WINDOW_SIZE_X - 10.0f - 140.0f) / WINDOW_SIZE_X,
		40.0f / WINDOW_SIZE_Y, 140.0f / WINDOW_SIZE_X, 100.0f / WINDOW_SIZE_Y));

}

//Main Render Loop
void Draw_Handler::draw(sf::Time timeSinceLastFrame)
{

	

	/*		Positions: Relatives
	*		m_wnd->getSize().x / 2
	*		txt.getLocalBounds().width / 2
	*		m_wnd->getSize().x / 2 - txt.getLocalBounds().width / 2
	//*/

	timeSinceLastStatUpdate += timeSinceLastFrame;

	m_wnd->clear();

	//Main View
	m_wnd->setView(Castle);

	m_wnd->draw(m_s_grass);
	m_wnd->draw(m_s_background);

	draw_EnemiesInside();

	//gates
	m_wnd->draw(m_s_CastleBridgeMid);
	m_wnd->draw(m_s_CastleBridgeLeft); 
	m_wnd->draw(m_s_CastleBridgeRight);

	

	draw_Buildings();

	draw_EnemiesOutside();

	draw_Players();

	draw_Collidables();
	
	
	m_wnd->draw(m_t_title);

	if (m_gs->isRaidOngoing())
	{
		m_wnd->draw(m_t_ttnr);
	}
	else
	{
		m_wnd->draw(*(m_ttnr_array[m_gs->getTimeTillNextRaid()]));
	}


	m_t_res.setString(std::to_string((int)m_gs->res.gold));
	m_t_res.setPosition(sf::Vector2f(247, 6));
	m_wnd->draw(m_t_res);
	m_t_res.setString(std::to_string((int)m_gs->res.iron));
	m_t_res.setPosition(sf::Vector2f(340, 6));
	m_wnd->draw(m_t_res);
	m_t_res.setString(std::to_string((int)m_gs->res.stone));
	m_t_res.setPosition(sf::Vector2f(929, 6));
	m_wnd->draw(m_t_res);
	m_t_res.setString(std::to_string((int)m_gs->res.wood));
	m_t_res.setPosition(sf::Vector2f(1020, 6));
	m_wnd->draw(m_t_res);


	//Testing stuff
	/**
	sf::Text m_plnumber;
	m_plnumber.setFont(m_font_azure);
	m_plnumber.setCharacterSize(12);
	m_plnumber.setString("0");
	sf::Glyph bla = m_font_azure.getGlyph(L'0', 20, false);
	sf::Sprite m_test;
	m_test.setTexture(m_tex_test);
	m_test.setTextureRect(sf::IntRect(16, 48, 16, 24));
	m_test.setPosition(500, 500);

	m_wnd->draw(m_test);
	//*/

	//Playerlist View
	m_wnd->setView(Playerlist);
	draw_Playerlist(timeSinceLastFrame);


	//Playerstats View
	m_wnd->setView(PlayerStats);
	draw_PlayerStats();


	//Display Everything
	m_wnd->display();

	//'Reset Draw Vars
	resetDrawVars();

}

void Draw_Handler::draw_Playerlist(sf::Time _tslf)
{
	//renew Active Playerlist every 500ms
	m_timeTillNextListUpdate -= _tslf;
	if (m_timeTillNextListUpdate <= sf::Time::Zero)
	{
		m_timeTillNextListUpdate = sf::milliseconds(500);
		m_playerlistString = "";
		m_playercount = 0;
		for (const auto &pl : m_gs->_activePlayers)
		{
			//std::cout << item->getName() << std::endl;
			m_playerlistString += "[" + std::to_string(pl->getId()) + "] " + pl->getPlayername() +
				" (" + std::to_string((int)pl->getXp()) + ")" + '\n';
			m_playercount++;
		}

		if (m_playercount > 17)
		{
			m_playerlistString  += m_playerlistString;
		}
		m_t_playerText.setString(m_playerlistString);
	}

	if (m_playercount > 17)
	{
		m_timeTillNextListMove -= _tslf;
		if (m_timeTillNextListMove <= sf::Time::Zero)
		{
			m_timeTillNextListMove = sf::seconds(1.0f / 40.0f);
			m_t_playerText.move(0, -0.5f);
		}

		if ((m_t_playerText.getPosition().y + m_t_playerText.getLocalBounds().height / 2) < 40)
		{
			m_t_playerText.setPosition(WINDOW_SIZE_X + VIEW_PLAYERLIST_OFFSET + 10, 38);
		}
		//*/
		m_wnd->draw(m_t_playerText);
	}
	else
	{
		m_wnd->draw(m_t_playerText);
	}

}

void Draw_Handler::draw_Players()
{
	//int counter = 0;
	//sf::Vector2f playerpos;

	for (const auto &pl : m_gs->_activePlayers)
	{
		int playerId = pl->getId();

		//print player
		m_wnd->draw(pl->getSprite());

		//print playernumber
		m_t_plNo[playerId]->setPosition(pl->getPos() + sf::Vector2f(m_t_plNoXoOffset[playerId], -8));
		m_wnd->draw(*(m_t_plNo[playerId]));

		//counter += 1;
	}

}

void Draw_Handler::draw_Buildings()
{
	for (const auto &b : m_gs->_buildingList)
	{
		m_wnd->draw(b->getSprite());
		m_wnd->draw(b->getHpBar());
		m_wnd->draw(b->getHPFill());
		m_wnd->draw(b->getHpText());
	}
}

void Draw_Handler::draw_Collidables()
{
	for (const auto &b : m_collidables)
	{
		/*for (const auto &pl : m_gs->_activePlayers)
		{
			if (!pl->m_isInside)
			{//none has drawn him yet
				//collision
				if (pl->getSprite().getGlobalBounds().intersects(b->getGlobalBounds()))
				{
					//std::cout << "intersects" << std::endl;
					m_wnd->draw(pl->getSprite());
					pl->m_isInside = true;
				}
			}
		}//*/
		m_wnd->draw(*b);
	}
}

void Draw_Handler::draw_EnemiesOutside()
{

	for (const auto &ene : m_gs->_enemyList)
	{
		//print enemy
		if (!ene->isInside())
		{
			m_wnd->draw(ene->getSprite());
			m_wnd->draw(ene->getHpBar());
			m_wnd->draw(ene->getHpFill());
		}
	}

}

void Draw_Handler::draw_EnemiesInside()
{

	for (const auto &ene : m_gs->_enemyList)
	{
		//print enemy
		if (ene->isInside())
		{
			m_wnd->draw(ene->getSprite());
			m_wnd->draw(ene->getHpBar());
			m_wnd->draw(ene->getHpFill());
		}
	}

}

void Draw_Handler::draw_PlayerStats()
{
	if (timeSinceLastStatUpdate > updateStat)
	{
		timeSinceLastStatUpdate -= updateStat;
	//	std::cout << "draw next players stats" << std::endl;
		if (!m_gs->_activePlayers.empty())
		{
			if (!m_gs->_playersThatWantToLookAtStats.empty())
			{
				featuredPlayer = m_gs->_playersThatWantToLookAtStats.front();
				m_gs->_playersThatWantToLookAtStats.pop_front();
			}
			else
			{
				//get Random Player....
				int rnn = std::rand() % m_gs->_activePlayers.size();
				int cnt = 0;
				for (const auto &pl : m_gs->_activePlayers)
				{
					if (cnt == rnn)
					{
						featuredPlayer = pl;
						break;
					}
					cnt++;
				}
			}

			//got new featured Player - make String
			m_statString = featuredPlayer->getPlayername() + "\n ";
			m_statString += "[Level] " + std::to_string((int)featuredPlayer->getLvl()) + "\n ";
			m_statString += "[Xp] " + std::to_string((int)featuredPlayer->getXp()) + "\n ";
			m_statString += "[Strength] " + std::to_string((int)featuredPlayer->getStr()) + "\n ";
			m_statString += "[Dexterity] " + std::to_string((int)featuredPlayer->getDex()) + "\n ";
			m_statString += "[Math] " + std::to_string((int)featuredPlayer->getMat()) + "\n ";
			m_statString += "[Beauty] " + std::to_string((int)featuredPlayer->getBty()) + "\n ";
			m_t_playerStats.setString(m_statString);
		}
	}

	if (featuredPlayer != NULL)
	{
		//std::cout << "FPlayer: " << featuredPlayer->getPlayername() << std::endl;
		//draw Playerstats in ouside View

		m_wnd->draw(m_t_playerStats);
	}


}

void Draw_Handler::resetDrawVars()
{
	for (auto &pl : m_gs->_activePlayers)
	{
		pl->m_isInside = false;
	}
}