#include "grid_common.h"
#include "obstructable_grid.h"
#include "grid_util.h"

using TG::graph::grid::utility::convert;

namespace TG::graph::grid {
obstructable_grid::obstructable_grid(int rows, int cols) :
	TG::graph::base::grid(rows, cols)
{
	reset_to(rows, cols);
}
void obstructable_grid::obstruct(int row, int col)
{
	obstructed_.insert(convert(row, col));
}
void obstructable_grid::unobstruct(int row, int col)
{
	obstructed_.erase(convert(row, col));
}
bool obstructable_grid::is_obstructed(int row, int col) const
{
	return obstructed_.find(convert(row, col)) != obstructed_.end();
}

void obstructable_grid::reset_to(int rows, int cols)
{
	grid::reset_to(rows, cols);
	obstructed_.clear();
	fgrid_ = std::make_unique<filtered_grid>(boost::make_vertex_subset_complement_filter(*grid_.get(), obstructed_));
}

filtered_grid obstructable_grid::get_filtered_grid() const
{
	return *fgrid_.get();
}
}
