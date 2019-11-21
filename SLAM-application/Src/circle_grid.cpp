#include "circle_grid.h"
#include <iostream>

using NTNU::gui::collections::circles;

namespace NTNU::gui::elements
{

circle_grid::circle_grid(int rows, int columns, int separation, sf::Color color) :
	rows_(rows),
	columns_(columns),
	separation_(separation)
{
	reset_to(rows, columns, separation, color);
}

void circle_grid::set_color(sf::Color color)
{
	circles_->setColor(color);
}

void circle_grid::change_color(sf::Color color, int row, int column)
{
	circles_->setColor(color, (row * columns_) + column);
}

void circle_grid::set_alpha(int alpha)
{
	circles_->set_alpha(alpha);
}

void circle_grid::reset_to(int rows, int columns, int separation, sf::Color color)
{
	circles_ = std::make_unique<circles>();
	circles_->setColor(color);
	separation_ = separation;

	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < columns; col++)
		{
			auto x = col * separation;
			auto y = row * separation;
			circles_->add({ col * separation, row * separation });
		}
	}

	circles_->setRadius(static_cast<float>(separation / 2));
}

void circle_grid::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	for (const auto & c : circles_->get())
	{
		target.draw(c, states);
	}
}

sf::Vector2f circle_grid::size() const
{
	return {
		static_cast<float>(columns_ * separation_),
		static_cast<float>(rows_ * separation_),
	};
}


}
