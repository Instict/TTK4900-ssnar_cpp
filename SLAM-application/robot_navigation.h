#pragma once
#include <string>
#include "MQTT.h"

namespace NTNU::application::SLAM {
	struct robot_navigation_config {
		std::string broker_address;
		std::string publish_topic;
	};

	class robot_navigation {
	public:
		robot_navigation(const struct robot_navigation_config& config);

		void run();

		~robot_navigation() {};

	private:
		NTNU::networking::protocols::MQTT::MQTT mqtt_;
		std::string topic_;


	};
}

