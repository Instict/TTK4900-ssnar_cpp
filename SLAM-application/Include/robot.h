#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include "animated_line.h"
#include "panel.h"
#include "circles.h"
#include "updatable.h"
#include "path.h"
#include "callbacks.h"

namespace NTNU::application::SLAM
{

enum class robot_events
{
	POINTS_CLEARED
};

class robot :
	public sf::Drawable,
	public sf::Transformable,
	public NTNU::gui::panel::panel,
	public NTNU::gui::base::updatable,
	public NTNU::utility::callbacks<enum robot_events>
{
public:
	robot(const std::string& name);
	~robot() {};

	void add_obstacle(const sf::Vector2f& pos);
	std::vector<std::pair<int, int>> get_obstacles() const;

	void setOffset(sf::Vector2f offset);

	void setRadius(float radius);
	float getRadius() const;

	sf::Color getColor() const;
	void setColor(sf::Color color);

	void set_path(const std::vector<std::pair<float, float>>& coords);

	std::optional<std::pair<float, float>> get_target() const;
	std::optional<std::pair<float, float>> get_next_on_path() const;

	void clear();

	void update(sf::Time delta) override;
	void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

private:
	std::string name_;
	float radius_;
	sf::Color color_;
	NTNU::gui::collections::circles circles_;
	sf::CircleShape robot_marker_;
	std::list<animated_line> animated_lines_;
	NTNU::gui::base::path path_;
	sf::Texture robot_texture_;
	sf::Sprite robot_sprite_;
	float robot_scaling_;
};

}

