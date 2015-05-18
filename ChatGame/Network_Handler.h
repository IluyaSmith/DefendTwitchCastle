#pragma once

#include "defines.h"
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include "GameState.h"
#include "File_Handler.h"
#include "Command.h"
#include "Message.h"

#include <vector>
#include <sstream>
#include <cctype>



class Network_Handler
{
public:
	Network_Handler(GameState &state, sf::TcpSocket &socket);
	~Network_Handler();

	void initialize();

	int ConnectToIrc();
	void ListenToIrc();

	void SendMsgToIrc(std::string &_msg);
	void SendMsgToChannel(std::string &_msg);

	void WorkMessagesToChannel();
	
private:
	GameState *m_gs;
	sf::TcpSocket *m_sckt;

};

