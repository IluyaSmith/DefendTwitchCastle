#pragma once

#include "defines.h"
#include <iostream>

#include <string>
#include <cmath>
#include <memory>

#include <SFML/Graphics.hpp>
#include <boost\lexical_cast.hpp>
#include "ini_parser.hpp"
#include "Helper.h"

class Waypoint
{
public:
	Waypoint() {};
	Waypoint(std::string _name, int id);
	~Waypoint() {};

	enum wptype { area, path };
	enum lane{mid, left, right, none};

	int getId();
	int getType();
	lane getLane() { return m_myLane; }

	std::string getName() { return m_name; }

	float getPosX();
	float getPosY();

	sf::Vector2f getPos();

	sf::Vector2f randomPointInArea();

	//Pathfinding vars
	std::list<Helper::vertex_t> pathTo(int _targetWP);

	std::vector<int> m_hasEdgesTo;
	std::vector<Helper::vertex_t> m_pathfinder;

	static std::vector< std::shared_ptr<Waypoint> > loadWaypoints();
	
private:
	int m_id;

	float m_posx;
	float m_posy;

	float m_areax;
	float m_areay;

	int m_type;
	lane m_myLane;

	std::string m_connectedTo;

	std::string m_name;

	//Djikstra
	typedef int vertex_t;
	typedef double weight_t;

	struct neighbor {
		vertex_t target;
		weight_t weight;
		neighbor(vertex_t arg_target, weight_t arg_weight)
			: target(arg_target), weight(arg_weight) { }
	};

	typedef std::vector<std::vector<neighbor> > adjacency_list_t;

	static void DijkstraComputePaths(vertex_t source,
		const adjacency_list_t &adjacency_list,
		std::vector<weight_t> &min_distance,
		std::vector<vertex_t> &previous);

};

