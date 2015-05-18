#include "stdafx.h"
#include "GameState.h"


GameState::GameState()
{
}


GameState::~GameState()
{
}

void GameState::initialize()
{
	std::future<std::vector< std::shared_ptr<Waypoint> >> wp_res = std::async(Waypoint::loadWaypoints);
	std::future<std::vector< std::shared_ptr<Player2> >> pl_res = std::async(PlayerIO::loadPlayerListFromFile);


	this->m_font_azure.loadFromFile("./assets/Pixel Azure Bonds.otf");

	this->m_texture_Player.loadFromFile("./assets/charsets_12_m-f_complete_by_antifarea.png");
	this->m_texture_DestObj.loadFromFile("./assets/destObj.png");
	this->m_texture_HpBar.loadFromFile("./assets/hp_bar.png");
	this->m_texture_HpFill.loadFromFile("./assets/hp_fill.png");

	//create Walls and Gates
	std::shared_ptr<DefenseBuilding> mainGate = std::make_shared<DefenseBuilding>("repair-Gate-Middle", 200.0f);
	mainGate->init(DefenseBuilding::type::gate, sf::FloatRect(612, 614, 56, 47), m_texture_DestObj, m_texture_HpBar, m_texture_HpFill,m_font_azure);
	this->_buildingList.push_back(mainGate);

	std::shared_ptr<DefenseBuilding> leftGate = std::make_shared<DefenseBuilding>("repair-Gate-Left", 200.0f);
	leftGate->init(DefenseBuilding::type::gate, sf::FloatRect(155, 597, 56, 47), m_texture_DestObj, m_texture_HpBar, m_texture_HpFill, m_font_azure);
	this->_buildingList.push_back(leftGate);

	std::shared_ptr<DefenseBuilding> rightGate = std::make_shared<DefenseBuilding>("repair-Gate-Right", 200.0f);
	rightGate->init(DefenseBuilding::type::gate, sf::FloatRect(1069, 597, 56, 47), m_texture_DestObj, m_texture_HpBar, m_texture_HpFill, m_font_azure);
	this->_buildingList.push_back(rightGate);

/*	std::shared_ptr<DefenseBuilding> leftWall = std::make_shared<DefenseBuilding>("repair-Wall-Left", 100.0f);
	leftWall->init(DefenseBuilding::type::gate, sf::FloatRect(390, 614, 56, 47), m_texture_DestObj, m_texture_HpBar, m_texture_HpFill);
	this->_buildingList.push_back(leftWall);	

	std::shared_ptr<DefenseBuilding> rightWall = std::make_shared<DefenseBuilding>("repair-Wall-Right", 100.0f);
	rightWall->init(DefenseBuilding::type::gate, sf::FloatRect(700, 614, 56, 47), m_texture_DestObj, m_texture_HpBar, m_texture_HpFill);
	this->_buildingList.push_back(rightWall);
//*/

	this->m_messagesSend = 0;

	this->_waypoints = wp_res.get();
	this->_plList = pl_res.get();

	//Link WP with Players
	for (const auto &pl : this->_plList)
	{
		pl->init(this->_waypoints[0], this->m_texture_Player, this->m_font_azure);
	}

}


////////
void GameState::resetMessagesSend() { this->m_messagesSend = 0; }
void GameState::incMessagesSend(int _num) { this->m_messagesSend += _num; }
int GameState::getMessagesSend() { return this->m_messagesSend; }

//Takes a Chatline, the corresponding cmd-Tag and its position
void GameState::addCommand(std::string _line, int _cmdpos, std::string _cmdname)
{

	std::vector<std::string> cmdargs = Helper::split(_line.substr(_cmdpos + 2, std::string::npos-1), ' ');
	std::string uname = _line.substr(1, _line.find_first_of("!") - 1);

	//Debug !cmd 
	if (_cmdname == "cmd" && (uname == "iluyasmith" || uname=="iluyasbot") ) 
	{
		//delete !cmd
		cmdargs.erase(cmdargs.begin());
		if (cmdargs[0] == "u")
		{	//!cmd u Iwantcmdforthisuser !oath
			cmdargs.erase(cmdargs.begin());
			uname = cmdargs[0];
			cmdargs.erase(cmdargs.begin());
		}
		if (cmdargs[0] == "uu")
		{
			std::vector<std::string> tmp_args = { "oath" };
			for (int j = 0; j < boost::lexical_cast<int>(cmdargs[1]); ++j)
			{
				uname = "bot" + to_string(j);
				std::string tmp_cmdname = "oath";
				this->_commandsList.push_back(Command::cmdFactory(uname, tmp_cmdname, tmp_args));
			}
			return;
		}
		cmdargs[0] = cmdargs[0].substr(1, cmdargs[0].size());
		//this->_commandsList.push_back(Command::cmdFactory(uname, _cmdname, cmdargs));
		std::cout << "Command command from: " << uname << ". Args: ";
		for (size_t i = 0; i < cmdargs.size(); ++i)
		{
			std::cout << "[" << i << "]" << cmdargs[i] <<  " ";
		}
		this->_commandsList.push_back(Command::cmdFactory(uname, cmdargs[0], cmdargs));
	}
	else
	{
		this->_commandsList.push_back(Command::cmdFactory(uname, _cmdname, cmdargs));
	}
}

void GameState::workCommands()
{
	using boost::lexical_cast;
	using boost::bad_lexical_cast;

	while (!this->_commandsList.empty())
	{	/*
		cmdpos = line.find(":!hello");
		if (cmdpos != std::string::npos) { m_gs->addCommand(line, cmdpos, "hello"); }

		//debug command for me !add <stat> <amount> <player if another>
		cmdpos = line.find(":!add");
		if (cmdpos != std::string::npos) { m_gs->addCommand(line, cmdpos, "add"); }

		cmdpos = line.find(":!wp");
		if (cmdpos != std::string::npos) { m_gs->addCommand(line, cmdpos, "wp"); }

		cmdpos = line.find(":!oath");
		if (cmdpos != std::string::npos) { m_gs->addCommand(line, cmdpos, "oath"); }

		cmdpos = line.find(":!work");
		if (cmdpos != std::string::npos) { m_gs->addCommand(line, cmdpos, "work"); }

		cmdpos = line.find(":!train");
		if (cmdpos != std::string::npos) { m_gs->addCommand(line, cmdpos, "train"); }

		cmdpos = line.find(":!betray");
		if (cmdpos != std::string::npos) { m_gs->addCommand(line, cmdpos, "betray"); }
		//*/
		Command* cmd = this->_commandsList.front().get();
		std::shared_ptr<Player2> pl = PlayerIO::getPlayer(cmd->getPlayername(), this->_plList, this->_waypoints,this->m_texture_Player,this->m_font_azure);
		if (LOG_COMMANDS) { std::cout << "[DoCmd] " << cmd->getCommandname() << " for " << cmd->getPlayername() << std::endl; }

		//automated messages!
		if (cmd->getCommandname() == "hello") {
			std::string pname = cmd->getPlayername();
			std::string txt = cmd->getPlayername() + " joined!";
			std::string cmdname = cmd->getCommandname();
			this->_messagesToSend.push_back(Message::msgFactory(pname,cmdname,txt));
		}

		if (cmd->getCommandname() == "bye") {
			std::string cmdname = "betray";
			std::string uname = cmd->getPlayername();
			std::vector<std::string> cmdargs{ "betray" };
			this->_commandsList.push_back(Command::cmdFactory(uname, cmdname, cmdargs));
		}


		//test if command is command alone (i.e. !adddded should not be handled)
		if (cmd->getCommandname() == cmd->getArgs()[0])
		{
			if (LOG_COMMAND_VALID) { std::cout << "cmd " << cmd->getArgs()[0] << " is valid." << std::endl; }
		}
		else
		{	//Throw away else
			if (LOG_COMMAND_VALID) { std::cout << "cmd " << cmd->getArgs()[0] << " was thrown away!" << std::endl; }
			_commandsList.pop_front();
			continue;
		}
		//commands
		if (cmd->getCommandname() == "add") 
		{	//debugcmd only!
			//should be: [0]add [1]attrib [2]amount [3]player(optional)
			if (cmd->getPlayername() == "iluyasmith" || cmd->getPlayername() == "iluyasbot")
			{
				for (size_t i = 0; i < cmd->getArgs().size(); i++)
				{
					std::cout << "getArgs(): " << cmd->getArgs()[i];
					for (auto &b : _buildingList)
					{
						b->LevelUp();
					}
				}
			}
		}
		else if (cmd->getCommandname() == "wp")
		{
			
			int bla = -100;
			//get waypoint and verify its valid
			if (cmd->getArgs().size() == 2)
			{	//!wp <wp_id>
				try
				{
					bla = lexical_cast<int>(cmd->getArgs()[1]);
				}
				catch (const bad_lexical_cast &)
				{
					if (LOG_COMMAND_VALID) { std::cout << "bad argumented wp from " << cmd->getPlayername() << std::endl; }
					_commandsList.pop_front();
					continue;
				}

				if (bla < (WP_COUNT_AREA + WP_COUNT_PATHS ) )
				{	//push new list
					std::list<int> wp = pl->getNextWaypoint()->pathTo(bla);
					std::list<std::shared_ptr<Waypoint>> tmp;
					while (!wp.empty())
					{
						tmp.push_back(this->_waypoints[wp.front()]);
						wp.pop_front();
					}
					pl->setWayToGo(tmp);
				}
				else
				{
					std::cout << "there is no wp " << bla << " " << cmd->getPlayername() << std::endl;
				}

			}
			else if (cmd->getArgs().size() == 1)
			{	//!wp
				std::cout << "Player " << cmd->getPlayername() << " is on his way to WP" << pl->getNextWaypoint()->getId() << std::endl;
			}
		}
		else if (cmd->getCommandname() == "vote")
		{
			if (cmd->getArgs().size() == 2)
			{	//!vote me  // !vote 1/2/3
				if ((cmd->getArgs()[1] == "me") && (m_votingstate == applicationPhase))
				{
					addApplicant(pl);
					_commandsList.pop_front();
					continue;
				}
				if (m_votingstate == votingPhase)
				{
					int votedFor = 0;
					if (!(pl->hasVoted))
					{
						try
						{
							votedFor = lexical_cast<int>(cmd->getArgs()[1]);
						}
						catch (const bad_lexical_cast &) { std::cout << "Player " << cmd->getPlayername() << "has a fould vote" << std::endl; }

						if ( ((int)m_votingApplicants.size() >= votedFor) && (votedFor > 0) )
						{
							for (auto &appl : m_votingApplicants)
							{
								if (appl->m_voteid == votedFor)
								{
									appl->m_votes += 1;
									std::cout << appl->getPlayername() << " received a vote from " << cmd->getPlayername() << std::endl;
									break;
								}
							}
						}
					}
				}
			}
		}
		else if (cmd->getCommandname() == "help")
		{
			std::cout << "Player " << cmd->getPlayername() << " wants help...";

			std::string pname = cmd->getPlayername();
			std::string txt = MSG_HELP_MSG;
			std::string cmdname = cmd->getCommandname();
			this->_messagesToSend.push_back(Message::msgFactory(pname, cmdname, txt));

		}
		else if (cmd->getCommandname() == "oath")
		{
			if (_activePlayers.size() < GAME_PLAYERS_MAX)
			{
				if (!PlayerIO::hasPlayer(cmd->getPlayername(), this->_activePlayers))
				{
					this->_activePlayers.push_back(pl);
				}
				//if joined player was no bot and there are still bots kick the first you find
				if (cmd->getPlayername().find("bot") == std::string::npos)
				{
					bool foundbot = false;
					std::shared_ptr<Player2> bot;
					for (auto &pl : _activePlayers)
					{
						foundbot = (pl->getPlayername().find("bot") != std::string::npos);
						bot = pl;
						if (foundbot) break;
					}
					if (foundbot)
					{
						std::string cmdname = "betray";
						std::string uname = bot->getPlayername();
						std::vector<std::string> cmdargs = { "betray" };
						this->_commandsList.push_back(Command::cmdFactory(uname, cmdname, cmdargs));
					}
				}
			}
		}
		else if (cmd->getCommandname() == "betray") 
		{
			this->_activePlayers.remove(pl);
		}
		else if (cmd->getCommandname() == "train")
		{	// !train str

			//get waypoint and verify its valid
			if (cmd->getArgs().size() == 2)
			{	//(arg: [0]train [1]str
				std::string whatToTrain = cmd->getArgs()[1];
				
				if (whatToTrain == "strength") { whatToTrain = "str"; }
				else if (whatToTrain == "math") { whatToTrain = "mat"; }
				else if (whatToTrain == "beauty") { whatToTrain = "bty"; }
				else if (whatToTrain == "dexterity") { whatToTrain = "dex"; }
				//if ((whatToTrain == "wood") || (whatToTrain == "stone")) { whatToTrain = "woodstone"; }

				for (const auto &wp : this->_waypoints)
				{
					if (wp->getName() == whatToTrain)
					{
						std::list<int> wplist = pl->getNextWaypoint()->pathTo(wp->getId());
						std::list<std::shared_ptr<Waypoint>> tmp;
						while (!wplist.empty())
						{
							tmp.push_back(this->_waypoints[wplist.front()]);
							wplist.pop_front();
						}
						pl->setWayToGo(tmp);
					}
				}
			}
			else 
			{	//!wp
				std::cout << "Player " << cmd->getPlayername() << " had wrong arg# for train" << std::endl;
			}


		} 
		else if (cmd->getCommandname() == "work")
		{// !work gold

			//get waypoint and verify its valid
			if (cmd->getArgs().size() == 2)
			{	//(arg: [0]work [1]gold
				std::string whatToTrain = cmd->getArgs()[1];

				for (const auto &wp : this->_waypoints)
				{
					if (wp->getName() == whatToTrain)
					{
						std::list<int> wplist = pl->getNextWaypoint()->pathTo(wp->getId());
						std::list<std::shared_ptr<Waypoint>> tmp;
						while (!wplist.empty())
						{
							tmp.push_back(this->_waypoints[wplist.front()]);
							wplist.pop_front();
						}
						pl->setWayToGo(tmp);
					}
				}
			}
			else
			{	//!wp
				std::cout << "Player " << cmd->getPlayername() << " had wrong arg# for train" << std::endl;
			}

		}
		else if (cmd->getCommandname() == "rep")
		{
			if (cmd->getArgs().size() == 2)
			{
				std::string cmdname = "repair";
				std::string uname = cmd->getPlayername();
				std::vector<std::string> cmdargs = cmd->getArgs();
				cmdargs[0] = "repair";
				this->_commandsList.push_back(Command::cmdFactory(uname, cmdname, cmdargs));
			}
		}
		else if (cmd->getCommandname() == "repair")
		{	// !train str

			//get waypoint and verify its valid
			if (cmd->getArgs().size() == 2)
			{	//(arg: [0]repair [1]<gate> (left,right,mid)
				std::string whichDestObj = cmd->getArgs()[1];

				//aliases
				if (whichDestObj == "midGate" || whichDestObj == "mg") { whichDestObj = "repair-Gate-Middle"; }
				if (whichDestObj == "leftGate" || whichDestObj == "lg") { whichDestObj = "repair-Gate-Left"; }
				if (whichDestObj == "rightGate" || whichDestObj == "rg") { whichDestObj = "repair-Gate-Right"; }


				for (const auto &wp : this->_waypoints)
				{
					if (wp->getName() == whichDestObj)
					{
						std::list<int> wplist = pl->getNextWaypoint()->pathTo(wp->getId());
						std::list<std::shared_ptr<Waypoint>> tmp;
						while (!wplist.empty())
						{
							tmp.push_back(this->_waypoints[wplist.front()]);
							wplist.pop_front();
						}
						pl->setWayToGo(tmp);
					}
				}
			}
			else
			{	
				std::cout << "Player " << cmd->getPlayername() << " had wrong arg# for repair" << std::endl;
			}


		}
		else if (cmd->getCommandname() == "fight")
		{	//!fight <wp name>
			//get waypoint and verify its valid
			if (cmd->getArgs().size() == 2)
			{	//(arg: [0]fight [1]<gate> (left,right,mid)
				std::string whichDestObj = cmd->getArgs()[1];

				//aliases
				if (whichDestObj == "midGate" || whichDestObj == "mg") { whichDestObj = "fight-Gate-Middle"; }
				//if (whichDestObj == "leftGate" || whichDestObj == "lg") { whichDestObj = "fight-Gate-Left"; }
				//if (whichDestObj == "rightGate" || whichDestObj == "rg") { whichDestObj = "fight-Gate-Right"; }


				for (const auto &wp : this->_waypoints)
				{
					if (wp->getName() == whichDestObj)
					{
						std::list<int> wplist = pl->getNextWaypoint()->pathTo(wp->getId());
						std::list<std::shared_ptr<Waypoint>> tmp;
						while (!wplist.empty())
						{
							tmp.push_back(this->_waypoints[wplist.front()]);
							wplist.pop_front();
						}
						pl->setWayToGo(tmp);
					}
				}
			}
			else
			{
				std::cout << "Player " << cmd->getPlayername() << " had wrong arg# for repair" << std::endl;
			}
		}
		else if (cmd->getCommandname() == "upgrade")
		{	//!upgrade <gate>
			if (cmd->getArgs().size() == 2)
			{
				//is Player King?
				bool plIsKing = false;
				for (const auto &pl : _activePlayers)
				{
					if (pl->getPlayername() == cmd->getPlayername())
					{
						if (pl->isKing())
						{
							plIsKing = true;
							break;
						}
					}
				}

				if (!plIsKing)
				{
					std::cout << "Player " << cmd->getPlayername() << " tried an upgrade - but he is not king" << std::endl;
					_commandsList.pop_front();
					continue;
				}


				if (cmd->getArgs()[1] == "costs")
				{
					std::string pname = cmd->getPlayername();
					std::string txt = "Upgradecosts: ";
					txt += "mg: " + Helper::costToStr(_buildingList[0]->getLevelUpCost()) + " | ";
					txt += "lg: " + Helper::costToStr(_buildingList[1]->getLevelUpCost()) + " | ";
					txt += "rg: " + Helper::costToStr(_buildingList[2]->getLevelUpCost());
					std::string cmdname = "kingcmds";
					this->_messagesToSend.push_back(Message::msgFactory(pname, cmdname, txt));

					_commandsList.pop_front();
					continue;
				}
				std::string whichDestObj = cmd->getArgs()[1];

				//aliases
				if (whichDestObj == "midGate" || whichDestObj == "mg") { whichDestObj = "repair-Gate-Middle"; }
				//if (whichDestObj == "leftGate" || whichDestObj == "lg") { whichDestObj = "fight-Gate-Left"; }
				//if (whichDestObj == "rightGate" || whichDestObj == "rg") { whichDestObj = "fight-Gate-Right"; }

				for (const auto &b : this->_buildingList)
				{
					if (b->getName() == whichDestObj)
					{
						cost levelUpCost = b->getLevelUpCost();
						if (Helper::isEnoughRes(res, levelUpCost))
						{
							Helper::removeRes(res, levelUpCost);
							b->LevelUp();
							std::string pname = cmd->getPlayername();
							std::string txt = "Your king has successfully upgraded " + b->getName();
							std::string cmdname = "kingcmds";
							this->_messagesToSend.push_back(Message::msgFactory(pname, cmdname, txt));
						}
						else
						{
							std::string pname = cmd->getPlayername();
							std::string txt = "There is not enough ressources to upgrade " + b->getName();
							txt += ". You need " + Helper::costToStr(levelUpCost);
							std::string cmdname = "kingcmds";
							this->_messagesToSend.push_back(Message::msgFactory(pname, cmdname, txt));
						}
						
					}
				}
			}

		}
		else if (cmd->getCommandname() == "stats")
		{
			this->_playersThatWantToLookAtStats.push_back(PlayerIO::getPlayer(cmd->getPlayername(), this->_plList, this->_waypoints, this->m_texture_Player, this->m_font_azure));

			std::string pname = cmd->getPlayername();
			std::string txt = "Find all players stats here: https://dl.dropboxusercontent.com/u/23797543/TwitchCastle/playerstats.htm";
			std::string cmdname = cmd->getCommandname();
			this->_messagesToSend.push_back(Message::msgFactory(pname, cmdname, txt));
		}
		else if (cmd->getCommandname() == "kingcmds")
		{
			this->_playersThatWantToLookAtStats.push_back(PlayerIO::getPlayer(cmd->getPlayername(), this->_plList, this->_waypoints, this->m_texture_Player, this->m_font_azure));

			std::string pname = cmd->getPlayername();
			std::string txt = "!upgrade costs to see the ressources needed for an upgrade. !upgrade <gate> to start a upgrade.";
			std::string cmdname = cmd->getCommandname();
			this->_messagesToSend.push_back(Message::msgFactory(pname, cmdname, txt));
		}

		//free item
		_commandsList.pop_front();
	}

}

void GameState::updateGameState(sf::Time _timeSiceLastCall)
{	//called every 200ms
	m_found_king = false;

	for (auto &pl : _activePlayers)
	{
		if (pl->getActualWaypoint()->getName() == "xp")
		{
			pl->addXp(0.1f);
		}
		if (pl->getActualWaypoint()->getName() == "str")
		{
			pl->addStr(0.1f);
		}
		if (pl->getActualWaypoint()->getName() == "dex")
		{
			pl->addDex(0.1f);
		}
		if (pl->getActualWaypoint()->getName() == "mat")
		{
			pl->addMat(0.1f);
		}
		if (pl->getActualWaypoint()->getName() == "bty")
		{
			pl->addBty(0.1f);
		}

		if (pl->getJob() == Player2::Job::repairing)
		{
			for (auto &b : _buildingList)
			{
				if (pl->getActualWaypoint()->getName() == b->getName())
				{
					//repair costs 0.02f iron per tick
					float repcost = 0.02f;
					if (res.iron > 0.02f)
					{
						if (b->getHpPercent() < 100)
							res.iron -= 0.02f;
						//repair amount depends on: math
						float repamount = std::pow((pl->getMat()/50),0.35f) - 0.5f;
						b->repairIt(repamount);
					}

				}

			}

		}

		
		//Test if there is a king
		if (pl->isKing())
		{
			m_found_king = true;
		}



		//= ((C43 / 5) ^ 0.35)*((D43 / 15) ^ 0.4) / 2
		if (pl->getJob() == Player2::Job::attacking)
		{
			for (auto &ene : _enemyList)
			{
				//take first enemy with correct lane and damage it
				if (ene->getActualWaypoint()->getLane() == pl->getActualWaypoint()->getLane())
				{
					std::cout << "attack " << ene->getPos().x << std::endl;
				//	if (
					//std::cout << abs(ene->getPos().y - pl->getPos().y) << std::endl;
					if (ene->getPos().y >= pl->getPos().y)
					{
						float dmg = (float)(pow((pl->getStr() / 5), 0.35f)*pow((pl->getDex() / 15), 0.4f)*0.5);
						ene->damageIt(dmg);
						std::cout << "enemy life: " << ene->getHp() << std::endl;
					}
					break;
				}
			}
		}
	}

	if ((m_votingstate == haveKing) || (m_votingstate == noKing)) timeSinceLastVote += _timeSiceLastCall;
	if ((m_votingstate == applicationPhase) || (m_votingstate == votingPhase))  timeTillEndPhase -= _timeSiceLastCall;
	//found no king or time has passed
	if ( ((m_found_king == false) && (m_votingstate == haveKing)) || timeSinceLastVote >= updateTimeVote )
	{
		//Start new Voting
		if (_activePlayers.size() >= 2)
		{
			m_votingstate = applicationPhase;
			timeTillEndPhase = sf::seconds(30);
			startApplicationPhase();
		}
		timeSinceLastVote = sf::Time::Zero;
	}

	if ((timeTillEndPhase <= sf::Time::Zero) && (m_votingstate == applicationPhase))
	{
		//Start the real vote
		m_votingstate = votingPhase;
		timeTillEndPhase = sf::seconds(30);
		startVotingPhase();
	}

	if ((timeTillEndPhase <= sf::Time::Zero) && (m_votingstate == votingPhase))
	{
		//Start the real vote
		m_votingstate = haveKing;
		endVotingPhase();
	}



	//Handling Enemies

	for (auto &en : _enemyList)
	{
		if (en->getModeEnemyIsIn() == Enemy::modus::attack)
		{
			_buildingList[en->getLane()]->damageIt(en->getAtk());
			//std::cout << _buildingList[en->getLane()]->getHpPercent();
			if ((!m_sent_50_Warning) && (_buildingList[en->getLane()]->getHpPercent() < 50))
			{
				m_sent_50_Warning = true;

				std::string pname = "Info";
				std::string txt = "The Middle Gate has less than 50% HP left!";
				std::string cmdname = "Info";
				this->_messagesToSend.push_back(Message::msgFactory(pname, cmdname, txt));
			}

			if ((!m_sent_0_Warning) && (_buildingList[en->getLane()]->getHpPercent() <= 0))
			{
				m_sent_0_Warning = true;

				std::string pname = "Info";
				std::string txt = "The castle has fallen... The enemies will now raid your town. Are you happy now?";
				std::string cmdname = "Info";
				this->_messagesToSend.push_back(Message::msgFactory(pname, cmdname, txt));
			}
		}
	}

	//Raid Stuff
	if (!m_isRaidOngoing)
	{
		timeSinceLastRaid += _timeSiceLastCall;
		if ((!m_sent_raid_announcement) && ((updateTimeRaid - timeSinceLastRaid) < sf::seconds(30)))
		{
			m_sent_raid_announcement = true;
			std::string pname = "Info";
			std::string txt = "The next raid starts in 30 seconds! Prepare your weapons!";
			std::string cmdname = "Info";
			this->_messagesToSend.push_back(Message::msgFactory(pname, cmdname, txt));
		}
	}

	if (timeSinceLastRaid >= updateTimeRaid)
	{
		timeSinceLastRaid = sf::Time::Zero;
		initializeRaid();
	}

	//Test if raid is over
	if (m_isRaidOngoing && _enemyList.empty())
	{
		m_isRaidOngoing = false;
		m_sent_50_Warning = false;
		m_sent_0_Warning = false;
		m_sent_raid_announcement = false;

		bool castle_got_raided = false;

		for (auto &b : _buildingList)
		{
			//if the mg is completly down we reset the wave-counter 
			if (b->getHpPercent() <= 0)
			{
				m_wavecount = 0;
				castle_got_raided = true;
				// TODO (fh): Add losing something on random(?) - Upgrade/stat? 
			}


			b->canBeRepairedAgain();
		}

		if (castle_got_raided)
		{
			std::string pname = "Info";
			std::string txt = "Your castle got raided and you lost something!";
			std::string cmdname = "Info";
			this->_messagesToSend.push_back(Message::msgFactory(pname, cmdname, txt));
		}
		else
		{
			std::string pname = "Info";
			std::string txt = "You successfully defended the castle!";
			std::string cmdname = "Info";
			this->_messagesToSend.push_back(Message::msgFactory(pname, cmdname, txt));
		}
	}

	


}

int GameState::getTimeTillNextRaid()
{
	sf::Time ttnr = updateTimeRaid - timeSinceLastRaid;
	return (int)(ttnr.asSeconds());
}

void GameState::initializeRaid()
{
	//create Enemies
	//one in front of the main gate
	std::cout << "Starting raid " << std::endl;
	m_wavecount++;
	m_isRaidOngoing = true;

	//list for middle Path
	std::list< std::shared_ptr<Waypoint> > middlePath;
	middlePath.push_back(_waypoints[WP_COUNT_AREA + WP_COUNT_PATHS]);
	middlePath.push_back(_waypoints[WP_COUNT_AREA + WP_COUNT_PATHS + 1]);
	middlePath.push_back(_waypoints[WP_COUNT_AREA + WP_COUNT_PATHS + 2]);
	middlePath.push_back(_waypoints[WP_COUNT_AREA + WP_COUNT_PATHS]);

	std::list< std::shared_ptr<Waypoint> > leftPath;
	leftPath.push_back(_waypoints[WP_COUNT_AREA + WP_COUNT_PATHS + 3]);
	leftPath.push_back(_waypoints[WP_COUNT_AREA + WP_COUNT_PATHS + 4]);
	leftPath.push_back(_waypoints[WP_COUNT_AREA + WP_COUNT_PATHS + 5]);
	leftPath.push_back(_waypoints[WP_COUNT_AREA + WP_COUNT_PATHS + 3]);

	std::list< std::shared_ptr<Waypoint> > rightPath;
	rightPath.push_back(_waypoints[WP_COUNT_AREA + WP_COUNT_PATHS + 6]);
	rightPath.push_back(_waypoints[WP_COUNT_AREA + WP_COUNT_PATHS + 7]);
	rightPath.push_back(_waypoints[WP_COUNT_AREA + WP_COUNT_PATHS + 8]);
	rightPath.push_back(_waypoints[WP_COUNT_AREA + WP_COUNT_PATHS + 6]);

	//Enemy(id, HP, atk)
	//HP=(wave+10)^1.2
	//#=CEILING(D32/6,1)
	//atk

	float hp_thiswave = (float)pow((m_wavecount + 9),1.1f);
	int count_thiswave = (int)(1 + m_wavecount / 15);
	if(LOG_WAVESTART) std::cout << "Starting wave no " << m_wavecount << " with " << count_thiswave << " enemies that have " << hp_thiswave << "hp." << std::endl;

	for (int i = 0; i < count_thiswave; ++i)
	{
		std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>(0, hp_thiswave, 2.0f);

		enemy->init(this->_waypoints[WP_COUNT_AREA + WP_COUNT_PATHS],
			middlePath, this->_buildingList,
			m_texture_Player, m_texture_HpBar, m_texture_HpFill);
		this->_enemyList.push_back(enemy);
	}

	for (int i = 0; i <= -1; ++i)
	{
		std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>(0, 10, 2.0f);

		enemy->init(this->_waypoints[WP_COUNT_AREA + WP_COUNT_PATHS + 3 ],
			leftPath, this->_buildingList,
			m_texture_Player, m_texture_HpBar, m_texture_HpFill);
		this->_enemyList.push_back(enemy);
	}

	for (int i = 0; i <= -1; ++i)
	{
		std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>(0, 10, 2.0f);

		enemy->init(this->_waypoints[WP_COUNT_AREA + WP_COUNT_PATHS + 6], rightPath,
			this->_buildingList,
			m_texture_Player, m_texture_HpBar, m_texture_HpFill);
		this->_enemyList.push_back(enemy);
	}
	//start to move Players that joined the raid

}

void GameState::startApplicationPhase()
{
	m_votingApplicants.clear();

	for (auto &pl : _activePlayers)
	{
		pl->setKing(false);
	}

	std::string pname = "Info";
	std::string txt = "A new Voting for the king has just started. If you want to become king apply by typing \"!vote me\"";
	std::string cmdname = "Info";
	this->_messagesToSend.push_back(Message::msgFactory(pname, cmdname, txt));
}

void GameState::addApplicant(std::shared_ptr<Player2> _newAppl)
{
	for (const auto &apl : m_votingApplicants)
	{
		if (apl->getPlayername() == _newAppl->getPlayername())
			return;
	}
	m_votingApplicants.push_back(_newAppl);

	std::string pname = "Info";
	std::string txt = _newAppl->getPlayername() + " has applied for becoming King!";
	std::string cmdname = "Info";
	this->_messagesToSend.push_back(Message::msgFactory(pname, cmdname, txt));
}

void GameState::startVotingPhase()
{
	if (m_votingApplicants.empty())
	{
		std::string pname = "Info";
		std::string txt = "Nobody wants to be king... ";
		std::string cmdname = "Info";
		this->_messagesToSend.push_back(Message::msgFactory(pname, cmdname, txt));

		timeSinceLastVote = sf::seconds(GAME_TIME_BETWEEN_VOTINGS * 3 / 4);
		m_votingstate = noKing;
		return;
	}

	for (auto &pl : _activePlayers)
	{
		pl->hasVoted = false;
		pl->m_votes = 0;
	}

	int cnt = 1;
	std::string applicants = "";
	for (auto &apl : m_votingApplicants)
	{
		apl->m_voteid = cnt;
		applicants += "!vote " + std::to_string(cnt) + " for " + apl->getPlayername() + ". ";
		cnt++;
	}

	std::string pname = "Info";
	std::string txt = "Vote now for your king! " + applicants;
	std::string cmdname = "Info";
	this->_messagesToSend.push_back(Message::msgFactory(pname, cmdname, txt));
}

void GameState::endVotingPhase()
{
	std::shared_ptr<Player2> winner;
	int maxvotes = -1;

	if (m_votingApplicants.empty()) return;

	for (auto &apl : m_votingApplicants)
	{
		if (apl->m_votes > maxvotes)
		{
			winner = apl;
		}
	}

	winner->setKing(true);
	timeSinceLastVote = sf::Time::Zero;

	std::string pname = "Info";
	std::string txt = "And your new King is: " + winner->getPlayername() + " with " + std::to_string(winner->m_votes);
	if (winner->m_votes <= 1) txt += " vote";
	else txt += " votes";
	std::string cmdname = "Info";
	this->_messagesToSend.push_back(Message::msgFactory(pname, cmdname, txt));
}