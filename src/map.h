#include <ncurses.h>
#include <functional>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#pragma once

/**
 * @brief defines a map that the player is using for gameplay
 *
 */
class Map
{
public:
    explicit Map(const std::string filePath);

    /**
     * @brief defines every point in map - gives it type
     *
     */
    struct Point
    {

        Point(int _x, int _y, char _c);


        char c;
        int x;
        int y;

       friend bool operator == (const Point& a, const Point& b);
        friend bool operator != (const Point& a, const Point& b);
        friend bool operator < (const Point& a, const Point& b); //for priority queue

        enum class PointType : char
        {
            Projectile,
            Wall,
            Path,
            Water,
            Tower,
            Enemy,
            Space,
            End,
            Start
        } pointtype;
        //todo: dve inicializace pointtypu tridy
        PointType type;

    private:
        friend class Map;
    };

    void printMap() const;
    char type(const Point &p) const;
    std::pair<int, int> getMapSize() { return map_size; }
    void createMapSubwindow(WINDOW *win);
    WINDOW *getMapWin() { return map_win; };
    void addPointToVector();
    //bfs - finds path for the enemies from start to end
    std::vector<Point> findPath(const std::vector<Point::PointType>& valid_points);

    friend class Game;
    friend class Projectile;

    static std::unordered_map<char, Map::Point::PointType> key_map;
    std::pair<std::unique_ptr<Point>, std::unique_ptr<Point>> start_end_points;

private:
    // map vector
    std::vector<std::vector<Point>> _map;
    std::vector<std::vector<Point>> _map_const;
    std::pair<int, int> map_size;
    std::pair<int, int> map_win_size;
    //path for enemies not able to walk on water
    std::vector<Point> path1;
    //path for enemies able to walk on water
    std::vector<Point> path2;
    // subwindow of game_win only for map
    WINDOW *map_win;
};

std::string read_map(const std::string file_path);
