#pragma once

#include "defines.h"
#include <iostream>

#include <boost\lexical_cast.hpp>

#include <string>
#include <fstream>
#include <list>
#include <memory>
#include <thread>

#include "Player2.h"
#include "DefenseBuilding.h"
#include "Enemy.h"
#include "Helper.h"

#include "GameState.h"

class GameState;

class PlayerIO
{
public:
	PlayerIO();
	~PlayerIO();

	static std::vector< std::shared_ptr<Player2> > loadPlayerListFromFile();
	static std::shared_ptr<Player2> addPlayer(std::string _pname, std::vector< std::shared_ptr<Player2> > &_actualPlList, std::vector< std::shared_ptr<Waypoint>>& _waypoints, sf::Texture &_playertexture, sf::Font& _font);

	static std::shared_ptr<Player2> getPlayer(std::string _pname, std::vector< std::shared_ptr<Player2> > &_actualPlList, std::vector< std::shared_ptr<Waypoint>>& _waypoints, sf::Texture &_playertexture, sf::Font& _font);
	static bool hasPlayer(std::string _pname, std::list< std::shared_ptr<Player2> > &_actualPlList);

	static void savePlayerList(std::vector< std::shared_ptr<Player2> > &_actualPlList);
	static void PlayerIO::savePlayerListSave(std::vector< std::shared_ptr<Player2> >& _actualPlList);

	static void updateEntities(GameState &_gs, sf::Time _timeSinceLastUpdate);
		//std::list< std::shared_ptr<Player2> > &_actualPlList, std::vector< std::shared_ptr<DefenseBuilding> > &_buildings , std::list< std::shared_ptr<Enemy> > &_enemyList, sf::Time _timeSinceLastUpdate);
	static void savePlayerStatsToFile(GameState &_gs);
private:
	static std::shared_ptr<Player2> makePlayerFromString(const std::string _str);
	static std::string PlayerIO::makeStringFromPlayer(const std::shared_ptr<Player2> _pl);
	static void savePlayers(std::vector< std::shared_ptr<Player2> >& _actualPlList);
};

