#include "robot_navigation.h"
#include "slam_utility.h"
#include "slam_message.h"
#include <iostream>
#include "window.h"
#include "imgui.h"
#include "robots.h"
#include "robot.h"
#include "grid_path_solver.h"
namespace NTNU::gui::panel {

	robot_navigation::robot_navigation() :
		robot_nav_(false),
		manual_target_({ 0, 0 }),
		robot_position_({ 0, 0 }),
		x(0),
		y(0),
		x_(0),
		y_(0)
	{
		set_fun([&]() {
			static int offset = 0;
			const ImVec2 btn_size{ 200, 0 };
			static int square_size_double{ 500 };
			static int square_size{ 250 };
			ImGui::Checkbox("Navigation On", &robot_nav_);
			if (ImGui::Button("Start navigation", btn_size)) {
				manual_target_ = { 10, 10 };
				robot_nav_ = true;
			}
			if (ImGui::Button("Reset waypoint", btn_size)) {
				x = 0;
				y = 0;

			}
			if (ImGui::Button("Next target", btn_size)) {
				x += 50;
				y += 50;
				manual_target_ = { x , y  };
				if (x > 600) {
					x = 0;
				}
			}

			ImGui::Text("Target: X[%d], Y[%d]", manual_target_.first, manual_target_.second);
			ImGui::Text("Position : X[%d], Y[%d]", x, y);
			});
	}
	
	void robot_navigation::run() {
		
	}
	void robot_navigation::set_robot_nav_enable(bool robot_nav) {
		robot_nav_ = robot_nav;
	}
	bool robot_navigation::get_robot_nav_enable() {
		return robot_nav_;
	}
	ipair robot_navigation::get_manual_target() {
		return manual_target_;
	}
	void robot_navigation::set_manual_target(ipair manual_target) {
		manual_target_ = manual_target;
	}
	void robot_navigation::set_robot_position(int x, int y) {
		x_ = x;
		y_ = y;
		robot_position_ = { x_, y_ };
		std::cout << "does this shit even work? { " << x_ << ": " << y_ << " }\n";
	}
	ipair robot_navigation::get_robot_position() {
		return robot_position_;
	}
	void robot_navigation::something() {
		if (manual_target_ == robot_position_) {
			std::cout << "Reached destination, new waypoint\n";
		}
		else {
			std::cout << "Moving to destination\n";
			std::cout << "robot { " << robot_position_.first << " , " << robot_position_.second << " }\n";
		}

	}
}