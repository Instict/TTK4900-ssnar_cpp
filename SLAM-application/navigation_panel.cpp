#include "navigation_panel.h"
#include "window.h"
#include "imgui.h"
#include <iostream>		//	testing

namespace NTNU::gui::panel {

	navigation_panel::navigation_panel() :
		robot_nav_(false),
		robot_nav_obs_(false),
		manual_target_({ 0, 0 }),
		robot_position_({ 0, 0 }),
		x(0),
		y(0),
		x_(0),
		y_(0)
	{
		set_fun([&]() {
			const ImVec2 btn_size{ 200, 0 };
			static int square_size_double{ 500 };
			static int square_size{ 250 };
			ImGui::Checkbox("Navigation On", &robot_nav_);
			ImGui::Checkbox("Navigate around obstacles", &robot_nav_obs_);
			if (ImGui::Button("Start navigation", btn_size)) {
				robot_nav_ = true;
	
			}

			ImGui::Text("Target: X[%d], Y[%d]", manual_target_.first, manual_target_.second);
			ImGui::Text("Position : X[%d], Y[%d]", x, y);
			});
	}

	void navigation_panel::run() {

	}
	void navigation_panel::set_robot_nav_enable(bool robot_nav) {
		robot_nav_ = robot_nav;
	}

	bool navigation_panel::get_robot_nav_enable() {
		return robot_nav_;
	}
	void navigation_panel::set_robot_nav_obstacle_enable(bool robot_nav_obs) {
		robot_nav_obs_ = robot_nav_obs;
	}

	bool navigation_panel::get_robot_nav_obstacle() {
		return robot_nav_obs_;
	}
	ipair navigation_panel::get_manual_target() {
		return manual_target_;
	}
	void navigation_panel::set_manual_target(ipair manual_target) {
		manual_target_ = manual_target;
	}
	void navigation_panel::set_robot_position(int x, int y) {
		x_ = x;
		y_ = y;
		robot_position_ = { x_, y_ };
	}
	ipair navigation_panel::get_robot_position() {
		return robot_position_;
	}
	void navigation_panel::something() {
		if (manual_target_ == robot_position_) {
			std::cout << "Reached destination, new waypoint\n";
		}
		else {
			std::cout << "Moving to destination\n";
			std::cout << "robot { " << robot_position_.first << " , " << robot_position_.second << " }\n";
		}

	}
}
