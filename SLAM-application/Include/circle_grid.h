#pragma once

#include "circles.h"
#include <SFML/Graphics.hpp>
#include <memory>

namespace TG::gui::elements
{

class circle_grid : public sf::Transformable, public sf::Drawable
{
public:
	circle_grid(int rows, int columns, int separation, sf::Color color = sf::Color::Green);

	void set_color(sf::Color color);
	void change_color(sf::Color color, int row, int column);
	void set_alpha(int alpha);

	void reset_to(int rows, int columns, int separation, sf::Color color = sf::Color::Green);
	sf::Vector2f size() const;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

protected:
	int rows_;
	int columns_;
	int separation_;

private:
	std::unique_ptr<gui::collections::circles> circles_;
};

}

