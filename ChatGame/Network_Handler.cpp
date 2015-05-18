#include "stdafx.h"
#include "Network_Handler.h"


Network_Handler::Network_Handler(GameState &state, sf::TcpSocket &socket)
{
	this->m_gs = &state;
	this->m_sckt = &socket;
}

Network_Handler::~Network_Handler()
{
}

void Network_Handler::initialize()
{

}

////////////////////

int Network_Handler::ConnectToIrc()
{


	std::cout << "Trying to log into Twitch... " << std::endl;

	std::string message = m_gs->zauth_token;
	SendMsgToIrc(message);

	char buffer[1024];
	std::size_t recieved = 0;
	m_sckt->receive(buffer, sizeof(buffer), recieved);
	std::string servermsg(buffer);
	servermsg.erase(recieved,servermsg.length());
	std::cout << "Twitch initial response: \n" << servermsg << std::endl;

	//Set Socket non-blocking and let "ListenToIRC" handle the rest!
	m_sckt->setBlocking(false);

	//Channel for testing: JOIN #tangentgaming\r\n
	//Join iluyasmith
	message = "JOIN #iluyasmith";
	SendMsgToIrc(message);

	return 0;
}

void Network_Handler::ListenToIrc()
{
	char buffer[1024];
	std::size_t recieved = 0;
	std::string servermsg;
	if (!(sf::Socket::NotReady == m_sckt->receive(buffer, sizeof(buffer), recieved)))
	{
		//do something with the message!
		servermsg = std::string(buffer);
		servermsg.erase(recieved, servermsg.length());

		//Log into oconsole
		if (LOG_CHAT) { std::cout << servermsg << std::endl; }

		//Log into file
		//m_file->writeToChatLog(servermsg);

		//split message per line
		std::vector<std::string> mgs;
		std::istringstream txt(servermsg);
		std::string line;
		while (std::getline(txt,line))
		{

			//Reply to a PING
			//PING :tmi.twitch.tv
			if (line.find("PING") != std::string::npos)
			{
				std::string reply = line.replace(0, 4, "PONG");
				SendMsgToIrc(reply);
				continue;
			}

			//Looking for join messages and create a Player for them!
			//:<uname>!<uname>@<uname>.tmi.twitch.tv JOIN #<chname>
			if (line.find("JOIN") != std::string::npos)
			{
				if (line.find(IRC_OPERATING_CHANNEL) != std::string::npos)
				{
					//Join message in the right channel take the name (: to !)
					std::string cmdname = "hello";
					std::string uname = line.substr(1, line.find_first_of("!") - 1);
					std::vector<std::string> cmdargs{ "hello" };
					m_gs->_commandsList.push_back(Command::cmdFactory(uname, cmdname, cmdargs));

					if (LOG_PLAYERS_NEW) { std::cout << "Found new Player: " << uname << std::endl; }
					continue;
				}
			}

			if (line.find("PART") != std::string::npos)
			{
				if (line.find(IRC_OPERATING_CHANNEL) != std::string::npos)
				{
					//Join message in the right channel take the name (: to !)
					std::string cmdname = "bye";
					std::string uname = line.substr(1, line.find_first_of("!") - 1);
					std::vector<std::string> cmdargs { "bye" };
					m_gs->_commandsList.push_back(Command::cmdFactory(uname, cmdname, cmdargs));

					if (LOG_PLAYERS_NEW) { std::cout << "Player left: " << uname << std::endl; }
					
					continue;
				}
			}

			//Looking for commands (looking for a ":!")
			//:<uname>!<uname>@<uname>.tmi.twitch.tv PRIVMSG #<chname> :!<command>
			if (line.find(IRC_OPERATING_CHANNEL) != std::string::npos)
			{
				
				std::size_t cmdpos;

				//debug command for me !add <stat> <amount>
				cmdpos = line.find(":!add");
				if (cmdpos != std::string::npos) { m_gs->addCommand(line, cmdpos, "add"); continue; }

				//debug command for me !cmd <cmd> | !cmd u <uname> <cmd> | !cmd uu <amount bots<int>>
				cmdpos = line.find(":!cmd");
				if (cmdpos != std::string::npos) { m_gs->addCommand(line, cmdpos, "cmd"); continue; }

				//debug command !wp <wp to go<int>>
				cmdpos = line.find(":!wp");
				if (cmdpos != std::string::npos) { m_gs->addCommand(line, cmdpos, "wp"); continue; }


				cmdpos = line.find(":!oath");
				if (cmdpos != std::string::npos) { m_gs->addCommand(line, cmdpos, "oath"); continue; }

				cmdpos = line.find(":!betray");
				if (cmdpos != std::string::npos) { m_gs->addCommand(line, cmdpos, "betray"); continue; }


				cmdpos = line.find(":!vote");
				if (cmdpos != std::string::npos) { m_gs->addCommand(line, cmdpos, "vote"); continue; }

				cmdpos = line.find(":!upgrade");
				if (cmdpos != std::string::npos) { m_gs->addCommand(line, cmdpos, "upgrade"); continue; }


				cmdpos = line.find(":!work");
				if (cmdpos != std::string::npos) { m_gs->addCommand(line, cmdpos, "work"); continue; }

				cmdpos = line.find(":!train");
				if (cmdpos != std::string::npos) { m_gs->addCommand(line, cmdpos, "train"); continue; }

				cmdpos = line.find(":!repair");
				if (cmdpos != std::string::npos) { m_gs->addCommand(line, cmdpos, "repair"); continue; }
				cmdpos = line.find(":!rep");
				if (cmdpos != std::string::npos) { m_gs->addCommand(line, cmdpos, "rep"); continue; }


				cmdpos = line.find(":!fight");
				if (cmdpos != std::string::npos) { m_gs->addCommand(line, cmdpos, "fight"); continue; }


				cmdpos = line.find(":!stats");
				if (cmdpos != std::string::npos) { m_gs->addCommand(line, cmdpos, "stats"); continue; }

				cmdpos = line.find(":!help");
				if (cmdpos != std::string::npos) { m_gs->addCommand(line, cmdpos, "help"); continue; }

				cmdpos = line.find(":!kingcmds");
				if (cmdpos != std::string::npos) { m_gs->addCommand(line, cmdpos, "kingcmds"); continue; }


			}
		}
	}

}

void Network_Handler::SendMsgToIrc(std::string &_msg)
{
	if (m_gs->getMessagesSend() < IRC_MAX_MSG_PER_30)
	{
		_msg += "\r\n";
		sf::Socket::Status bla = m_sckt->send(_msg.c_str(), _msg.size() + 1);

		m_gs->incMessagesSend();
		if (LOG_SEND){ std::cout << "[SMTI] SEND: " << _msg  << " [" << bla << "]" << std::endl; }
	}
	else
	{
		std::cout << "TOO MANY MESSAGES!!!!" << std::endl;
	}
}

void Network_Handler::SendMsgToChannel(std::string &_msg)
{
	if (m_gs->getMessagesSend() < IRC_MAX_MSG_PER_30)
	{
		std::string chn = IRC_OPERATING_CHANNEL;
		//PRIVMSG #channel-name :YOUR MESSAGE HERE
		_msg = "PRIVMSG " + chn + " :" + _msg + "\r\n";
		sf::Socket::Status bla = m_sckt->send(_msg.c_str(), _msg.size() + 1);

		m_gs->incMessagesSend();
		if (LOG_SEND){ std::cout << "[SMTC] SEND: " << _msg << " [" << bla << "]" << std::endl; }
	}
	else
	{
		std::cout << "TOO MANY MESSAGES!!!!" << std::endl;
	}
}

void Network_Handler::WorkMessagesToChannel()
{
	//std::cout << "Sending Messages... \n";

	//Prepare Strings here.. < later >
	for (std::list<std::shared_ptr<Message>>::iterator it = m_gs->_messagesToSend.begin(); it != m_gs->_messagesToSend.end(); ++it)
	{
		Message* item = (*it).get();
		//std::cout << item->text << std::endl;
	}

	while (!m_gs->_messagesToSend.empty())
	{
		Message *msg = m_gs->_messagesToSend.front().get();

		std::cout << "Message to " << msg->getReveivingPlayer() << ": " << msg->getText() << std::endl;

		if ( (msg->getSendingCommand() == "help") ||
			 (msg->getSendingCommand() == "stats") ||
			 (msg->getSendingCommand() == "Info") ||
			 (msg->getSendingCommand() == "kingcmds") )
		{
			SendMsgToChannel(m_gs->_messagesToSend.front()->getText());
		}

		m_gs->_messagesToSend.pop_front();

	}
}