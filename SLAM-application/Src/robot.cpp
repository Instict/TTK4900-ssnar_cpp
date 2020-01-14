#include "robot.h"
#include <iostream>

#include "imconfig-SFML.h"
#include <imgui.h>
#include "imgui-SFML.h"
#include <Thor/Graphics.hpp>
#include <Thor/Math.hpp>
#include <Thor/Animations.hpp>
#include <Thor/Vectors.hpp>

namespace NTNU::application::SLAM
{

robot::robot(const std::string& name) :
	name_(name),
	radius_(10.f),
	color_(),
	circles_(),
	robot_marker_(15.0f, 5),
	animated_lines_(),
	path_(),
	robot_scaling_(0.025)
{
	color_.a = 200;

	robot_marker_.setFillColor(color_);
	robot_marker_.setOrigin({ 15.f, 15.f });

	circles_.setRadius(2.f);
	circles_.setColor(color_);
	circles_.setOrigin({ 2.f, 2.f });

	path_.set_color(color_);

	if (!robot_texture_.loadFromFile("Media/robot.png"))
	{
		std::cout << "Could not load robot texture.\n";
	}
	else
	{
		robot_sprite_.setTexture(robot_texture_);
		auto robot_size = robot_texture_.getSize();
		robot_sprite_.setOrigin(robot_size.x / 2, robot_size.y / 2);

		robot_sprite_.setScale(robot_scaling_, robot_scaling_);
	}

	set_fun([&]() {
		{
			ImGui::Text(name_.c_str());
		}
		{
			float radius = radius_;
			if (ImGui::SliderFloat("Radius", &radius, 1.0f, 10.0f))
			{
				setRadius(radius);
			}
		}
		{
			float rotation = thor::toRadian(getRotation());
			if (ImGui::SliderAngle("Tower Angle", &rotation, 0.f, 359.0f))
			{
				setRotation(thor::toDegree(rotation));
			}
		}
		{
			if (ImGui::SliderFloat("Robot Size", &robot_scaling_, 0.005f, 0.05f))
			{
				robot_sprite_.setScale(robot_scaling_, robot_scaling_);
			}
		}
		{
			if (ImGui::TreeNode("Color"))
			{
				ImVec4 color = ImVec4(color_);
				ImGui::SetColorEditOptions(ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
				if (ImGui::ColorPicker4("Color", (float*)&color))
				{
					setColor(sf::Color(color));
				}
				ImGui::TreePop();
			}
		}
		{
			if (ImGui::Button("Reset"))
			{
				call_callback(robot_events::POINTS_CLEARED, name_);
				clear();
			}
			ImGui::SameLine();
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Points: %d", circles_.get().size());
		}
	});
}

void robot::update(sf::Time delta)
{
	auto it = animated_lines_.begin();
	while (it != animated_lines_.end())
	{
		if (it->update(delta) == false)
		{
			animated_lines_.erase(it++);
		}
		else
		{
			++it;
		}
	}
}

void robot::add_obstacle(const sf::Vector2f& pos)
{
	circles_.add({ pos.x, pos.y });
	auto robot_pos = getPosition();

	auto diff = pos - robot_pos;
	auto vec = thor::PolarVector2(diff);
	auto dist = thor::length(vec);
	//std::cout << "Length: " << dist << '\n';

	// Visual hack: Change alpha based on distance from robot at point of add
	int alpha = 0;
	if (dist < 400)
	{
		alpha = 255;
	}
	else if (dist > 700)
	{
		alpha = 10;
	}
	else
	{
		dist -= 400;
		// Range is now 0 to 300
		float scale = dist / 300;
		// Want lower ranges to yield higher alphas (reversal)
		scale = 1.0 - scale;
		// Alpha is 
		alpha = 255 * scale;
		if (alpha < 10)
			alpha = 10;
	}
	//std::cout << "Alpha: " << alpha << '\n';
	sf::Color col = getColor();
	col.a = alpha;
	circles_.setColor(col, circles_.get().size() - 1);
	// Visual hack end

	// Look at the added obstacle.
	setRotation(vec.phi);

	// Add an animated line to point at the added obstacle.

	animated_line line(getPosition(), pos, color_, sf::milliseconds(350));
	animated_lines_.push_back(line);
}

std::vector<std::pair<int, int>> robot::get_obstacles() const
{
	std::vector<std::pair<int, int>> obstacles;
	for (const auto& obs : circles_.get())
	{
		auto pos = obs.getPosition();
		obstacles.emplace_back(pos.x, pos.y);
	}
	return obstacles;
}

void robot::setOffset(sf::Vector2f offset)
{
	Transformable::setPosition(offset);
}

void robot::setRadius(float radius)
{
	if (radius < 1)
	{
		return;
	}

	radius_ = radius;
	circles_.setRadius(radius);
}

float robot::getRadius() const
{
	return radius_;
}

sf::Color robot::getColor() const
{
	return color_;
}


void robot::setColor(sf::Color color)
{
	color_ = color;
	auto opaque_color = color_;
	opaque_color.a = 255;

	robot_marker_.setFillColor(opaque_color);
	circles_.setColor(color_);
	path_.set_color(color_);
	robot_sprite_.setColor(color_);
}

void robot::set_path(const std::vector<std::pair<float, float>>& coords)
{
	path_.clear();
	path_.set(coords);
}

std::optional<std::pair<float, float>> robot::get_target() const
{
	if (path_.size() == 0)
		return std::nullopt;

	return path_.end_point();
}

std::optional<std::pair<float, float>> robot::get_next_on_path() const
{
	if (path_.size() == 0)
		return std::nullopt;

	return path_.next_point();
}

void robot::clear()
{
	circles_.clear();
}

void robot::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// We want the obstacles to not accept the global passed down
	// transform, but not be affected by own transform...

	target.draw(circles_, states);
	target.draw(path_, states);

	for (const auto & l : animated_lines_)
	{
		target.draw(l, states);
	}

	// ... so we apply our own transform here.
	states.transform *= getTransform();
	//target.draw(robot_marker_, states);
	target.draw(robot_sprite_, states);
}

}
