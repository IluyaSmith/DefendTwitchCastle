#pragma once

#include "defines.h"
//#include "Player.h"
#include "Player2.h"
#include "PlayerIO.h"
#include "Enemy.h"
#include "Command.h"
#include "Message.h"
#include "Helper.h"
#include "Waypoint.h"
#include "DefenseBuilding.h"
#include <iostream>
#include <string>
#include <list>
#include <cctype>
#include <cmath>
#include <memory>
#include <thread>
#include <future>
#include <boost\lexical_cast.hpp>

class GameState
{
public:
	GameState();
	~GameState();

	void initialize();
	//void loadWaypoints();

	cost res;

	void resetMessagesSend();
	void incMessagesSend(int _num = 1);
	int getMessagesSend();

	//std::list<Player*> _playerList;
	//std::list<Player*> _activePlayerList;
	
	std::list< std::shared_ptr<Command> > _commandsList;
	std::list< std::shared_ptr<Message> > _messagesToSend;
	std::list< std::shared_ptr<Player2> > _activePlayers;
	std::list< std::shared_ptr<Enemy> > _enemyList;

	std::list< std::shared_ptr<Player2> > _playersThatWantToLookAtStats;


	//Constructed at beginning of programm and used for reference
	std::vector< std::shared_ptr<Waypoint> > _waypoints;
	std::vector< std::shared_ptr<Player2> > _plList;
	std::vector< std::shared_ptr<DefenseBuilding> > _buildingList;

	//Command Handling
	void addCommand(std::string _line, int _cmdpos, std::string _cmdname);
	void workCommands();

	//Automated Statgain and recalc etc
	void updateGameState(sf::Time _timeSiceLastCall);

	//Raiding phase
	void initializeRaid();
	bool isRaidOngoing() { return m_isRaidOngoing; }
	int getTimeTillNextRaid();

	//Voting Stuff
	void startApplicationPhase();
	void addApplicant(std::shared_ptr<Player2> _newAppl);
	void startVotingPhase();
	void endVotingPhase();

	//DrawStuff
	sf::Time getTimeToNextFrame();
	void addTimeToNextFrame(sf::Time _dt);
	void resetTimeToNextFrame();
	//bool isAnimUpdateFrame;

	std::string zauth_token;
private:
	int m_messagesSend;

	int m_wavecount = 0;

	bool m_isRaidOngoing = false;
	sf::Time timeSinceLastRaid = sf::Time::Zero;
	sf::Time updateTimeRaid = sf::seconds(GAME_TIME_BETWEEN_RAIDS);
	bool m_sent_50_Warning = false;
	bool m_sent_0_Warning = false;
	bool m_sent_raid_announcement = false;
	
	sf::Time timeSinceLastVote = sf::Time::Zero;
	sf::Time timeTillEndPhase = sf::seconds(20);
	sf::Time updateTimeVote = sf::seconds(GAME_TIME_BETWEEN_VOTINGS);
	bool m_found_king = false;
	enum votingstate {haveKing,applicationPhase,votingPhase,noKing};
	votingstate m_votingstate = haveKing;
	std::list<std::shared_ptr<Player2>> m_votingApplicants;
	
	sf::Font m_font_azure;

	sf::Texture m_texture_Player;
	sf::Texture m_texture_DestObj;
	sf::Texture m_texture_HpBar;
	sf::Texture m_texture_HpFill;
	//Helper *hlp;
};

