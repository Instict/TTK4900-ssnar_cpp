#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <functional>
#include "panel.h"
#include "callbacks.h"
#include "updatable.h"

using ipair = std::pair<int, int>;

namespace TG::gui::base
{

class window : public sf::Transformable, public TG::utility::callbacks<sf::Event::EventType, sf::Event>
{
public:

	window();
	virtual ~window() {};

	void add_panel(const TG::gui::panel::panel* panel);
	void add_drawable(const sf::Drawable* element);
	void add_updatable(TG::gui::base::updatable* thing);

	void remove_drawable(const sf::Drawable* element);

	void close();
	bool run();

private:

	std::vector<const TG::gui::panel::panel*> panels_;
	std::vector<const sf::Drawable*> elements_;
	std::vector<TG::gui::base::updatable*> updatables_;

	sf::RenderWindow window_;
	sf::Clock clk_;
	sf::Font font_;

	std::string font_path_;
	std::string window_title_;
	ipair window_size_;

	void check_events();
};

}

