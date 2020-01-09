#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include "panel.h"
#include "circles.h"
#include "updatable.h"
#include "callbacks.h"
#include "robot.h"

namespace NTNU::application::SLAM
{

class message;

enum class robots_events
{
	ROBOT_MOVED,				// Context: Tuple (id, x, y)
	ROBOT_FOUND_OBSTACLE,		// Context: Tuple (id, x, y)
	ROBOT_CLEARED_POINTS,		// Context: std::vector<pair<int, int>> of all obstacles being cleared
	ROBOT_IDLE
};

class robots :
	public sf::Drawable,
	public sf::Transformable,
	public NTNU::gui::panel::panel,
	public NTNU::gui::base::updatable,
	public NTNU::utility::callbacks<robots_events>
{
public:
	robots();
	~robots() {};

	void feed_message(const message& msg);

	std::vector<std::pair<int, int>> get_all_obstacles() const;
	std::vector<std::string> get_all_robot_ids() const;

	std::optional<std::pair<int, int>> get_target(const std::string& robot_id) const;
	std::optional<std::pair<int, int>> get_next_point(const std::string& robot_id) const;
	std::optional<std::pair<float, float>> position(const std::string& robot_id) const;

	void update(sf::Time delta) override;
	bool navigate() const;

	void set_path(const std::string& robot_id, const std::vector<std::pair<float, float>> coords);

	void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

private:
	const std::unique_ptr<robot>* create_robot(const std::string& robot_id);
	const std::unique_ptr<robot>* get_robot(const std::string& robot_id) const;

	std::map<std::string, std::unique_ptr<robot>> robots_;

	bool robots_navigate_;
};

}

