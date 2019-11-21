#pragma once

#include "grid_common.h"

namespace NTNU::graph::base
{

class grid
{

public:
	grid(int rows, int cols);
	virtual ~grid() {};

	void reset_to(int rows, int cols);

	int rows() const;
	int columns() const;

protected:
	std::unique_ptr<NTNU::graph::grid::bgrid> grid_;
};

}
