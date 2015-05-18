#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <list>

#include <SFML\Graphics.hpp>

#include <limits> // for numeric_limits

#include <set>
#include <utility> // for pair
#include <algorithm>
#include <iterator>

#include "defines.h"




class Helper
{
public:
	Helper();
	~Helper();

	// Split
	static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
	static std::vector<std::string> split(const std::string &s, char delim);

	// cost to string
	static std::string costToStr(const cost& _c);
	static bool isEnoughRes(const cost& _res, const cost& _costs);
	static void removeRes(cost& _res, const cost& _costs);

	//PointCalc
	static bool hasReached(sf::Vector2f _playerposition, sf::Vector2f _targetposition, float _velocity);

	//Djikstra
	typedef int vertex_t;
	typedef double weight_t;

	const weight_t max_weight = std::numeric_limits<double>::infinity();

	struct neighbor {
		vertex_t target;
		weight_t weight;
		neighbor(vertex_t arg_target, weight_t arg_weight)
			: target(arg_target), weight(arg_weight) { }
	};

	typedef std::vector<std::vector<neighbor> > adjacency_list_t;

	void DijkstraComputePaths(vertex_t source,
		const adjacency_list_t &adjacency_list,
		std::vector<weight_t> &min_distance,
		std::vector<vertex_t> &previous);

	std::list<vertex_t> DijkstraGetShortestPathTo(
		vertex_t vertex, const std::vector<vertex_t> &previous);

};

