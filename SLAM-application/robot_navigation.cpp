#include <iostream>	// testing
#include "robot_navigation.h"
#include "MQTT.h"
#include "slam_message.h"
#include "slam_utility.h"

namespace NTNU::application::SLAM {
	
	robot_navigation::robot_navigation(const struct robot_navigation_config& config) :
		mqtt_(config.broker_address),
		topic_(config.publish_topic) 
	{
		mqtt_.connect();
	}

	void robot_navigation::run() {
		std::cout << "robot navigation running!\n";
		
		
		NTNU::application::SLAM::message msg(topic_);

		if (mqtt_.publish(topic_, msg.serialize()) != networking::protocols::MQTT::MQTT::SUCCESS)
			std::cout << "navigation bad publish?\n";
	}

}