#include "slam_utility.h"
#include <iostream>
#include <random>

namespace TG::application::SLAM::utility
{

std::optional<std::pair<float, float>> row_col_to_coord(const TG::application::SLAM::slam_grid & grid, int row, int col)
{
	if (row < 0 || col < 0)
	{
		std::cout << "Can't have negative row/col";
		return std::nullopt;
	}
	if (row > grid.rows() || col > grid.columns())
	{
		std::cout << "Row or col out of range";
		return std::nullopt;
	}

	const float row_offset = grid.size().y / 2.0f;
	const float col_offset = grid.size().x / 2.0f;

	float y = -row_offset;
	float x = -col_offset;

	y += row * grid.separation();
	x += col * grid.separation();

	return std::make_pair(x, y);
}

std::optional<std::pair<int, int>> coord_to_row_col(const TG::application::SLAM::slam_grid & grid, float x, float y)
{
	// Coordinates are signed,
	// row, col are unsigned.
	// Therefore we must offset the result such as that the
	// most negative possible signed result becomes row col {0, 0}.
	const float row_offset = grid.size().y / 2.0f;
	const float col_offset = grid.size().x / 2.0f;

	y += row_offset;
	x += col_offset;

	if (x < 0 || y < 0)
	{
		//std::cout << "Coord out of bounds (less than zero)" << std::endl;
		return std::nullopt;
	}

	int row = y / grid.separation();
	int col = x / grid.separation();

	if (row >= grid.rows() || col >= grid.columns())
	{
		//std::cout << "Coord out of bounds" << std::endl;
		return std::nullopt;
	}

	return std::make_pair(row, col);
}

std::vector<std::pair<float, float>> points_to_coords(const TG::application::SLAM::slam_grid & grid, const std::vector<std::pair<int, int>>& points)
{
	std::vector<std::pair<float, float>> coords;

	for (const auto&[row, col] : points)
	{
		auto result = row_col_to_coord(grid, row, col);
		if (result)
		{
			auto[x, y] = result.value();
			auto sep = grid.separation();
			x += sep / 2;
			y += sep / 2;

			coords.emplace_back(x, y);
		}
	}

	return coords;
}

int get_random(int min, int max)
{
	static std::random_device rd;
	static std::mt19937 gen(rd());

	std::uniform_int_distribution<> dis(min, max);

	return dis(gen);
}

sf::Color get_random_color(int alpha)
{
	return sf::Color(get_random(0, 255), get_random(0, 255), get_random(0, 255), alpha);
}

int16_t from_byte_ptr(const std::byte * pbyte)
{
	return *reinterpret_cast<const int16_t *>(pbyte);
}

int16_t from_bytes(std::byte lsb, std::byte msb)
{
	std::vector<std::byte> v{ lsb, msb };
	return from_byte_ptr(v.data());
}

}
