#include "robot_navigation.h"

namespace NTNU::application::SLAM {

	robot_navigation::robot_navigation(const struct robot_navigation_config& config) :
		mqtt_(config.broker_address),
		topic_(config.publish_topic),
		crawl_speed_(config.crawl_speed),
		obstacle_spread_(config.obstacle_spread)
	{
		mqtt_.connect();
	}



	void robot_navigation::run() {

	}
}