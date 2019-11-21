#pragma once

#include "obstructable_grid.h"
#include "circle_grid.h"
#include "panel.h"
#include "callbacks.h"
#include <utility>
#include <map>
#include <SFML/Graphics.hpp>

namespace NTNU::application::SLAM
{

enum class slam_grid_events
{
	GRID_GEOMETRY_CHANGED,
	NODE_OBSTRUCTED,
};

class slam_grid :
	public NTNU::gui::panel::panel,
	public NTNU::graph::grid::obstructable_grid,
	public NTNU::gui::elements::circle_grid,
	public NTNU::utility::callbacks<slam_grid_events>
{
public:
	slam_grid(int rows, int cols, int separation, sf::Color color = sf::Color::Green);

	int separation() const;

	void set_alpha(int alpha);

	void reset_to(int rows, int cols, int separation);

	void obstruct(int row, int col) override;
	void unobstruct(int row, int col) override;

	virtual ~slam_grid() {};

private:
	sf::Color color_;
	int separation_;
};

}

