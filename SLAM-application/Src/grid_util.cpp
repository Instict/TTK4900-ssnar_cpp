#include "grid_util.h"
#include <boost/graph/graphviz.hpp>
#include <iostream>

using vertex_descriptor = boost::graph_traits<boost::grid_graph<2>>::vertex_descriptor;

namespace TG::graph::grid::utility
{

vertex_descriptor convert(int row, int col)
{
	return { static_cast<std::size_t>(row), static_cast<std::size_t>(col) };
}

}

