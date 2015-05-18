#include "stdafx.h"
#include "File_Handler.h"

File_Handler::File_Handler(GameState &state)
{
	this->m_gs = &state;
}


File_Handler::~File_Handler()
{
	
}

void File_Handler::initialize() {

	//clear log and rename old one
	myfile.open(FILE_LOCATION_CHATLOG);
	std::string old_timestamp;
	getline(myfile, old_timestamp);
	old_timestamp = FILE_LOCATION + old_timestamp + "chatlog.log";
	myfile.close();
	std::rename(FILE_LOCATION_CHATLOG, old_timestamp.c_str());

	myfile.open(FILE_LOCATION_CHATLOG, std::ofstream::app);
	time_t rawtime;
	time(&rawtime);
	myfile << std::to_string(rawtime) << std::endl << std:: endl;
	myfile.close();

	ini_parser ini_startup(INI_LOCATION_STARTUP);

	//Making IRC Token
	std::string authtoken = "PASS ";
	authtoken.append(ini_startup.get_string("PASS", "Login"));
	authtoken.append("\nNICK ");
	authtoken.append(ini_startup.get_string("USER", "Login"));
	authtoken.append("\nUSER ");
	authtoken.append(ini_startup.get_string("NICK", "Login"));
	authtoken.append("\r\n");
	m_gs->zauth_token = authtoken;

}

bool File_Handler::writeToChatLog(std::string _msg)
{
	myfile.open(FILE_LOCATION_CHATLOG, std::ios::app);
	myfile << _msg;
	myfile.close();

	return EXIT_SUCCESS;
}
