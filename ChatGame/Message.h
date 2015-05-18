#pragma once

#include <string>
#include <memory>

class Message
{
public:
	Message() : m_toPlayer(""), m_fromCommand(""), m_text("") {};
	Message(std::string _toPlayer, std::string _fromCmd, std::string _text) : m_toPlayer(_toPlayer), m_fromCommand(_fromCmd), m_text(_text) {};
	static std::shared_ptr<Message> msgFactory(std::string _toPlayer, std::string _fromCmd, std::string _text)
	{
		return std::make_shared<Message>(_toPlayer, _fromCmd, _text);
	}
	~Message() {}

	std::string getReveivingPlayer() { return m_toPlayer; }
	std::string getSendingCommand() { return m_fromCommand; }
	std::string getText() { return m_text; }

private:
	std::string m_toPlayer;
	std::string m_fromCommand;
	std::string m_text;
};

