#pragma once

#include <boost/graph/grid_graph.hpp>
#include "grid.h"
#include "obstructable_grid.h"

namespace TG::graph::grid::utility
{

boost::graph_traits<boost::grid_graph<2>>::vertex_descriptor convert(int row, int col);

}