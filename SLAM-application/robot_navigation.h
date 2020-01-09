#pragma once
#include <string>
#include "MQTT.h"
#include "panel.h"
#include <optional>
using ipair = std::pair<int, int>;


namespace NTNU::gui::panel {
	struct robot_navigation_config {
		std::string broker_address;
		std::string publish_topic;
		int crawl_speed;
		int obstacle_spread;
	};
	class robot_navigation : 
		public panel {
	public:
		robot_navigation();

		void run();

		~robot_navigation() {};
		void set_robot_nav_enable(bool robot_nav);
		bool get_robot_nav_enable();
		ipair get_manual_target();
		void set_manual_target(ipair manual_target);
		void set_robot_position(int x, int y);
		ipair get_robot_position();
		void something();

	private:
		ipair robot_position_;
		int x_;
		int y_;
		int x;
		int y;
		bool robot_nav_;
		ipair manual_target_;

	};
}

