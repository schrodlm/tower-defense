#include <ncurses.h>
#include <algorithm>
#include "projectile.h"


/**
 * @brief move the projectile on the map based on its direction
 * 
 */
void Projectile::moving()
{
    switch (direction)
    {
    case Direction::Up:
        point->y--;
        break;
    case Direction::Down:
        point->y++;
        break;
    case Direction::Left:
        point->x--;
        break;
    case Direction::Right:
        point->x++;
        break;
    case Direction::NoDirection:
        break;

    }
}

/**
 * @brief checks if projectile collided with anything on the map
 * 
 * @param map           --> map
 * @param valid_types   --> points that projectile cannot collide with
 * @return true         --> projectile collided
 * @return false        --> projectile didn't collide
 */
bool Projectile::collisionCheck(const std::vector<std::vector<Map::Point>> &map, const std::vector<Map::Point::PointType>& valid_types)const
{
    //if element from types vector is found in map at point -- point is not VALID for projectile
    if (std::find(valid_types.begin(), valid_types.end(), prev_point->type) == valid_types.end())
    {
        return true;
    }
    else return false;
}

/**
 * @brief sets up a point in which new projectiles of specific tower are generated
 * 
 * @param p  --> current game map
 */
void Projectile::setStartingPoint(const Map::Point &p)
{
    starting_point = std::make_unique<Map::Point>(p);
    switch (direction)
    {
    case Direction::Up:
        starting_point->y--;
        break;
    case Direction::Down:
        starting_point->y++;
        break;
    case Direction::Left:
        starting_point->x--;
        break;
    case Direction::Right:
        starting_point->x++;
        break;
    case Direction::NoDirection:
        break;
    }
    starting_point->c = display;
}
