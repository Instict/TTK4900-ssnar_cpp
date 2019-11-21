#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "circles.h"
#include "lines.h"

namespace NTNU::gui::base
{

class path : public sf::Drawable, public sf::Transformable
{
public:
	path(int line_thickness = 1, int circle_radius = 5, sf::Color = sf::Color::Green);

	void set(const std::vector<std::pair<float, float>> coords);
	void set_color(sf::Color color);

	int size() const;

	std::pair<float, float> next_point() const;
	std::pair<float, float> end_point() const;

	void clear();

	~path() {};

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	gui::collections::circles circles_;
	gui::collections::lines lines_;
	sf::Color color_;
};

}
