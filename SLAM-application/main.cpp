// USER
#include "panel.h"
#include "lines.h"
#include "window.h"
#include "MQTT.h"
#include "MQTT_panel.h"
#include "MQTT_utility.h"
#include "slam_grid.h"
#include "slam_message.h"
#include "slam_utility.h"
#include "clicks.h"
#include "path.h"
#include "grid_path_solver.h"
#include "robots.h"

//	Panels
#include "robot_simulation.h"
#include "control_panel.h"
#include "target_panel.h"
#include "simulation_panel.h"

// C++ standard lib
#include <vector>		// std::vector
#include <algorithm>	// std::find
#include <utility>		// std::pair, std::any

// THIRD-PARTY
// imgui
#include "imgui.h"
#include <Thor/Math.hpp>

// boost
#include <boost/fiber/all.hpp>
#include <boost/algorithm/string/predicate.hpp>

// FIBERS
using boost::this_fiber::yield;
using boost::fibers::fiber;
using boost::this_fiber::sleep_until;

// TIME
using std::chrono::system_clock;
using std::chrono::milliseconds;
using std::chrono::seconds;

// CONFIG
//const std::string MQTT_ADDRESS{"tcp://localhost:1883"};
//	Config when the BoarderRouter is connected to the internet
const std::string MQTT_ADDRESS{ "tcp://188.166.100.22:1883" };

//const std::string MQTT_ADDRESS{ " tcp://198.41.30.241:1883" };

//const std::string MQTT_ADDRESS{ "tcp://10.42.0.1:1883" };


int main()
{
	////////////////////////////////////////////////////////////////////////////////
	// GUI: Setup elements
	////////////////////////////////////////////////////////////////////////////////

	// Drawables
	auto rows = 30;
	auto cols = 40;
	auto separation = 30;
	auto alpha = 60;

	auto grid_color = sf::Color::Green;
	grid_color.a = alpha;

	TG::application::SLAM::slam_grid grid(rows, cols, separation, grid_color);
	TG::application::SLAM::robots robots;

	// Panels
	TG::gui::panel::mqtt_panel mqtt_panel;
	TG::gui::panel::clicks clicks;
	TG::gui::panel::target_panel target_panel;
	TG::gui::panel::simulation_panel simulation_panel;

	////////////////////////////////////////////////////////////////////////////////
	// GUI: Create window, add children
	////////////////////////////////////////////////////////////////////////////////

	TG::gui::base::window win;
	// Flip y-axis by default.
	// This positive y upwards when looking at the screen.
	// This is in line with the robots---the default (y upwards negative) is not.
	// x-axis positive right is still default and correct.
	auto scale = win.getScale();
	scale.y *= -1.0;
	win.setScale(scale);

	// Drawables
	win.add_drawable(&grid);
	win.add_drawable(&robots);

	// Updatable
	win.add_updatable(&robots);


	// Panels
	TG::gui::panel::control_panel ctrl_panel;
	//	TESTING!!
	/*
	TG::gui::panel::panel robot_sim;
	bool robot_sim_enable = false;
	robot_sim.set_fun([&robot_sim_enable]() {
		ImGui::Checkbox("Simulation On", &robot_sim_enable);
	});
	*/

	TG::gui::panel::panel main_panel;
	bool yaxis_flip = true;
	main_panel.set_fun([&win, &yaxis_flip]() {
		if (ImGui::Checkbox("Flip Y-axis", &yaxis_flip)) {
			auto scale = win.getScale();
			auto y = abs(scale.y);
			scale.y = yaxis_flip ? -y : y;
			win.setScale(scale);
		}

		float rotation = thor::toRadian(win.getRotation());
		if (ImGui::SliderAngle("Angle", &rotation, 0.f, 359.0f))
		{
			win.setRotation(thor::toDegree(rotation));
		}
	});
	// TESTING
	/*
	TG::gui::panel::panel target_panel;
	bool manual_robot_drive = false;
	ipair manual_target{ 0, 0 };
	target_panel.set_fun([&]() {
		static bool bottom_left_corner_is_start = true;
		static int offset = 0;
		const ImVec2 btn_size{ 200, 0 };
		static int square_size_double{ 500 };
		static int square_size{ 250 };

		ImGui::Checkbox("Enable Manual Drive", &manual_robot_drive);
		ImGui::Checkbox("Bottom-Left Corner Is Starting Point", &bottom_left_corner_is_start);

		offset = bottom_left_corner_is_start ? square_size : 0;

		if (ImGui::InputInt("Square Size", &square_size_double, 2))
			square_size = square_size_double / 2;

		ImGui::Spacing();

		if (ImGui::Button("Top-Left Corner", btn_size))
			manual_target = { -square_size + offset, square_size + offset };
		ImGui::SameLine();
		ImGui::Text("X [%d], Y [%d]", -square_size + offset, square_size + offset);

		if (ImGui::Button("Top-Right Corner", btn_size))
			manual_target = { square_size + offset, square_size + offset };
		ImGui::SameLine();
		ImGui::Text("X [%d], Y [%d]", square_size + offset, square_size + offset);

		if (ImGui::Button("Bottom-Left Corner", btn_size))
			manual_target = { -square_size + offset, -square_size + offset };
		ImGui::SameLine();
		ImGui::Text("X [%d], Y [%d]", -square_size + offset, -square_size + offset);

		if (ImGui::Button("Bottom-Right Corner", btn_size))
			manual_target = { square_size + offset, -square_size + offset };
		ImGui::SameLine();
		ImGui::Text("X [%d], Y [%d]", square_size + offset, -square_size + offset);

		ImGui::Spacing();

		static int manual_input[2] = { 0, 0 };
		if (ImGui::InputInt2("Manual Input", manual_input))
			manual_target = { manual_input[0], manual_input[1] };

		ImGui::Spacing();

		ImGui::Text("Current target is: X [%d], Y [%d]", manual_target.first, manual_target.second);
	});
	*/
///////////////////////////////////////// 
	//	TEST	//
///////////////////////////////////////// 
	TG::gui::panel::panel example_panel;
	example_panel.set_fun([&] {					
		static int slider_value = 0;
		ImGui::SliderInt("Some Value", &slider_value, -100, 100);
		if (ImGui::Button("Click me!"))
			std::cout << "I was clicked! Slider has value: " << slider_value << "\n";
		});
	ctrl_panel.embed_panel(&example_panel, "Example");

	ctrl_panel.embed_panel(&main_panel, "Main");
	ctrl_panel.embed_panel(&mqtt_panel, "MQTT");
	ctrl_panel.embed_panel(&clicks, "Clicks");
	ctrl_panel.embed_panel(&robots, "Robots");
	ctrl_panel.embed_panel(&target_panel, "Manual Drive");
	ctrl_panel.embed_panel(&grid, "Grid");
	ctrl_panel.embed_panel(&simulation_panel, "Robot Simulation");

	win.add_panel(&ctrl_panel);

	////////////////////////////////////////////////////////////////////////////////
	// Setup channels for communication between callbacks and fibers
	////////////////////////////////////////////////////////////////////////////////
	using string_channel = boost::fibers::buffered_channel<std::string>;
	using slam_channel = boost::fibers::buffered_channel<TG::application::SLAM::message>;

	slam_channel slam_ch{ 32 };
	slam_channel mqtt_to_publish_ch{ 32 };
	string_channel mqtt_to_subscribe_ch{ 32 };

	////////////////////////////////////////////////////////////////////////////////
	// Callbacks
	////////////////////////////////////////////////////////////////////////////////

	ctrl_panel.enable_callback(TG::gui::panel::control_panel_event::QUIT, [&win](auto a) {
		win.close();
	});

	// Create function objects for lambda functions used in several types of callbacks
	auto reapply_obstruction_points = [&](std::any) {
		auto obstacles = robots.get_all_obstacles();

		for (const auto&[x, y] : obstacles)
		{
			auto convert_to_grid = TG::application::SLAM::utility::coord_to_row_col(grid, x, y);
			if (convert_to_grid)
			{
				auto[row, col] = convert_to_grid.value();
				grid.obstruct(row, col);
			}
		}
	};

	// Target given in terms of grid row, column
	auto update_path_for_robot = [&](const std::string& robot, std::pair<int, int> target) {
		auto get_pos = robots.position(robot);
		if (!get_pos)
			return;

		auto source = get_pos.value();
		// Robot positions are given in global coordinates...
		auto result = TG::application::SLAM::utility::coord_to_row_col(grid, source.first, source.second);
		if (!result)
			return;

		auto[robo_row, robo_col] = result.value();
		source = { robo_row, robo_col };

		// ... paths are solved in terms of the underlying grid (which uses rows and columns)...
		auto path = TG::graph::pathfinding::solve(grid.get_filtered_grid(), source, target);
		if (!path)
			return;

		// ... reduce the given path to straight lines ...
		auto rpath = TG::graph::pathfinding::reduce(path.value());
		// ... and convert it back to global coordinates ...
		if (auto coords = TG::application::SLAM::utility::points_to_coords(grid, rpath); robots.navigate())
		{
			// ... apply this new path to the given robot.
			robots.set_path(robot, coords);
		}
		else
		{
			robots.set_path(robot, { coords.front(), coords.back() });
		}
	};

	// Create "inline" lambdas for functions used in only one callback
	win.enable_callback(sf::Event::MouseButtonPressed, [&](sf::Event e) {
		sf::Vector2f pos{ static_cast<float>(e.mouseButton.x), static_cast<float>(e.mouseButton.y) };
		pos = win.getInverseTransform().transformPoint(pos);

		std::cout << "Mouse button press: {" << pos.x << ", " << pos.y << "}" << std::endl;

		auto grid_idx = TG::application::SLAM::utility::coord_to_row_col(grid, pos.x, pos.y);
		if (grid_idx)
		{
			using TG::gui::panel::clicks_choices;
			const auto[row, col] = grid_idx.value();

			if (e.mouseButton.button == sf::Mouse::Left && clicks.left_is(clicks_choices::obstruct)
				|| e.mouseButton.button == sf::Mouse::Right && clicks.right_is(clicks_choices::obstruct))
				grid.obstruct(row, col);

			if (e.mouseButton.button == sf::Mouse::Left && clicks.left_is(clicks_choices::set_target)
				|| e.mouseButton.button == sf::Mouse::Right && clicks.right_is(clicks_choices::set_target))
			{
				auto robot_ids = robots.get_all_robot_ids();
				for (const auto& id : robot_ids)
				{
					std::pair<int, int> target = { row, col };
					update_path_for_robot(id, target);
				}
			}
		}
	});

	win.enable_callback(sf::Event::MouseMoved, [&grid, &win](sf::Event e) {
		sf::Vector2f pos{ static_cast<float>(e.mouseMove.x), static_cast<float>(e.mouseMove.y) };
		static sf::Vector2f previous{ 0, 0 };
		sf::Vector2f delta = pos - previous;
		previous = pos;

		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle))
		{
			win.move(delta);
		}
	});

	win.enable_callback(sf::Event::MouseWheelScrolled, [&win](sf::Event e) {
		constexpr auto scroll_scaling = 0.1;
		auto scroll_amount = e.mouseWheelScroll.delta * scroll_scaling;

		auto scale = win.getScale();
		auto y_sign = std::copysignf(1.0, scale.y);
		auto x_sign = std::copysignf(1.0, scale.x);

		scale.x = abs(scale.x);
		scale.y = abs(scale.y);

		scale.x += scroll_amount;
		scale.y += scroll_amount;

		std::cout << "scale: " << scale.x << "," << scale.y << "\n";

		if (scale.x > 0 && scale.y > 0)
		{
			scale.y *= y_sign;
			scale.x *= x_sign;
			win.setScale(scale);
		}
	});

	robots.enable_callback(TG::application::SLAM::robots_events::ROBOT_CLEARED_POINTS, [&](std::any context) {
		try
		{
			auto obstacles = std::any_cast<std::vector<std::pair<int, int>>>(context);
			for (const auto&[x, y] : obstacles)
			{
				auto result = TG::application::SLAM::utility::coord_to_row_col(grid, x, y);
				if (result)
				{
					auto[row, col] = result.value();
					grid.unobstruct(row, col);
				}
			}
		}
		catch (const std::bad_any_cast& e) { std::cout << e.what(); }
	});

	robots.enable_callback(TG::application::SLAM::robots_events::ROBOT_MOVED, [&](std::any context) {
		try
		{
			auto[id, x, y] = std::any_cast<std::tuple<std::string, int16_t, int16_t>>(context);
			//std::cout << "Robot [" << id << "] moved to {" << x << ", " << y << "}\n";
		}
		catch (const std::bad_any_cast& e) { std::cout << e.what(); }
	});

	robots.enable_callback(TG::application::SLAM::robots_events::ROBOT_FOUND_OBSTACLE, [&](std::any context) {
		try
		{
			auto[id, x, y] = std::any_cast<std::tuple<std::string, int16_t, int16_t>>(context);
			//std::cout << "Robot [" << id << "] obstacle at {" << x << ", " << y << "}\n";

			auto convert_to_grid = TG::application::SLAM::utility::coord_to_row_col(grid, x, y);
			if (convert_to_grid)
			{
				auto[row, col] = convert_to_grid.value();
				grid.obstruct(row, col);
			}

			auto has_target = robots.get_target(id);
			if (has_target)
			{
				auto[tx, ty] = has_target.value();
				auto target = TG::application::SLAM::utility::coord_to_row_col(grid, tx, ty);
				if (target)
					update_path_for_robot(id, target.value());
			}
		}
		catch (const std::bad_any_cast& e) { std::cout << e.what(); }
	});

	mqtt_panel.enable_callback(TG::gui::panel::mqtt_panel_events::PUBLISH_REQUEST, [&](std::any context) {
		try
		{
			std::cout << "Publish req\n";

			auto msg = std::any_cast<TG::application::SLAM::message>(context);
			if (mqtt_to_publish_ch.try_push(msg) !=
				boost::fibers::channel_op_status::success)
				std::cerr << "Could not put onto pub ch!\n";
		}
		catch (const std::bad_any_cast& e) { std::cout << e.what(); }
	});

	mqtt_panel.enable_callback(TG::gui::panel::mqtt_panel_events::SUBSCRIBE_REQUEST, [&](std::any context) {
		try
		{
			auto topic = std::any_cast<std::string>(context);
			std::cout << "Sub req\n";

			if (mqtt_to_subscribe_ch.try_push(topic) !=
				boost::fibers::channel_op_status::success)
				std::cerr << "Could not put onto sub ch!\n";
		}
		catch (const std::bad_any_cast& e) { std::cout << e.what(); }
	});

	grid.enable_callback(TG::application::SLAM::slam_grid_events::GRID_GEOMETRY_CHANGED, [&](std::any a) {
		reapply_obstruction_points(a);
	});

	grid.enable_callback(TG::application::SLAM::slam_grid_events::NODE_OBSTRUCTED, [&](std::any context) {

	});

	////////////////////////////////////////////////////////////////////////////////
	// Fibers: Setup asynchronous cooperative user-space threads
	////////////////////////////////////////////////////////////////////////////////

	fiber gui_f([&] {
		for (;;)
		{
			if (!win.run())
				return;

			yield();
		}
	});

	fiber mqtt_f([&] {
		TG::networking::protocols::MQTT::MQTT mqttClient(MQTT_ADDRESS);
		mqtt_panel.set_client_id(mqttClient.getClientId());

		using TG::networking::protocols::MQTT::MQTT_events;
		// Shorthand for the format mqtt_panel uses for messages.
		using mqtt_msg = std::pair<std::string, std::string>;

		// Can yield/wait on a response.
		// The first bool indicates if a response has been made (yield while this is false).
		// The second bool is the result of the response (i.e. success/fail).
		std::pair<bool, bool> response{ false, false };
		mqttClient.enable_callback(MQTT_events::connect_response, [&](std::any any) {
			try
			{
				auto result = std::any_cast<bool>(any);
				std::cout << "Connect response result: " << std::boolalpha << result << '\n';

				response = std::make_pair(true, result);
			}
			catch (const std::bad_any_cast& e) { std::cout << e.what(); }
		});

		mqttClient.enable_callback(MQTT_events::message_arrived, [&](std::any any) {
			try
			{
				auto[topic, msg] = std::any_cast<mqtt_msg>(any);
				mqtt_panel.add_msg_in(topic, msg);
				TG::application::SLAM::message slam_msg(topic);
				slam_msg.set_payload(msg);
				

				//std::cout << "Message arrived: " << topic << " | " << msg << '\n';	
				// FOR TESTING PURPOSE //
				/*
				std::cout << ".enable_callback: ";
				for (int i = 0; i < msg.length(); i++) {
					std::cout << msg[i];
				}
				std::cout << std::endl;
				
				std::vector<std::byte> raw;

				for (const auto& ch : msg) {
					auto byte = std::byte(ch);
					raw.push_back(byte);
				}
				auto iterator = raw.begin();

				std::cout << "pX: " << TG::application::SLAM::utility::from_byte_ptr(&(*iterator));
				iterator += 2;
				std::cout << " pY: " <<TG::application::SLAM::utility::from_byte_ptr(&(*iterator));
				iterator += 2;

				for (iterator; iterator < raw.end();)
				{
					std::cout << "oX: " << TG::application::SLAM::utility::from_byte_ptr(&(*iterator));
					iterator += 2;
					std::cout << " oY: " << TG::application::SLAM::utility::from_byte_ptr(&(*iterator));
					iterator += 2;
					std::cout << std::endl;
				}
				*/
				// FOR TESTING PURPOSE //
				
				
				if (slam_ch.try_push(slam_msg) !=
					boost::fibers::channel_op_status::success)
					std::cerr << "Could not put onto slam ch!\n";
			}
			catch (const std::bad_any_cast& e) { std::cout << e.what(); }
		});

		mqttClient.enable_callback(MQTT_events::subscribe_response, [&](std::any any) {
			try
			{
				auto result = std::any_cast<std::string>(any);
				std::cout << "Subscribe response: " << result << '\n';
				if (result != "") {
					mqtt_panel.add_sub(result);
				}
			}
			catch (const std::bad_any_cast& e) { std::cout << e.what(); }
		});

		mqttClient.enable_callback(MQTT_events::publish_response, [&](std::any any) {
			try
			{
				auto[topic, msg] = std::any_cast<mqtt_msg>(any);
				std::cout << "Publish response: " << topic << " | " << msg << '\n';
				mqtt_panel.add_msg_out(topic, msg);
			}
			catch (const std::bad_any_cast& e) { std::cout << e.what(); }
		});

		if (mqttClient.connect() != TG::networking::protocols::MQTT::MQTT::SUCCESS)
		{
			std::cout << "Connect returned fail!" << std::endl;
		}

		while (!response.first)
			yield();

		if (!response.second)
			std::cerr << "Could not connect!!\n";

		for (;;)
		{
			// Mqtt: Subscribe to all subtopic of NTNU
			if (static bool once = true; once)
			{
				once = false;

				if (mqttClient.subscribe("v1/robot/#") != TG::networking::protocols::MQTT::MQTT::SUCCESS)
				{
					std::cerr << "Subscribe error!" << std::endl;
				}
			}

			using TG::application::SLAM::message;
			using boost::fibers::channel_op_status;

			// Mqtt: Subscribe to channels if requested to do so
			if (std::string topic; mqtt_to_subscribe_ch.try_pop(topic) ==
				channel_op_status::success) {
				std::cout << "Got a request to subscribe to: " << topic << '\n';
				if (mqttClient.subscribe(topic) != TG::networking::protocols::MQTT::MQTT::SUCCESS)
				{
					std::cerr << "Subscribe error from request!" << std::endl;
				}
			}

			// Mqtt: Publish messages if requested to do so
			if (message msg; mqtt_to_publish_ch.try_pop(msg) ==
				channel_op_status::success) {
				std::cout << "Got a request to publish to: " << msg.sender() << '\n';

				if (mqttClient.publish(msg.sender(), msg.serialize()) != TG::networking::protocols::MQTT::MQTT::SUCCESS)
					std::cout << "Bad publish from request\n";
			}

			yield();
		}
	});

	fiber robots_inbox_f([&] {
		for (;;)
		{
			TG::application::SLAM::message msg;
			auto res = slam_ch.pop(msg);

			if (res != boost::fibers::channel_op_status::success) {
				std::cerr << "SLAM ch pop res not success!\n";
				continue;
			}

			if (!msg.is_valid()) {
				std::cerr << "Got invalid SLAM message!";
				continue;
			}

			robots.feed_message(msg);

			yield();
		}
	});
	// TESTING
	
	fiber robots_outbox_f([&] {
		for (;;)
		{
			auto all_robots = robots.get_all_robot_ids();
			for (const auto& robot : all_robots)
			{
				std::optional<ipair> next_point = std::nullopt;
				TG::gui::panel::target_panel manual_target;
				TG::gui::panel::target_panel manual_robot_drive;
				
				if (manual_robot_drive.get_manual_robot_drive())
				{
					next_point = manual_target.get_manual_target();
				}
				else
					next_point = robots.get_next_point(robot);

				if (!next_point)
					continue;

				auto result = TG::networking::protocols::MQTT::utility::parse_topic(robot);
				if (result)
				{
					auto mqtt_topic = result.value();

					TG::application::SLAM::message msg(mqtt_topic.version + "/server/" + mqtt_topic.id + "/cmd");

					auto[nx, ny] = next_point.value();
					TG::application::SLAM::message::position pos{ nx, ny };
					msg.set_payload(pos);
				//	std::cout << "Next point, X: " << pos.x << " Y: " << pos.y << std::endl;		//testing purpose
					auto result = mqtt_to_publish_ch.push(msg);
					if (result != boost::fibers::channel_op_status::success) {
						std::cerr << "Robot push msg onto publish queue did not succeed!\n";
					}
				}
				else
				{
					std::cout << "Bad topic parse?!" << '\n';
				}


				auto grace_time_over = system_clock::now() + milliseconds(100);
				sleep_until(grace_time_over);
			}

			auto next_time = system_clock::now() + milliseconds(1000);
			sleep_until(next_time);
		}
	});

	/*
	Robot Simulation
	*/
	fiber robot_simulation_f([&] {
		TG::application::SLAM::robot_simulation_config config = { MQTT_ADDRESS, "v1/robot/simulated/adv", 3, 25 };
		TG::application::SLAM::robot_simulation robot_sim{ config };
		TG::gui::panel::simulation_panel robot_sim_enable;		//	TESTING
		// Grace time (for e.g. MQTT connection)
		sleep_until(system_clock::now() + seconds(3));
		std::cout << "robot_sim_enable: " << robot_sim_enable.get_robot_sim_enable() << std::endl;	//	TESTING
		for (;;)
		{
			if (robot_sim_enable.get_robot_sim_enable())	//	TESTING
				robot_sim.run();

			auto next_time = system_clock::now() + milliseconds(10);
			sleep_until(next_time);
		}
	});

	gui_f.join();
	mqtt_f.join();
	robots_inbox_f.join();
	robots_outbox_f.join();	//	TESTING
	robot_simulation_f.join();

	return 0;
}