#include <fstream>
#include <iostream>
#include <ncurses.h>
#include <sstream>
#include <map>           //map
#include <unordered_map> //unordered_map
#include <deque>
#include "map.h"
#include "terminal.h"

/**
 * @brief copies a string that represent map from a file generates it on the screen and also return the string
 *
 * @param file_path
 * @return std::string
 */
std::string readFromFile(const std::string file_path)
{
    std::stringstream buffer;
    std::ifstream file(file_path, std::ios::out);
    if (!file)
        throw std::runtime_error("File/Map doesn't exist");

    buffer << file.rdbuf();
    std::string str = buffer.str();

    return str;
}
// point operator
Map::Point::Point(int _x, int _y, char _c)
{
    c = _c;
    x = _x;
    y = _y;
    type = key_map[c];
}

bool operator==(const Map::Point &a, const Map::Point &b)
{
    return a.x == b.x && a.y == b.y;
}
bool operator!=(const Map::Point &a, const Map::Point &b)
{
    return !(a == b);
}
bool operator<(const Map::Point &a, const Map::Point &b)
{
    return std::tie(a.x, a.y) < std::tie(b.x, b.y);
}

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
Map::Map(const std::string filePath)
{

    const std::string map = readFromFile(filePath);
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
        // checks if the point is end or start
        if (c == 'E')
        {
            start_end_points.second = std::make_unique<Point>(column, row, c);
        }
        if (c == 'S')
        {
            start_end_points.first = std::make_unique<Point>(column, row, c);
        }
        column++;
    }
    // deleting last row (its empty)
    _map.erase(_map.end());

    //setting up const map so I can later save it into a file if player wants to save the game
    _map_const = _map;

    map_size = std::make_pair(_map.back().back().x + 1, _map.back().back().y + 1); // posledni element ve vektoru vektorů bude velikost mapy
};

/**
 * @brief prints out a map from a _map vector
 *
 */
void Map::printMap() const
{
    int row_number = 1;
    wmove(map_win, 1, row_number);
    for (int i = 0; i < (int)_map.size(); i++)
    {
        wmove(map_win, i + 1, 1);
        for (int j = 0; j < (int)_map[i].size(); j++)
        {
            // to-do resfresh až na konci loopu abych zvýšil rychlost
            // mvprintw
            waddch(map_win, _map[i][j].c);
        }
        // window nepodporuje \n
        row_number++;
    }
    refresh();
    wrefresh(map_win);
}

/**
 * @brief creates a subwindow for map
 *
 * @param win
 */
void Map::createMapSubwindow(WINDOW *win)
{
    map_win_size = std::make_pair(map_size.first + 2, map_size.second + 2);

    map_win = createNewSubwindow(win, map_win_size.second, map_win_size.first, 1, 1);
    wrefresh(map_win);
}

/**
 * @brief bfs algorthith for enemies to create a path for them
 *          -- big inspiration taken from RNDr. Radek Hušek with his approval
 *
 */
std::vector<Map::Point> Map::findPath(const std::vector<Point::PointType> &valid_points)
{
    using Point = Map::Point;

    // sets up s as start point
    Point s = *start_end_points.first;

    // sets up two data structure needed -
    // pred  --> saving all the possbile paths, by making a map of points
    // q     --> for trying all 4 possbile directions from each point starting from point S - start ending when reaching point E - end
    std::map<Point, Point> pred;
    std::deque<Point> q;
    q.push_back(s);

    while (!q.empty())
    {
        Point p = q.front();
        q.pop_front();

        for (auto [yd, xd] : {std::pair<int, int>{-1, 0}, {0, -1}, {1, 0}, {0, 1}})
        {
            int y = p.y + yd;
            int x = p.x + xd;
            if (y < 0 || y >= (int)_map.size())
                continue;
            auto &row = _map[y];
            if (x < 0 || x >= (int)row.size())
                continue;

            Point u = _map[y][x];

            if (u == s || !(std::count(valid_points.begin(), valid_points.end(), u.type)) || pred.count(u) != 0)
                continue;
            pred.emplace(u, p);
            q.push_back(u);
        }
    }
    Point t = *start_end_points.second;
    std::vector<Point> path;
    // no path has been found
    if (pred.count(t) == 0)
    {
        throw std::runtime_error("This map has no possible paths from start to end");
    }
    path.push_back(t);

    // if path has been found push it into path vector and reverse it - because its from end to start
    while (pred.count(t) != 0)
        path.push_back(t = pred.at(t));
    std::reverse(path.begin(), path.end());

    return path;
}

std::unordered_map<char, Map::Point::PointType> Map::key_map = {
    {'#', Map::Point::PointType::Wall},
    {'~', Map::Point::PointType::Water},
    {' ', Map::Point::PointType::Space},
    {'S', Map::Point::PointType::Start},
    {'E', Map::Point::PointType::End},
    {'G', Map::Point::PointType::Tower},
    {'B', Map::Point::PointType::Tower},
    {'F', Map::Point::PointType::Tower},
    {'I', Map::Point::PointType::Tower},
    {'@', Map::Point::PointType::Enemy},
    {'&', Map::Point::PointType::Enemy},
    {'*', Map::Point::PointType::Projectile},
    {'o', Map::Point::PointType::Projectile},
    {'.', Map::Point::PointType::Projectile},
    {'*', Map::Point::PointType::Projectile},

};