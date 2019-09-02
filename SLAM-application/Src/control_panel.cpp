#include "control_panel.h"
#include "imgui.h"

namespace TG::gui::panel
{

control_panel::control_panel()
{
	set_fun([&] {
		ImGui::Begin("Control Panel");

		if (ImGui::Button("Quit Progam"))
		{
			call_callback(control_panel_event::QUIT, {});
		}

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		if (ImGui::BeginTabBar("Control Panel Tab Bar", ImGuiTabBarFlags_None)) {
			for (const auto & [panel, title] : panels_)
			{
				if (ImGui::BeginTabItem(title.c_str()))
				{
					panel->show();
					ImGui::EndTabItem();
				}
			}
			ImGui::EndTabBar();
		}

		ImGui::End();
	});
}

void control_panel::embed_panel(const TG::gui::panel::panel * panel, const std::string & title)
{
	panels_.emplace_back(std::make_pair(panel, title));
}

}
