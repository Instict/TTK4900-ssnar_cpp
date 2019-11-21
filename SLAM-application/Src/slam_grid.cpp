#include "slam_grid.h"
#include "imgui.h"
#include <iostream>

using NTNU::graph::grid::obstructable_grid;
using NTNU::gui::elements::circle_grid;

namespace NTNU::application::SLAM
{

slam_grid::slam_grid(int rows, int cols, int separation, sf::Color color) :
	obstructable_grid(rows, cols),
	circle_grid(rows, cols, separation, color),
	color_(color),
	separation_(separation)
{
	reset_to(rows, cols, separation);

	set_fun([&]() {
		bool need_reset = false;

		need_reset |= ImGui::SliderInt("Rows", &rows_, 4, 100, "%d");
		need_reset |= ImGui::SliderInt("Columns", &columns_, 4, 100, "%d");
		need_reset |= ImGui::SliderInt("Separation", &separation_, 1, 100, "%d");

		static int alpha = 100;
		if (ImGui::SliderInt("Grid Alpha", &alpha, 0, 255, "%d"))
		{
			set_alpha(alpha);
		}

		if (need_reset)
			reset_to(rows_, columns_, separation_);
	});
}

int slam_grid::separation() const
{
	return separation_;
}

void slam_grid::set_alpha(int alpha)
{
	if (alpha < 0 || alpha > 255)
		return;

	color_.a = alpha;

	circle_grid::set_alpha(alpha);
}

void slam_grid::reset_to(int rows, int cols, int separation)
{
	rows_ = rows;
	columns_ = cols;
	separation_ = separation;

	circle_grid::reset_to(rows, cols, separation);
	auto size = circle_grid::size();

	circle_grid::setOrigin({ size.x / 2.0f, size.y / 2.0f });
	circle_grid::setPosition({ 0.f, 0.f });

	circle_grid::set_color(color_);

	obstructable_grid::reset_to(rows, cols);

	call_callback(slam_grid_events::GRID_GEOMETRY_CHANGED, std::any());
}

void slam_grid::obstruct(int row, int col)
{
	if (!obstructable_grid::is_obstructed(row, col))
	{
		auto obstruct_color = sf::Color::Red;
		obstruct_color.a = color_.a;

		circle_grid::change_color(obstruct_color, row, col);
		obstructable_grid::obstruct(row, col);
		call_callback(slam_grid_events::NODE_OBSTRUCTED, (row, col));
	}
}

void slam_grid::unobstruct(int row, int col)
{
	circle_grid::change_color(color_, row, col);
	obstructable_grid::unobstruct(row, col);
}

}
