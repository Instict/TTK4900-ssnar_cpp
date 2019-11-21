#pragma once

#include <utility>
#include <vector>
#include <SFML/Graphics.hpp>

namespace NTNU::gui::collections
{

class circles : public sf::Drawable, public sf::Transformable
{
public:

	circles();

	void add(std::pair<float, float> pos);

	void setRadius(float radius);
	void setNumPoints(int points);
	void setColor(sf::Color color);
	void setColor(sf::Color color, int index);
	void set_alpha(int alpha);

	sf::Color color() const;

	const std::vector<sf::CircleShape>& get() const;

	void clear();
	
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	std::vector<sf::CircleShape> circles_;
	int numPoints_;
	int radius_;
	sf::Color color_;
};

}

