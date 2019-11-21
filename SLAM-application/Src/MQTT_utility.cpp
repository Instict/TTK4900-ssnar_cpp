#include "MQTT_utility.h"
#include <vector>

namespace NTNU::networking::protocols::MQTT::utility
{

std::optional<topic> parse_topic(const std::string & mqtt_topic)
{
	// Topic structure is as of today:
	/*
		VERSION / SENDER / ID / COMMAND

		Examples:

		v1/robot/123/adv
		v1/server/123/cmd
	*/

	std::vector<std::size_t> slashes;
	auto slash_pos = mqtt_topic.find('/');
	while (slash_pos != std::string::npos)
	{
		slashes.push_back(slash_pos);
		slash_pos = mqtt_topic.find('/', slash_pos + 1);
	}

	if (slashes.size() != 3)
		return std::nullopt;

	std::string version = mqtt_topic.substr(0, slashes[0]);
	std::string sender = mqtt_topic.substr(slashes[0] + 1, slashes[1] - slashes[0] - 1);
	std::string id = mqtt_topic.substr(slashes[1] + 1, slashes[2] - slashes[1] - 1);
	std::string command = mqtt_topic.substr(slashes[2] + 1, mqtt_topic.size() - slashes[2]);

	topic top{ version, sender, id, command };

	return top;
}

std::ostream & operator<<(std::ostream & out, const topic & topic)
{
	return out << "Version: [" << topic.version << "], " <<
		"sender: [" << topic.sender << "], " <<
		"id: [" << topic.id << "], " <<
		"command: [" << topic.command << "]";
}

}
