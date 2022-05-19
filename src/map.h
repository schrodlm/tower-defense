#include <ncurses.h>
#include <functional>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#ifndef _MAP_H_
#define _MAP_H_

/**
 * @brief defines a map that the player is using for gameplay
 *
 */
class Map
{
public:
    explicit Map(const std::string &map);

    struct Point
    {
        Point(int _x, int _y, char _c)
        {
            c = _c;
            x = _x;
            y = _y;
        };

        char c;
        int x;
        int y;

        enum class PointType : uint8_t
        {
            MapBorder,
            Projectile,
            Wall,
            Water,
            Tower,
            Space,
            End,
            Start
        };

    private:
        friend Map;
    };

    void print_map() const;
    char type(const Point &p) const;

private:
    std::vector<std::vector<Point>> _map;
    std::pair<int,int> map_size;
};

std::string read_map(const std::string file_path);

#endif