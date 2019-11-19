#include "simulation_panel.h"
#include "window.h"
#include "imgui.h"
#include <iostream>	//	TESTING


namespace TG::gui::panel 
{
	simulation_panel::simulation_panel()
	{
	//set_robot_sim_enable(false);
		std::cout << "sim_pan enable:1 " << robot_sim_ << std::endl;
		bool robot_sim_enable = get_robot_sim_enable();
		set_robot_sim_enable(&robot_sim_enable);
		std::cout << "sim_pan enable:2 " << robot_sim_ << std::endl;
	
	set_fun([&robot_sim_enable]() {
		ImGui::Checkbox("Simulation On", &robot_sim_enable);
		});
	}
	
	void simulation_panel::set_robot_sim_enable(bool robot_sim) {
		robot_sim_ = robot_sim;
	}
	bool simulation_panel::get_robot_sim_enable() {
		return robot_sim_;
	}
}

