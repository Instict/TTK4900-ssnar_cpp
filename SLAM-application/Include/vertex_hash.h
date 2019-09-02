#pragma once

#include <boost/graph/properties.hpp>
#include <boost/graph/grid_graph.hpp>
#include <boost/container_hash/hash.hpp>

namespace TG::graph::grid
{

// Uses both indices ({x, y}) as seed for hash.
struct vertex_hash
{
	std::size_t operator()(boost::graph_traits<boost::grid_graph<2>>::vertex_descriptor const& u) const {
		std::size_t seed = 0;
		boost::hash_combine(seed, u[0]);
		boost::hash_combine(seed, u[1]);
		return seed;
	}
};

}
