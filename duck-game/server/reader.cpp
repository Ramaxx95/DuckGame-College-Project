#include "reader.h"

#include<utility>
#include <fstream>
#include <filesystem>
#include <iostream>

const static int INITIAL_ID = 0;

namespace fs = std::filesystem;

Reader::Reader(const std::string& path) : path(path), current_id(INITIAL_ID){}

std::vector<Map> Reader::readMaps() {
    std::vector<Map> maps;
    
    for (const auto& entry : fs::directory_iterator(path)) {
        if (fs::is_regular_file(entry.path())) {
            Map map = readMap(entry.path());
            maps.push_back(map);
        }
    }

    return maps;
}

Map Reader::readMap(const std::string& relative_path) {
    std::ifstream maps(relative_path);

    std::vector<std::vector<char>> map;
    if (maps.is_open()) {
        std::vector<char> line;
        char c;
        while (maps) {
            c = maps.get();
            if (c != '\n') {
                line.push_back(c);
            } else {
                map.push_back(line);
                line.clear();
            }
        }
    }

    return Map(map, current_id++);
}
