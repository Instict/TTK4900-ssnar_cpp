#include "simulation_panel.h"
#include "window.h"
#include "imgui.h"
#include <iostream>	//	TESTING


namespace NTNU::gui::panel 
{
	simulation_panel::simulation_panel() :
		robot_sim_(false)
	{
		set_fun([&]() {
			ImGui::Checkbox("Simulation On", &robot_sim_);
		});

	}
	
	void simulation_panel::set_robot_sim_enable(bool robot_sim) {
		robot_sim_ = robot_sim;
	}
	bool simulation_panel::get_robot_sim_enable() {
		return robot_sim_;
	}
}

