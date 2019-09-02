#pragma once

#include <boost/graph/grid_graph.hpp>
#include <boost/graph/filtered_graph.hpp>
#include <boost/graph/properties.hpp>
#include <boost/unordered_set.hpp>
#include <utility>
#include "vertex_hash.h"

namespace TG::graph::grid
{

// Points are on the form (row, col)
using point = std::pair<int, int>;

using bgrid = boost::grid_graph<2>;
using vertex_hash = TG::graph::grid::vertex_hash;
using vertex_descriptor = boost::graph_traits<bgrid>::vertex_descriptor;

typedef boost::unordered_set<vertex_descriptor, vertex_hash> vertex_set;
typedef boost::vertex_subset_complement_filter<bgrid, vertex_set>::type filtered_grid;

}
