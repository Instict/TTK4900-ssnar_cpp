#include "slam_message.h"
#include "slam_utility.h"
#include <iostream>

namespace TG::application::SLAM
{

using position = message::position;

message::message(std::string sender) :
	valid_(false),
	sender_(sender),
	robot_(),
	obstacles_()
{
}

void message::set_payload(std::string payload)
{
	if (!sanity_check(payload.size()))
	{
		valid_ = false;
		std::cout << "Invalid size of payload!";
		return;
	}

	std::vector<std::byte> raw;

	for (const auto& ch : payload) {
		auto byte = std::byte(ch);
		raw.push_back(byte);
	}

	auto it = raw.begin();

	robot_.x = utility::from_byte_ptr(&(*it));
	it += 2;

	robot_.y = utility::from_byte_ptr(&(*it));
	it += 2;

	valid_ = true;

	for (it; it < raw.end();)
	{
		position obstacle;

		obstacle.x = utility::from_byte_ptr(&(*it));
		it += 2;
		obstacle.y = utility::from_byte_ptr(&(*it));
		it += 2;

		obstacles_.push_back(obstacle);
	}
}

void message::set_payload(position pos)
{
	robot_ = pos;
	valid_ = true;
}

void message::set_payload(position pos, std::vector<position> obstacles)
{
	robot_ = pos;
	obstacles_ = obstacles;
	valid_ = true;
}

bool message::is_valid() const
{
	return valid_;
}

std::vector<std::byte> message::serialize() const
{
	std::vector<std::byte> raw;

	auto serialize_int16 = [&](int16_t val) {
		auto data_ptr = reinterpret_cast<const std::byte *> (&val);
		raw.push_back(data_ptr[0]);
		raw.push_back(data_ptr[1]);
	};

	serialize_int16(robot_.x);
	serialize_int16(robot_.y);

	for (const auto& obs : obstacles_) {
		serialize_int16(obs.x);
		serialize_int16(obs.y);
	}

	return raw;
}

std::string message::sender() const
{
	return sender_;
}

position message::robot_pos() const
{
	return robot_;
}

std::vector<position> message::obstacles() const
{
	return obstacles_;
}

bool message::sanity_check(int size)
{
	// Need at least the size of one position, which is two int16_t.
	// Also need the payload to be divisble by that size, such that
	// we are sure are getting a whole number of positions.
	constexpr auto pos_size = 2 * sizeof(int16_t);
	return (size >= pos_size) && ((size % pos_size) == 0);
}

}
