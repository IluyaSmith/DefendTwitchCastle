#include "stdafx.h"
#include "PlayerIO.h"


PlayerIO::PlayerIO()
{
}


PlayerIO::~PlayerIO()
{
}


std::vector< std::shared_ptr<Player2> > PlayerIO::loadPlayerListFromFile()
{
	if (LOG_LOAD) std::cout << "Loading players...";

	std::vector< std::shared_ptr< Player2 > > l;

	std::ifstream fs(FILE_LOCATION_PLAYERS);
	std::string line;
	int counter = 0;
	while (std::getline(fs, line)) 
	{
		if (!line.empty())
		{	//skip empty lines
			if (!(line[0] == ';'))
			{	//skip comment lines
				std::shared_ptr<Player2> p = PlayerIO::makePlayerFromString(line);
				//p->setId(counter);
				p->setIsSaved(true);
				l.push_back(p);
				++counter;
			}
		}	
	}

	if (LOG_LOAD) std::cout << "Finished loading " << counter << " players...";
	return l;
}


std::shared_ptr<Player2> PlayerIO::makePlayerFromString(const std::string _str)
{
	std::vector<std::string> s = Helper::split(_str, ':');
	//;            xp    lvl   str   dex   mat   bty
	//iluyasbot : 2000 : 100 : 500 : 500 : 500 : 500
	//    0         1     2     3     4     5     6
	using boost::lexical_cast;

	//
	//	BASE LOADING POINT - CHANGING OF SAVEFILES HERE 
	//

	return std::make_shared<Player2>(s[0], lexical_cast<float>(s[1]), lexical_cast<float>(s[2]),
		lexical_cast<float>(s[3]), lexical_cast<float>(s[4]), lexical_cast<float>(s[5]), lexical_cast<float>(s[6]));
}

std::string PlayerIO::makeStringFromPlayer(const std::shared_ptr<Player2> _pl)
{

	//
	//	BASE SAVE POINT - CHANGING OF SAVEFILES HERE 
	//

	std::string s = _pl->getPlayername();
	s += ':' + std::to_string(_pl->getXp());
	s += ':' + std::to_string(_pl->getLvl());
	s += ':' + std::to_string(_pl->getStr());
	s += ':' + std::to_string(_pl->getDex());
	s += ':' + std::to_string(_pl->getMat());
	s += ':' + std::to_string(_pl->getBty());

	return s;
}

//Only method to initialize a player! returns the pointer to the player.
std::shared_ptr<Player2> PlayerIO::addPlayer(std::string _pname, std::vector< std::shared_ptr<Player2> >& _actualPlList, std::vector< std::shared_ptr<Waypoint>>& _waypoints, sf::Texture &_playertexture, sf::Font& _font)
{
	//look if Player is already in List
	bool foundPlayer = false;
	for(const auto &it : _actualPlList)
	{
		if (it->getPlayername() == _pname)
		{
			foundPlayer = true;
			//std::cout << "Found Player " << it->toString() << std::endl;
			return it;
		}
	} 

	//Add if not found
	if (!foundPlayer)
	{

		//
		//	BASE LOADING POINT - CHANGING OF SAVEFILES HERE 
		//

		std::string playerstring = _pname;
		playerstring += ':' + std::to_string(PLAYER_SVAL_XP);
		playerstring += ':' + std::to_string(PLAYER_SVAL_LVL);
		playerstring += ':' + std::to_string(PLAYER_SVAL_STR);
		playerstring += ':' + std::to_string(PLAYER_SVAL_DEX);
		playerstring += ':' + std::to_string(PLAYER_SVAL_MAT);
		playerstring += ':' + std::to_string(PLAYER_SVAL_BTY);



		//std::cout << "Add Player: \"" << playerstring << "\"" << std::endl;
		std::shared_ptr<Player2> newPlayer = PlayerIO::makePlayerFromString(playerstring);
		newPlayer->setId(_actualPlList.size() - 1);
		newPlayer->init(_waypoints[0], _playertexture, _font);
		_actualPlList.push_back(newPlayer);
		return newPlayer;
	}
	return NULL;
}

std::shared_ptr< Player2 > PlayerIO::getPlayer(std::string _pname, std::vector< std::shared_ptr<Player2> >& _actualPlList, std::vector< std::shared_ptr<Waypoint>>& _waypoints, sf::Texture& _playertexture, sf::Font& _font)
{
	return addPlayer(_pname, _actualPlList,_waypoints, _playertexture, _font);
}

void PlayerIO::savePlayerList(std::vector< std::shared_ptr<Player2> >& _actualPlList)
{
	std::thread t(&savePlayers, _actualPlList);
	t.detach();
}
void PlayerIO::savePlayerListSave(std::vector< std::shared_ptr<Player2> >& _actualPlList)
{
	std::thread t(&savePlayers, _actualPlList);
	t.join();
}

void PlayerIO::savePlayers(std::vector< std::shared_ptr<Player2> >& _actualPlList)
{
	std::ofstream fs(FILE_LOCATION_PLAYERS, std::fstream::trunc);
	std::string line;
	if (LOG_SAVE) std::cout << "Saving players...";
	int cnt = 0;
	for (const auto &it : _actualPlList)
	{
		//std::cout << "Saving " << it->toString() << std::endl;
		line = makeStringFromPlayer(it) + '\n';		
		fs.write(line.c_str(),line.size());
		++cnt;
	}
	if (LOG_SAVE) std::cout << "saved " << cnt << " players..." << std::endl;
}

bool PlayerIO::hasPlayer(std::string _pname, std::list< std::shared_ptr<Player2> > &_actualPlList)
{
	for (const auto &pl : _actualPlList)
	{
		if (pl->getPlayername() == _pname)
		{
			return true;
		}
	}
	return false;
}

//void PlayerIO::updateEntities(std::list< std::shared_ptr<Player2> > &_actualPlList, std::vector< std::shared_ptr<DefenseBuilding> > &_buildings, std::list< std::shared_ptr<Enemy> > &_enemyList, sf::Time _timeSinceLastUpdate)
void PlayerIO::updateEntities(GameState &_gs, sf::Time _timeSinceLastUpdate)
{
	int counter = 0;
	for (const auto &pl : _gs._activePlayers)
	{
		++counter;
		pl->setId(counter);

		pl->update();

		if (pl->hasJobFinished())
		{
			switch (pl->getJob())
			{
			case Player2::Job::gold:
				_gs.res.gold += pl->takeJobEarnings();
				break;
			case Player2::Job::wood:
				_gs.res.wood += pl->takeJobEarnings();
				break;
			case Player2::Job::iron:
				_gs.res.iron += pl->takeJobEarnings();
				break;
			case Player2::Job::stone:
				_gs.res.stone += pl->takeJobEarnings();
				break;
			default:
				std::cout << "Finished Job without right job" << std::endl;
			}
		}
	}

	for (const auto &b : _gs._buildingList)
	{
		b->update(_timeSinceLastUpdate);
	}

	std::vector<std::shared_ptr<Enemy>> enemiesToRemove;
	for (const auto &en : _gs._enemyList)
	{
		en->update(_timeSinceLastUpdate);

		if (en->isDone())
		{
			enemiesToRemove.push_back(en);
		}

	}

	while (!enemiesToRemove.empty())
	{
		_gs._enemyList.remove(enemiesToRemove.back());
		enemiesToRemove.pop_back();
	}
}

void PlayerIO::savePlayerStatsToFile(GameState& _gs)
{

	if (LOG_SAVE) std::cout << "Making HTML for players...";

	std::ofstream fs(FILE_LOCATION_HTML, std::fstream::trunc);
	std::string line = 
		"<html><head><title> Defend TwitchCastle Players</title>"
		"<meta http-equiv=refresh content=30>"
		"<link rel=stylesheet href=mainstyle.css type=text/css /></head>"
		"<body> <p> This page is regenerated every 30 seconds </p>"
		"<div><div class=wrap>";

	std::list<std::string> plnames;

	for (const auto &pl : _gs._activePlayers)
	{
		//std::cout << pl.m_lvl << std::endl;
		line += "<div class=player>";
		line += "<div class=name><a id=" + pl->getPlayername() + ">" + pl->getPlayername() + "</a>"
				"<img src=charClassless.png class=profilepic> </div>";
		line += "<div class=stat> ";
		line += "<table>";
		line += "<tr><td>Level: </td><td>" + std::to_string((int)pl->getLvl()) + "</td></tr>";
		line += "<tr><td>Strength: </td><td>" + std::to_string((int)pl->getStr()) + "</td></tr>";
		line += "<tr><td>Dexterity: </td><td>" + std::to_string((int)pl->getDex()) + "</td></tr>";
		line += "<tr><td>Maths: </td><td>" + std::to_string((int)pl->getMat()) + "</td></tr>";
		line += "<tr><td>Beauty: </td><td>" + std::to_string((int)pl->getBty()) + "</td></tr>";
		line += "</table></div></div>";

		plnames.push_back(pl->getPlayername());
	}

	fs.write(line.c_str(), line.size());

	plnames.sort();
	line = "</div><div class=lookuplist>";
	//Scroll to quickstuff
	for (const auto &ns : plnames)
	{
		line += "<a href=#" + ns + ">" + ns + "</a><br/>";
	}
	line += "</div></div></body></html>";
	fs.write(line.c_str(), line.size());
}