#include "map.h"

#include <cstdint>
#include <iostream>
#include <vector>

static const int ROWS = 30;
static const int COLUMNS = 40;

Map::Map(const std::vector<std::vector<char>>& map, int id): id(id), map(map) {}

Map::Map(): id(0), map(std::vector<std::vector<char>> ()) {}

void Map::print() {
    for (const auto& line: map) {
        for (const auto& c: line) {
            std::cout << c;
        }
        std::cout << std::endl;
    }
}

int Map::get_rows() { return map.size(); }

int Map::get_columns() { return map[0].size(); }

std::vector<std::vector<char>> Map::get_matrix() { return this->map; }

std::vector<uint8_t> Map::serialize() {
    std::vector<uint8_t> matrix;
    for (const auto& line: map) {
        for (const auto& c: line) {
            matrix.push_back(c);
        }
        // std::copy(line.cbegin(), line.cend(), matrix.data()); no funciona esto
    }
    return matrix;
}

Map Map::deserialize(std::vector<uint8_t> data) {
    std::vector<std::vector<char>> map;
    for (int i = 0; i < ROWS; i++) {
        std::vector<char> row;
        for (int j = 0; j < COLUMNS; j++) {
            row.push_back(data[i * COLUMNS + j]);
        }
        map.push_back(row);
    }
    return Map(map, 0);
}

int Map::get_id() const {
    return id;
}
