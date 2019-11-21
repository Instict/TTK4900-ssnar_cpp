#pragma once

#include <string>
#include <vector>
#include <optional>
#include <variant>

namespace NTNU::application::SLAM
{

class message
{
public:
	struct position {
		int16_t x;
		int16_t y;
	};

	struct payload {
		position robot;
		std::vector<position> obstacles;
	};

	message(std::string sender = "");

	void set_payload(std::string payload);
	void set_payload(position pos);
	void set_payload(position pos, std::vector<position> obstacles);

	bool is_valid() const;

	std::vector<std::byte> serialize() const;

	std::string sender() const;

	position robot_pos() const;
	std::vector<position> obstacles() const;

	virtual ~message() {};

private:
	bool valid_;
	std::string sender_;

	position robot_;
	std::vector<position> obstacles_;

	bool sanity_check(int size);
};

}
