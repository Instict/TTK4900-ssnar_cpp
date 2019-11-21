#pragma once

#include <memory>
#include "grid_common.h"
#include "grid.h"

using NTNU::graph::base::grid;

namespace NTNU::graph::grid {

class obstructable_grid : public grid
{
public:
	obstructable_grid(int rows, int cols);

	virtual void obstruct(int row, int col);
	virtual void unobstruct(int row, int col);
	bool is_obstructed(int row, int col) const;

	void reset_to(int rows, int cols);

	filtered_grid get_filtered_grid() const;

	virtual ~obstructable_grid() {};

private:
	vertex_set obstructed_;
	std::unique_ptr<filtered_grid> fgrid_;
};

}