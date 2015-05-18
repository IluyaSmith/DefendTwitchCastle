#pragma once

#include "defines.h"
#include "GameState.h"
#include "ini_parser.hpp"

#include <time.h>

#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>

class File_Handler
{
public:
	File_Handler(GameState &state);
	~File_Handler();

	void initialize();

	bool loadPlayer(std::string _name);

	bool writeToChatLog(std::string _msg);

private:
	GameState *m_gs;
	std::fstream myfile;

};

