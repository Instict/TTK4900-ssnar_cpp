#pragma once

#include <SFML/Graphics.hpp>
#include <optional>
#include <stddef.h>
#include <vector>
#include "slam_grid.h"

namespace NTNU::application::SLAM::utility
{

std::optional<std::pair<float, float>> row_col_to_coord(const NTNU::application::SLAM::slam_grid& grid, int row, int col);
std::optional<std::pair<int, int>> coord_to_row_col(const NTNU::application::SLAM::slam_grid& grid, float x, float y);

std::vector<std::pair<float, float>> points_to_coords(const NTNU::application::SLAM::slam_grid& grid, const std::vector<std::pair<int, int>>& points);

int get_random(int min, int max);

sf::Color get_random_color(int alpha = 255);

int16_t from_byte_ptr(const std::byte* pbyte);
int16_t from_bytes(std::byte lsb, std::byte msb);

}
