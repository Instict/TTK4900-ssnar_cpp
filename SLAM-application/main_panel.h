#pragma once
#include "panel.h"

namespace NTNU::gui::panel {
	class main_panel :
		public panel
	{
	public:
		main_panel();
		~main_panel();
	private:
		bool yaxis_flip_;

	};

}

