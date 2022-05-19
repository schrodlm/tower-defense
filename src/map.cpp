#include <fstream>
#include <iostream>
#include <ncurses.h>
#include <sstream>
#include <map> //map
#include "map.h"

// //==================================================
// //                  Point
// //==================================================
// /**
//  * @brief Construct a new Map:: Point:: Point object
//  * 
//  * @param x  x-axis position
//  * @param y  y-axis position
//  */
// Map::Point::Point(int _x, int _y, char _c) : x(_x), y(_y), c(_c) {}


//==================================================
//                  Map
//==================================================

/**
 * @brief Construct a new Map:: Map object
 *          - creates a vector<vector<Point>> _map that will be represent a map loaded from a file and its points - x,y
 *          - also loads a size of a map that will be used to create a window that will hold said map
 * 
 * @param map 
 */
Map::Map(const std::string &map)
{
    _map.emplace_back();
    auto *cur_row = &_map.back();

    int column = 0;
    int row = 0;

    for (char c : map)
    {
        if (c == '\n')
        {
            _map.emplace_back();
            cur_row = &_map.back();
            row++;
            column = 0;
            continue;
        }
        
        cur_row->emplace_back(column, row, c);
        column++;
    }
    map_size = std::make_pair(_map.back()[2].x,_map[1][1].y);  // posledni element ve vektoru vektorů bude velikost mapy 
};


/**
 * @brief copies a string that represent map from a file generates it on the screen and also return the string
 *
 * @param file_path
 * @return std::string
 */
std::string read_map(const std::string file_path) 
{
    std::stringstream buffer;
    std::ifstream file(file_path, std::ios::out);
    if (!file)
        throw std::runtime_error("File/Map doesn't exist");

    buffer << file.rdbuf();
    std::string str = buffer.str();

    return str;
}

/**
 * @brief prints out a map from a _map vector
 * 
 */
void Map::print_map() const
{
    for (int i = 0; i < (int)_map.size(); i++)
    {
        for (int j = 0; j < (int)_map[i].size(); j++)
        {
            //to-do resfresh až na konci loopu abych zvýšil rychlost
            //mvprintw
            addch(_map[i][j].c);
        }
        // window nepodporuje \n
        addch('\n');
    }
}

// /**
//  * @brief returns a character in a specified x,y position
//  * 
//  * @param p 
//  * @return char 
//  */
// char Map::type(const Point &p) const
// {
//     return _map.at(p.x).at(p.y);
// }