#include "stdafx.h"
#include "Helper.h"


Helper::Helper()
{
}


Helper::~Helper()
{
}


std::vector<std::string>  &Helper::split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		if (!item.empty())
			elems.push_back(item);
	}
	if (!elems.empty())
	{
		if (elems.back().back() == '\r')
		{
			elems.back().erase(elems.back().size() - 1);
		}
	} //*/
	return elems;
}

std::vector<std::string> Helper::split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

std::string Helper::costToStr(const cost& _c)
{
	std::string bla = "";
	bla += std::to_string((int)_c.gold) + "g-";
	bla += std::to_string((int)_c.iron) + "i-";
	bla += std::to_string((int)_c.stone) + "s-";
	bla += std::to_string((int)_c.wood) + "w";
	return bla;
}

bool Helper::isEnoughRes(const cost& _res, const cost& _costs)
{
	if (_res.gold < _costs.gold) return false;
	if (_res.iron < _costs.iron) return false;
	if (_res.iron < _costs.stone) return false;
	if (_res.wood < _costs.wood) return false;
	return true;
}

void Helper::removeRes(cost& _res, const cost& _costs)
{
	_res.gold -= _costs.gold;
	_res.iron -= _costs.iron;
	_res.stone -= _costs.stone;
	_res.wood -= _costs.wood;
}

bool Helper::hasReached(sf::Vector2f _playerposition, sf::Vector2f _targetposition, float _velocity)
{
	sf::Vector2f dif = _playerposition - _targetposition;
	return ((abs(dif.x) + abs(dif.y)) <= (_velocity));
}

/* Djikstra: http://rosettacode.org/wiki/Dijkstra's_algorithm#C.2B.2B (MIT X11)
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and / or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions :
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//*/
void Helper::DijkstraComputePaths(vertex_t source,
	const adjacency_list_t &adjacency_list,
	std::vector<weight_t> &min_distance,
	std::vector<vertex_t> &previous)
{
	int n = adjacency_list.size();
	min_distance.clear();
	min_distance.resize(n, max_weight);
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

std::list<Helper::vertex_t> Helper::DijkstraGetShortestPathTo(
	vertex_t vertex, const std::vector<vertex_t> &previous)
{
	std::list<vertex_t> path;
	for (; vertex != -1; vertex = previous[vertex])
		path.push_front(vertex);
	return path;
}