#include "path.h"

namespace NTNU::gui::base
{

path::path(int line_thickness, int circle_radius, sf::Color color) :
	lines_(line_thickness),
	circles_(),
	color_(color)
{
	set_color(color);

	circles_.setRadius(circle_radius);
	auto offset = static_cast<float>(circle_radius);
	circles_.setOrigin({ offset, offset });
}

void path::set(const std::vector<std::pair<float, float>> coords)
{
	auto it = coords.cbegin();
	auto it_ahead = it + 1;

	for (it, it_ahead; it_ahead != coords.cend(); it++, it_ahead++)
	{
		auto[x, y] = *it;
		auto[x_ahead, y_ahead] = *it_ahead;

		lines_.add({ x, y }, { x_ahead, y_ahead });
		circles_.add({ x_ahead, y_ahead });
	}
}

void path::set_color(sf::Color color)
{
	color_ = color;
	lines_.set_color(color);
	circles_.setColor(color);
}

int path::size() const
{
	return static_cast<int>(circles_.get().size());
}

std::pair<float, float> path::end_point() const
{
	// The last circle on the path should be positioned at the end point.
	auto last_circle = circles_.get().back().getPosition();

	return { last_circle.x, last_circle.y };
}

std::pair<float, float> path::next_point() const
{
	// Since a circle is not added at the point of the robot,
	// the "first circle is the next circle".
	auto next_circle = circles_.get().front().getPosition();

	return { next_circle.x, next_circle.y };
}

void path::clear()
{
	lines_.clear();
	circles_.clear();
}

void path::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(lines_, states);
	target.draw(circles_, states);
}

}
