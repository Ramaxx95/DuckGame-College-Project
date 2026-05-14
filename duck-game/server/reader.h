#ifndef READER_H
#define READER_H

#include <vector>
#include <string>
#include "common/map.h"

class Reader {
    private:
        std::string path;
        int current_id;
        /**
         * @brief Read a map from a file
         * 
         * @param relative_path path to the file
         * @return Map 
         */
        Map readMap(const std::string& relative_path);

    public:

        /**
         * @brief Construct a new Reader object
         * 
         * @param path path to the maps directory
         */
        explicit Reader(const std::string& path);

        /**
         * @brief Read all the maps in the directory
         *
         * If the directory does not exist, it will return an empty vector
         * 
         * @return std::vector<Map> 
         */
        std::vector<Map> readMaps();
};

#endif // READER_H
