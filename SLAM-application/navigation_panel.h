#pragma once
#include "panel.h"
#include <stdbool.h>
#include <optional>
using ipair = std::pair<int, int>;


namespace NTNU::gui::panel {

	class navigation_panel :
		public panel {
	public:
		navigation_panel();

		void run();

		~navigation_panel() {};
		void set_robot_nav_enable(bool robot_nav);
		bool get_robot_nav_enable();
		ipair get_manual_target();
		void set_manual_target(ipair manual_target);
		void set_robot_position(int x, int y);
		ipair get_robot_position();
		void something();
		void set_robot_nav_obstacle_enable(bool robot_nav_obs);
		bool get_robot_nav_obstacle();

	private:
		ipair robot_position_;
		int x_;
		int y_;
		int x;
		int y;
		bool robot_nav_;
		bool robot_nav_obs_;
		ipair manual_target_;

	};
}
