#include "robots.h"
#include "robot.h"
#include "slam_message.h"
#include "slam_utility.h"
#include "grid_path_solver.h"
#include "imgui.h"
#include <iostream>


namespace NTNU::application::SLAM
{

	robots::robots() :
		robots_navigate_obstacle_(false),
		robots_navigate_(false),
		counter(0)
{
}

void robots::feed_message(const message & msg)
{
	auto robot_id = msg.sender();
	auto robot = get_robot(robot_id);
	if (!robot)
		robot = create_robot(robot_id);

	auto[msg_x, msg_y] = msg.robot_pos();
	auto[bot_x, bot_y] = robot->get()->getPosition();
	if (msg_x != bot_x || msg_y != bot_y)
	{
		robot->get()->setPosition(sf::Vector2f(msg_x, msg_y));
		auto context = std::make_tuple(robot_id, msg_x, msg_y);
		call_callback(robots_events::ROBOT_MOVED, context);
	} // TESTING
	if(msg_x == bot_x && msg_y == bot_y){
		counter++;
		if (counter > 50) {
		robot->get()->setPosition(sf::Vector2f(msg_x, msg_y));
		auto context = std::make_tuple(robot_id, msg_x, msg_y);
		call_callback(robots_events::ROBOT_IDLE, context);
		counter = 0;
		}

		//auto next_time = system_clock::now() + milliseconds(2000);
		//sleep_until(next_time);
	} // TESTING

	auto obstacles = msg.obstacles();
	for (const auto& obstacle : obstacles)
	{
		robot->get()->add_obstacle(sf::Vector2f(obstacle.x, obstacle.y));

		auto context = std::make_tuple(robot_id, obstacle.x, obstacle.y);
		call_callback(robots_events::ROBOT_FOUND_OBSTACLE, context);
	}

	set_fun([&]() {
		const ImVec2 btn_size{ 200, 0 };
		if (robots_.size() == 0)
			return;

		ImGui::Checkbox("Navigate Around Obstacles", &robots_navigate_obstacle_);
		if (ImGui::Button("Start navigation", btn_size)) {
			robots_navigate_ = true;

		}
		if (ImGui::Button("Stop navigation", btn_size)) {
			robots_navigate_ = false;
		}
		if (ImGui::BeginTabBar("Robots Tab Bar", ImGuiTabBarFlags_None))
		{
			for (const auto& entry : robots_)
			{
				const auto&[id, robot] = entry;
				if (ImGui::BeginTabItem(id.c_str()))
				{
					robot->show();

					ImGui::EndTabItem();
				}
			}

			ImGui::EndTabBar();

		}

	});
}

std::vector<std::pair<int, int>> robots::get_all_obstacles() const
{
	std::vector<std::pair<int, int>> obstacles;
	for (const auto& entry : robots_)
	{
		const auto&[_, robot] = entry;
		auto robot_obstacles = robot->get_obstacles();
		std::copy(robot_obstacles.begin(), robot_obstacles.end(), std::back_inserter(obstacles));
	}

	return obstacles;
}

std::vector<std::string> robots::get_all_robot_ids() const
{
	std::vector<std::string> ids;

	for (const auto& entry : robots_)
	{
		const auto&[id, _] = entry;
		ids.push_back(id);
	}

	return ids;
}

std::optional<std::pair<int, int>> robots::get_target(const std::string & robot_id) const
{
	auto robot = get_robot(robot_id);
	if (!robot)
		return std::nullopt;

	return robot->get()->get_target();
}

std::optional<std::pair<int, int>> robots::get_next_point(const std::string & robot_id) const
{
	auto robot = get_robot(robot_id);
	if (!robot)
		return std::nullopt;

	return robot->get()->get_next_on_path();
}

std::optional<std::pair<float, float>> robots::position(const std::string & robot_id) const
{
	auto robot = get_robot(robot_id);
	if (!robot)
		return std::nullopt;

	auto pos = robot->get()->getPosition();
	return std::make_pair(pos.x, pos.y);
}

void robots::update(sf::Time delta)
{
	for (const auto& entry : robots_)
	{
		const auto&[_, robot] = entry;
		robot->update(delta);
	}
}

bool robots::navigate_obstacle() const
{
	return robots_navigate_obstacle_;
}
bool robots::robot_navigate() {
	return robots_navigate_;
}

void robots::set_path(const std::string & robot_id, const std::vector<std::pair<float, float>> coords)
{
	auto robot = get_robot(robot_id);
	if (!robot)
		return;

	robot->get()->set_path(coords);
}

void robots::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	for (const auto& entry : robots_)
	{
		const auto&[_, robot] = entry;
		robot->draw(target, states);
	}
}

const std::unique_ptr<robot>* robots::create_robot(const std::string& robot_id)
{
	auto[robot_entry, robot_success] = robots_.emplace(std::make_pair(robot_id, std::make_unique<robot>(robot_id)));
	if (!robot_success)
		throw std::exception("Could not emplace robot.");

	auto& robot = robot_entry->second;
	robot->setColor(NTNU::application::SLAM::utility::get_random_color());

	robot->enable_callback(robot_events::POINTS_CLEARED, [&](std::any) {
		call_callback(robots_events::ROBOT_CLEARED_POINTS, robot->get_obstacles());
	});

	return &robot;
}

const std::unique_ptr<robot>* robots::get_robot(const std::string& robot_id) const
{
	if (auto it = robots_.find(robot_id); it != robots_.end())
		return &robots_.find(robot_id)->second;

	return nullptr;
}

}
