#pragma once

#include <optional>
#include <set>
#include "grid_common.h"

using point = TG::graph::grid::point;
using filtered_grid = TG::graph::grid::filtered_grid;

namespace TG::graph::pathfinding
{

std::optional<std::vector<point>> solve(const filtered_grid& grid, point from, point to);

// Given a path returns the reduced version.
// A reduction means it only has the longest straight lines possible (any line will only have two points).
std::vector<point> reduce(const std::vector<point> path);

}