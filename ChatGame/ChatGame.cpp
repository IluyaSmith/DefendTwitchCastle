// ChatGame.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "defines.h"

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <iostream>
#include <thread>

#include "GameState.h"
#include "Network_Handler.h"
#include "File_Handler.h"
#include "Draw_Handler.h"

int _tmain(int argc, _TCHAR* argv[])
{

	//Setting up the window
	sf::RenderWindow _app(sf::VideoMode(WINDOW_SIZE_X, WINDOW_SIZE_Y), "ChatGame v1");
	//_app.setVerticalSyncEnabled(true);
	_app.setPosition(sf::Vector2i(570,325));

	//Setting up GameState
	GameState _GS;
	_GS.initialize();

	//Setting up File interaction
	File_Handler _File(_GS);
	_File.initialize();
	
	// Clock used in restricting Update loop to a fixed rate
	sf::Clock clock;

	//Setting update Intervals (not exactly for better load distribution)
	sf::Time updateRateFast = sf::seconds(1.0f / 60.0f);
	sf::Time updateRate1Sec = sf::milliseconds(200);
	sf::Time updateRate3Sec = sf::seconds(2.975f);
	sf::Time updateRate30Sec = sf::seconds(30.01f);
	
	// Time since last update
	sf::Time timeSinceLastUpdateFast = sf::Time::Zero;
	sf::Time timeSinceLastUpdate1Sec = sf::Time::Zero;
	sf::Time timeSinceLastUpdate3Sec = sf::Time::Zero;
	sf::Time timeSinceLastUpdate30Sec = sf::Time::Zero;
	sf::Time timeSinceLastUpdateRateAnimations = sf::Time::Zero;

	//Initialize Network
	sf::TcpSocket _Socket;
	Network_Handler _Network(_GS,_Socket);
	_Network.initialize();
	sf::Socket::Status status = _Socket.connect("irc.twitch.tv", 6667);
	if (status != sf::Socket::Done){
		std::cout << "Error! Socket failed!\n";
		return EXIT_FAILURE;
	}
	if (IRC_USE) _Network.ConnectToIrc();
	
	//Initialize Renderer
	Draw_Handler _draw(_GS,_app);

	/* Testing

	//*/

	PlayerIO::savePlayerStatsToFile(_GS);

	std::cout << "\n\n+++ Initialization complete - Starting... +++ \n\n";

	/*
	std::thread rendering(&Draw_Handler::startRenderLoop, &_draw);
	rendering.detach();
	*/

	//add initial 3 bots
	for (int i = 0; i < 4; i++)
	{
		std::string uname = "bot" + to_string(i);
		std::string tmp_cmdname = "oath";
		std::vector<std::string> tmp_args = { "oath" };
		_GS._commandsList.push_back(Command::cmdFactory(uname, tmp_cmdname, tmp_args));
		switch (i)
		{
		case 0:
			tmp_cmdname = "repair";
			tmp_args = { "repair", "mg" };
			_GS._commandsList.push_back(Command::cmdFactory(uname, tmp_cmdname, tmp_args));
			break;
		case 1:
			tmp_cmdname = "work";
			tmp_args = { "work", "wood" };
			_GS._commandsList.push_back(Command::cmdFactory(uname, tmp_cmdname, tmp_args));
			break;
		case 2:
			tmp_cmdname = "work";
			tmp_args = { "work", "iron" };
			_GS._commandsList.push_back(Command::cmdFactory(uname, tmp_cmdname, tmp_args));
			break;
		case 3:
			tmp_cmdname = "fight";
			tmp_args = { "fight", "mg" };
			_GS._commandsList.push_back(Command::cmdFactory(uname, tmp_cmdname, tmp_args));
			break;
		}
	}

	sf::Time dt;
	int fpscount = 0;
	while (_app.isOpen())
	{

		dt = clock.restart();
		timeSinceLastUpdateFast += dt;
		timeSinceLastUpdate1Sec += dt;
		timeSinceLastUpdate3Sec += dt;
		timeSinceLastUpdate30Sec += dt;
		timeSinceLastUpdateRateAnimations += dt;

		if (LOG_DT) { std::cout << std::to_string(dt.asMilliseconds()) << std::endl;  }

		//Fixed Updatetimes
		//60 times a sec
		if (timeSinceLastUpdateFast > updateRateFast) {
			

			//Take any messages from IRC!
			if (IRC_USE) _Network.ListenToIrc();

			//Move, Stat changes, recalc of stats, etc...
			PlayerIO::updateEntities(_GS, timeSinceLastUpdateFast);
			timeSinceLastUpdateFast -= updateRateFast;
		}


		//Every second
		if (timeSinceLastUpdate1Sec > updateRate1Sec) {

			//do the Command Queque
			_GS.workCommands();

			_GS.updateGameState(timeSinceLastUpdate1Sec);

			timeSinceLastUpdate1Sec -= updateRate1Sec;
		}

		//Every 3 Secs
		if (timeSinceLastUpdate3Sec > updateRate3Sec) {

			//Send collected Messages into the Channel
			if (IRC_USE) _Network.WorkMessagesToChannel();

			std::cout << "fps: " << (int)(fpscount / timeSinceLastUpdate3Sec.asSeconds()) << std::endl;
			fpscount = 0;

			timeSinceLastUpdate3Sec -= updateRate3Sec;

		}

		//Every 30 Secs
		if (timeSinceLastUpdate30Sec > updateRate30Sec) {
			timeSinceLastUpdate30Sec -= updateRate30Sec;

			//Save Players to File
			PlayerIO::savePlayerList(_GS._plList);
			PlayerIO::savePlayerStatsToFile(_GS);

			//Reset Message Count
			_GS.resetMessagesSend();
		}

		sf::Event event;
		while (_app.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				PlayerIO::savePlayerListSave(_GS._plList);
				_app.close();
			}
		}

		_draw.draw(dt);
		fpscount++;

	}

	return EXIT_SUCCESS;

}

