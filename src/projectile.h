
#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <ncurses.h>
#include "map.h"
  

        enum class Direction : int
        {
            Up,
            Down,
            Left,
            Right,
            NoDirection
        };

/**
 * @brief defines projectile each child of Tower class in the game generates
 * 
 */
class Projectile 
{

    public:
        Projectile(Direction _direction, char _display, Map::Point &_p)
        {
            //static cast int to Direction
            direction = static_cast<Direction>(_direction);
            display = _display;

            setStartingPoint(_p);
        };
        ~Projectile() = default;

        bool collisionCheck(const std::vector<std::vector<Map::Point>> &map,const std::vector<Map::Point::PointType>& valid_types)const;
        void moving();
        void placeProjectile(int x, int y, Map::Point &prev);
        void setStartingPoint(const Map::Point &p);


        //starting point of projectile - should be set before moving
        std::shared_ptr<Map::Point> starting_point;
        //current point of projectile
        std::shared_ptr<Map::Point> point = nullptr;
        std::unique_ptr<Map::Point> prev_point = nullptr;
        char display;
        Direction direction;
};

#endif