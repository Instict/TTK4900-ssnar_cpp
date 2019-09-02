#pragma once
#include "panel.h"
#include "callbacks.h"

namespace TG::gui::panel
{
enum class control_panel_event
{
	QUIT
};

class control_panel :
	public panel,
	public TG::utility::callbacks<enum control_panel_event>
{
public:
	control_panel();
	void embed_panel(const TG::gui::panel::panel* panel, const std::string& title);

	~control_panel() {};

private:
	std::vector<std::pair<const TG::gui::panel::panel*, std::string>> panels_;
};

}
