#include "MapGenerator.hpp"
#include "Block.hpp"
#include <random>

#include "Map.hpp"
class MapSharedPointerEnabler : public Map { public: MapSharedPointerEnabler(size_t width, size_t height, std::shared_ptr<Block> default_block) : Map(width, height, default_block) {} };

std::shared_ptr<Map> MapGenerator::generate_single_block_filled_map(size_t width, size_t height, std::shared_ptr<Block> block) {
	auto map = std::make_shared<MapSharedPointerEnabler>(width, height, block);
	map->addNewBlock(block);
	for (size_t i = 0; i < map->width(); i++)
		for (size_t j = 0; j < map->height(); j++)
			map->set(block, i, j);
	return map;
}
std::shared_ptr<Map> MapGenerator::generate_map_with_borders_only(size_t width, size_t height, std::shared_ptr<Block> border, std::shared_ptr<Block> empty) {
	auto map = std::make_shared<MapSharedPointerEnabler>(width, height, border);
	map->addNewBlock(border);
	map->addNewBlock(empty);
	for (size_t i = 0; i < map->width(); i++)
		for (size_t j = 0; j < map->height(); j++)
			if (map->isBorder(i, j))
				map->set(border, i, j);
			else
				map->set(empty, i, j);
	return map;
}
std::shared_ptr<Map> MapGenerator::generate_random_map(size_t width, size_t height, std::vector<std::shared_ptr<Block>> types) {
	if (types.size() == 0) return nullptr;
	std::random_device rd;
	std::mt19937 g(rd());
	std::uniform_int_distribution<size_t> d(0, types.size() - 1);

	auto map = std::make_shared<MapSharedPointerEnabler>(width, height, types.front());
	for (size_t i = 0; i < types.size(); i++)
		map->addNewBlock(types[i]);
	for (size_t i = 0; i < map->width(); i++)
		for (size_t j = 0; j < map->height(); j++)
			map->set(types[d(g)], i, j);
	return map;
}
std::shared_ptr<Map> MapGenerator::generate_continious_map(size_t width, size_t height, std::shared_ptr<Block> ceiling, std::shared_ptr<Block> floor, std::shared_ptr<Block> free, size_t max_step, size_t ceiling_start, size_t floor_start, size_t min_height) {
	std::random_device rd;
	std::mt19937 g(rd());
	std::uniform_int_distribution<int> d(-int(max_step), int(max_step));

	auto map = std::make_shared<MapSharedPointerEnabler>(width, height, ceiling);
	map->addNewBlock(free);
	map->addNewBlock(ceiling);
	map->addNewBlock(floor);

	size_t current_ceiling = ceiling_start;
	size_t current_floor = floor_start;
	for (size_t i = 0; i < map->width(); i++) {
		size_t j = 0;
		for (; j < current_ceiling; j++)
			map->set(ceiling, i, j);
		for (; j < current_floor; j++)
			map->set(free, i, j);
		for (; j < map->height(); j++)
			map->set(floor, i, j);

		current_ceiling += d(g);
		current_floor += d(g);
		int diff = int(current_floor) - int(current_ceiling);
		if (diff < 5) {
			size_t center = (current_floor + current_ceiling) / 2;
			current_floor = center + 3;
			current_ceiling = center - 2;
		}
		if (current_ceiling < min_height)
			current_ceiling = min_height;
		if (current_floor > map->height() - min_height)
			current_floor = map->height() - min_height;
	}

	addBorders(map, floor);
	return map;
}
std::shared_ptr<Map> MapGenerator::generate_map_with_horizontal_rows_only(size_t width, size_t height, std::shared_ptr<Block> odd, std::shared_ptr<Block> even) {
	auto map = std::make_shared<MapSharedPointerEnabler>(width, height, odd);
	map->addNewBlock(odd);
	map->addNewBlock(even);
	for (size_t i = 0; i < map->width(); i++)
		for (size_t j = 0; j < map->height(); j += 2) {
			map->set(odd, i, j);
			map->set(even, i, j + 1);
		}
	return map;
}
std::shared_ptr<Map> MapGenerator::generate_map_with_vertical_rows_only(size_t width, size_t height, std::shared_ptr<Block> odd, std::shared_ptr<Block> even) {
	auto map = std::make_shared<MapSharedPointerEnabler>(width, height, odd);
	map->addNewBlock(odd);
	map->addNewBlock(even);
	for (size_t j = 0; j < map->height(); j++)
		for (size_t i = 0; i < map->width(); i += 2) {
			map->set(odd, i, j);
			map->set(even, i + 1, j);
		}
	return map;
}

std::shared_ptr<Map> MapGenerator::generate_empty_map(size_t width, size_t height) {
	return generate_single_block_filled_map(width, height, std::make_shared<EmptyBlock>());
}
std::shared_ptr<Map> MapGenerator::generate_filled_map(size_t width, size_t height) {
	return generate_single_block_filled_map(width, height, std::make_shared<WallBlock>());
}
std::shared_ptr<Map> MapGenerator::generate_map_with_borders_only(size_t width, size_t height) {
	return generate_map_with_borders_only(width, height, std::make_shared<WallBlock>(), std::make_shared<EmptyBlock>());
}
std::shared_ptr<Map> MapGenerator::generate_random_map(size_t width, size_t height) {
	return generate_random_map(width, height, {std::make_shared<WallBlock>(), std::make_shared<EmptyBlock>()});
}
std::shared_ptr<Map> MapGenerator::generate_continious_map(size_t width, size_t height) {
	auto wall = std::make_shared<WallBlock>();
	return generate_continious_map(width, height, wall, wall, std::make_shared<EmptyBlock>(), 3, height / 5, height - height / 5, 5);
}
std::shared_ptr<Map> MapGenerator::generate_map_with_horizontal_rows_only(size_t width, size_t height) {
	return generate_map_with_horizontal_rows_only(width, height, std::make_shared<WallBlock>(), std::make_shared<EmptyBlock>());
}
std::shared_ptr<Map> MapGenerator::generate_map_with_vertical_rows_only(size_t width, size_t height) {
	return generate_map_with_vertical_rows_only(width, height, std::make_shared<WallBlock>(), std::make_shared<EmptyBlock>());
}

void MapGenerator::addBorders(std::shared_ptr<Map> map, std::shared_ptr<Block> border) {
	for (size_t i = 0; i < map->width(); i++) {
		map->set(border, i, 0);
		map->set(border, i, map->height() - 1);
	}
	for (size_t i = 0; i < map->height(); i++) {
		map->set(border, 0, i);
		map->set(border, map->width() - 1, i);
	}
}