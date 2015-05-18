#pragma once

#include <string>
#include <vector>
#include <memory>

class Command
{
public:
	Command() : m_playername("NA"), m_commandname("NA"), m_args({}) {};
	Command(std::string _plname, std::string _cname, std::vector<std::string> _args) : m_playername(_plname), m_commandname(_cname), m_args(_args) {};
	static std::shared_ptr<Command> cmdFactory(std::string& _plname, std::string& _cname, std::vector<std::string>& _args)
	{
		return std::make_shared<Command>(_plname, _cname, _args);
	}
	~Command() {};

	std::string getPlayername() { return m_playername; }
	std::string getCommandname() { return m_commandname; }
	std::vector<std::string> getArgs() { return m_args; }

private:
	std::string m_playername;
	std::string m_commandname;
	std::vector<std::string> m_args;

};

