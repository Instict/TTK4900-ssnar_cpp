#pragma once
#include <string>
#include "MQTT.h"

namespace NTNU::application::SLAM {
	struct robot_navigation_config {
		std::string broker_address;
		std::string publish_topic;
		int crawl_speed;
		int obstacle_spread;
	};
	class robot_navigation {
		robot_navigation(const struct robot_navigation_config& config);

		void run();

		~robot_navigation() {};

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

