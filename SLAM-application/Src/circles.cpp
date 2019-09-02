#include "circles.h"
#include <iostream>


namespace TG::gui::collections
{

circles::circles() :
	circles_(),
	numPoints_(10),
	radius_(),
	color_()
{
}

void circles::add(std::pair<float, float> pos)
{
	auto circle = sf::CircleShape(radius_, numPoints_);
	circle.setPosition({ pos.first, pos.second });
	circle.setFillColor(color_);
	circles_.push_back(circle);
}

void circles::setRadius(float radius)
{
	radius_ = radius;

	for (auto & c : circles_)
	{
		c.setRadius(radius_);
	}
}

void circles::setNumPoints(int numPoints)
{
	numPoints_ = numPoints;

	for (auto & c : circles_)
	{
		c.setPointCount(numPoints_);
	}
}

void circles::setColor(sf::Color color)
{
	color_ = color;

	for (auto & c : circles_)
	{
		c.setFillColor(color);
	}
}

void circles::setColor(sf::Color color, int index)
{
	if (index < circles_.size())
		circles_.at(index).setFillColor(color);
}

void circles::set_alpha(int alpha)
{
	for (auto & c : circles_)
	{
		auto color = c.getFillColor();
		color.a = alpha;
		c.setFillColor(color);
	}
}

sf::Color circles::color() const
{
	return color_;
}

const std::vector<sf::CircleShape>& circles::get() const
{
	return circles_;
}

void circles::clear()
{
	circles_.clear();
}

void circles::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	for (const auto& c : circles_)
	{
		target.draw(c, states);
	}
}

}

