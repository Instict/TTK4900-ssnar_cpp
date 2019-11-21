#pragma once

#include <string>
#include "MQTT.h"

namespace NTNU::application::SLAM
{

struct robot_simulation_config {
	std::string broker_address;
	std::string publish_topic;
	int crawl_speed; // How much the simulated robot can move between each call to run
	int obstacle_spread; // How far new obstacles can be placed relative to the last generated obstacle
};

class robot_simulation
{
public:
	robot_simulation(const struct robot_simulation_config& config);

	void run();

	~robot_simulation() {};

private:
	NTNU::networking::protocols::MQTT::MQTT mqtt_;
	std::string topic_;

	int x_;
	int y_;
	int obs_x_;
	int obs_y_;

	int crawl_speed_;
	int obstacle_spread_;
};

}
