#include "stdafx.h"
#include "Waypoint.h"


//Loads the waypoint specifications - please make sure it exists before using this function!
Waypoint::Waypoint(std::string _name, int id)
{
	this->m_id = id;

	this->m_posx = 0;
	this->m_posy = 0;
	this->m_type = 0;

	this->m_areax = 0;
	this->m_areay = 0;

	ini_parser ini_wp(INI_LOCATION_WAYPOINTS);
	try
	{
		this->m_posx = ini_wp.get_float("x", _name)-8;
		this->m_posy = ini_wp.get_float("y", _name)-17;
		this->m_type = ini_wp.get_int("type", _name);
		if (this->m_type == Waypoint::wptype::area) {
			this->m_areax = ini_wp.get_float("areax", _name);
			this->m_areay = ini_wp.get_float("areay", _name);
			this->m_name = ini_wp.get_string("name", _name);
		}
		else
		{
			this->m_name = "path";
		}
		this->m_connectedTo = ini_wp.get_string("connectedTo", _name);
	}
	catch (std::runtime_error e)
	{
		std::cout << "[CritERROR!] Could not load waypoint! Check the section ->" << _name << std::endl;
	}

	std::vector< std::string > laneStrings = Helper::split(this->getName(), '-');
	if (laneStrings.size() == 2)
	{ // midGate-fight
		if (laneStrings[0] == "midGate"){ m_myLane = lane::mid; }
		else if (laneStrings[0] == "leftGate"){ m_myLane = lane::left; }
		else if (laneStrings[0] == "rightGate"){ m_myLane = lane::right; }
		else { m_myLane = lane::none; }
	}
	if (laneStrings.size() == 3)
	{ // repair-Gate-Middle
		if (laneStrings[2] == "Middle") { m_myLane = lane::mid; }
		else if (laneStrings[2] == "Left"){ m_myLane = lane::left; }
		else if (laneStrings[2] == "Right"){ m_myLane = lane::right; }
		else { m_myLane = lane::none; }
	}
}

int Waypoint::getId() { return this->m_id; }
int Waypoint::getType() { return this->m_type; }

float Waypoint::getPosX() { return this->m_posx; }
float Waypoint::getPosY() { return this->m_posy; }

sf::Vector2f Waypoint::getPos() { return sf::Vector2f(this->m_posx, this->m_posy); }

sf::Vector2f Waypoint::randomPointInArea()
{
	if (this->m_type == Waypoint::wptype::path)
	{
		return sf::Vector2f(m_posx, m_posy);
	}
	float variancex = (rand() % static_cast<int>(m_areax * 2)) - m_areax;
	float variancey = (rand() % static_cast<int>(m_areay * 2)) - m_areay;
	return sf::Vector2f(m_posx+variancex,m_posy+variancey);
}

std::list<int> Waypoint::pathTo(int _targetWP){

	std::list<int> path;
	for (; _targetWP != -1; _targetWP = m_pathfinder[_targetWP])
	{
		path.push_front(_targetWP);
	}

	return path;
}

std::vector< std::shared_ptr<Waypoint> > Waypoint::loadWaypoints()
{
	if (LOG_LOAD) std::cout << "Loading waypoints...";
	int counter = 0;

	std::vector< std::shared_ptr<Waypoint> > wps;

	for (int i = 0; i < WP_COUNT_AREA; i++)
	{
		wps.push_back(std::make_shared<Waypoint>("wp_a_" + std::to_string(i), i));
		++counter;
	}

	for (int i = 0; i < WP_COUNT_PATHS; i++)
	{
		wps.push_back(std::make_shared<Waypoint>("wp_p_" + std::to_string(i), i + WP_COUNT_AREA));
		++counter;
	}

	for (int i = 0; i < WP_COUNT_ENEMY_WPS; i++)
	{
		wps.push_back(std::make_shared<Waypoint>("wp_e_" + std::to_string(i), i + WP_COUNT_AREA + WP_COUNT_PATHS));
		++counter;
	}

	int countofwaypoints = WP_COUNT_AREA + WP_COUNT_PATHS + WP_COUNT_ENEMY_WPS;

	for (auto &wp : wps)
	{
		std::vector<std::string> conTo = Helper::split(wp->m_connectedTo,',');

		for (auto &edge : conTo)
		{
			if (edge[0] == 'a')
			{
				edge = edge.substr(1, edge.size());
				wps[wp->getId()]->m_hasEdgesTo.push_back(boost::lexical_cast<int>(edge));
			}
			else if (edge[0] == 'p')
			{
				edge = edge.substr(1, edge.size());
				wps[wp->getId()]->m_hasEdgesTo.push_back(boost::lexical_cast<int>(edge)+WP_COUNT_AREA);
			}
			else if (edge[0] == 'e')
			{
				edge = edge.substr(1, edge.size());
				wps[wp->getId()]->m_hasEdgesTo.push_back(boost::lexical_cast<int>(edge)+WP_COUNT_AREA+WP_COUNT_PATHS);
			}
		}
	}

	//Got all Waypoints stored... connecting them
	//weight = distance ala pyth 
	adjacency_list_t adjacency_list(countofwaypoints);

	for (auto &wp: wps)
	{
		for (auto &edge : wp->m_hasEdgesTo)
		{
			Helper::weight_t w = sqrt(
				(wps[edge]->getPosX() - wp->getPosX())*(wps[edge]->getPosX() - wp->getPosX()) +
				(wps[edge]->getPosY() - wp->getPosY())*(wps[edge]->getPosY() - wp->getPosY())
				);
			adjacency_list[wp->getId()].push_back(neighbor(edge, w));
		}
	}

	//Store shortest path list
	for (auto &wp : wps)
	{
		std::vector<weight_t> min_distance;
		//std::vector<Helper::vertex_t>* previous = new std::vector<Helper::vertex_t>;
		DijkstraComputePaths(wp->getId(), adjacency_list, min_distance, wp->m_pathfinder);
		//std::cout << "Distance from 2 to 5: " << min_distance[5] << std::endl;
		if (LOG_PATHS)
		{
			std::list<vertex_t> path = wp->pathTo(13);
			std::cout << "Path from " << wp->getId() << " to 13: ";
			std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
			std::cout << std::endl;
		}
	}

	if (LOG_LOAD) std::cout << "Finished loading "<< counter << " waypoints...";

	return wps;
}


/* Djikstra: http://rosettacode.org/wiki/Dijkstra's_algorithm#C.2B.2B (MIT X11)
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and / or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions :
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//*/
void Waypoint::DijkstraComputePaths(vertex_t source,
	const adjacency_list_t &adjacency_list,
	std::vector<weight_t> &min_distance,
	std::vector<vertex_t> &previous)
{
	int n = adjacency_list.size();
	min_distance.clear();
	min_distance.resize(n, std::numeric_limits<double>::infinity());
	min_distance[source] = 0;
	previous.clear();
	previous.resize(n, -1);
	std::set<std::pair<weight_t, vertex_t> > vertex_queue;
	vertex_queue.insert(std::make_pair(min_distance[source], source));

	while (!vertex_queue.empty())
	{
		weight_t dist = vertex_queue.begin()->first;
		vertex_t u = vertex_queue.begin()->second;
		vertex_queue.erase(vertex_queue.begin());

		// Visit each edge exiting u
		const std::vector<neighbor> &neighbors = adjacency_list[u];
		for (std::vector<neighbor>::const_iterator neighbor_iter = neighbors.begin();
			neighbor_iter != neighbors.end();
			neighbor_iter++)
		{
			vertex_t v = neighbor_iter->target;
			weight_t weight = neighbor_iter->weight;
			weight_t distance_through_u = dist + weight;
			if (distance_through_u < min_distance[v]) {
				vertex_queue.erase(std::make_pair(min_distance[v], v));

				min_distance[v] = distance_through_u;
				previous[v] = u;
				vertex_queue.insert(std::make_pair(min_distance[v], v));

			}

		}
	}
}