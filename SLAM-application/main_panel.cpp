#include "main_panel.h"
#include "window.h"
#include "imgui.h"
/*
namespace NTNU::gui::panel {
	main_panel::main_panel() :
		yaxis_flip_(true)
	{
		NTNU::gui::base::window win;
		auto scale = win.getScale();
		scale.y *= -1.0;
		win.setScale(scale);
		win.add_drawable(&grid);
		win.add_drawable(&robots);
		set_fun([&win, &yaxis_flip_]() {
			if (ImGui::Checkbox("Flip Y-axis", &yaxis_flip_)) {
				auto scale = win.getScale();
				auto y = abs(scale.y);
				scale.y = yaxis_flip ? -y : y;
				win.setScale(scale);
			}

			float rotation = thor::toRadian(win.getRotation());
			if (ImGui::SliderAngle("Angle", &rotation, 0.f, 359.0f))
			{
				win.setRotation(thor::toDegree(rotation));
			}
			});
	}

}
*/