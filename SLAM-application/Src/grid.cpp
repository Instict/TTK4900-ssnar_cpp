#include "grid.h"

#include <iostream>

using namespace boost;

namespace NTNU::graph::base
{

grid::grid(int rows, int cols) :
	grid_()
{
	reset_to(rows, cols);
}

void grid::reset_to(int rows, int cols)
{
	grid_ = std::make_unique<NTNU::graph::grid::bgrid>(array<std::size_t, 2>({ std::size_t(rows), std::size_t(cols) }));
}

int grid::rows() const
{
	return grid_->length(0);
}

int grid::columns() const
{
	return grid_->length(1);
}

}
