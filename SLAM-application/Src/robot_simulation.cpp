#include <iostream>
#include "robot_simulation.h"
#include "MQTT.h"
#include "slam_message.h"
#include "slam_utility.h"

namespace TG::application::SLAM
{

robot_simulation::robot_simulation(const struct robot_simulation_config& config) :
	mqtt_(config.broker_address),
	topic_(config.publish_topic),
	crawl_speed_(config.crawl_speed),
	obstacle_spread_(config.obstacle_spread)
{
	mqtt_.connect();
}

void robot_simulation::run()
{
	using TG::application::SLAM::utility::get_random;

	x_ += get_random(-crawl_speed_, crawl_speed_);
	y_ += get_random(-crawl_speed_, crawl_speed_);

	obs_x_ += get_random(-obstacle_spread_, obstacle_spread_);
	obs_y_ += get_random(-obstacle_spread_, obstacle_spread_);

	TG::application::SLAM::message msg(topic_);
	TG::application::SLAM::message::position pos{ x_, y_ };
	TG::application::SLAM::message::position obs{ obs_x_, obs_y_ };

	//std::cout << "Simulation: {" << pos.x << ", " << pos.y << "}, obs: {" << obs.x << ", " << obs.y << "}\n";

	msg.set_payload(pos, { obs });

	if (mqtt_.publish(topic_, msg.serialize()) != networking::protocols::MQTT::MQTT::SUCCESS)
		std::cout << "Simulation bad publish?\n";
}

}
