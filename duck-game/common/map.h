#ifndef MAP_H
#define MAP_H

#include <algorithm>
#include <cstdint>
#include <vector>

class Map {
private:
    int id;
    std::vector<std::vector<char>> map;

public:
    /**
     * @brief Construct a new Map object
     *
     * The map has the following format:
     *
     *  ######
     *  #....#
     *  #..1.#
     *  #....#
     *  #.W..#
     *  ######
     *
     * Where:
     * - # is a wall
     * - . is a floor
     * - 1 is a player
     * - W is a weapon
     *
     * @param map vector of vectors of chars
     */
    explicit Map(const std::vector<std::vector<char>>& map, int id);

    Map();

    /**
     * @brief Print the map
     *
     */
    void print();

    /**
     * @brief Get the rows of the map
     *
     * @return int
     */
    int get_rows();


    /**
     * @brief Get the columns of the map
     *
     * @return int
     */
    int get_columns();

    /**
     * @brief Get the matrix of the map
     *
     * @return std::vector<std::vector<char>>
     */
    std::vector<std::vector<char>> get_matrix();

    /**
     * @brief Serialize the map to send it through the network
     *
     * @return std::vector<uint8_t> array of bytes
     */
    std::vector<uint8_t> serialize();

    /**
     * @brief Deserialize the map to receive it through the network
     * 
     * Returns a new Map
     * 
     * @param data array of bytes
     * @return Map 
     */
    static Map deserialize(std::vector<uint8_t> data);

    int get_id() const;
};

#endif  // MAP_H
