#pragma once

#include <iostream>
#include <fstream>
#include <iostream>
#include <ncurses.h>
#include <sstream>
#include <string>
#include <algorithm>
#include <map>
#include <regex>
#include "map.h"
#include "projectile.h"

// path to entities config file
const std::string file_path = "assets/entities/entities.txt";

/**
 * @brief represents tower or Enemy in the game - "blueprint" for all entities in the game
 *
 */
class Entity
{
public:
    void operator=(const Entity &) = delete;
    virtual ~Entity(){};

    char getDisplay() { return display; }
    int getDamage() { return damage; }

protected:
    Entity(){};
    // how will entity print onto the map
    char display;
    // tower projectile damage || monster nexus damage (nexus is the end of the map - limited health)
    int damage;
};

//==================================================
//                  Towers
//==================================================

/**
 * @brief "blueprint" for all the towers in the game - child of Entity class
 *
 */
class Tower : public Entity
{

public:
    virtual ~Tower(){};
    using P = Map::Point::PointType;

    virtual std::shared_ptr<Tower> createTower(Direction _direction) = 0;
    virtual std::unique_ptr<Projectile> createProjectile(Map::Point &p) = 0;

    void addActiveProjectile(std::shared_ptr<Projectile> projectile);
    //assigns projectiles first point - point where it generates
    void assignStartToPoint();
    void eraseActiveProjectile(std::vector<std::shared_ptr<Projectile>>::const_iterator it);
    void clearActiveProjectiles();

    int getProjectileFrequency() const { return projectile_frequency; }
    char getProjectileDisplay() const { return projectile_display; }
    int getPrice() const { return price; }
    int getTowerType() const { return tower_type; }
    Direction getDirection() const { return direction; }
    const std::vector<P> &getPlacablePoints() const { return placable_points; }
    const std::vector<P> &getProjectilePlacablePoints() const { return projectile_placable_points; }
    const std::vector<std::shared_ptr<Projectile>> &getActiveProjectiles() const { return active_projectiles; }
    std::vector<std::shared_ptr<Projectile>> active_projectiles;

protected:
    Tower(const std::string &file_path, const std::string tower_name);

    // after how long will tower shoot another projectile
    int projectile_frequency;
    char projectile_display;
    int price;
    // when game is saved program have to know what tower to create to load it properly
    int tower_type;
    // direction which tower is facing
    Direction direction;

    std::vector<P> placable_points = {P::Water, P::Space};
    // point projectile is able to print on
    std::vector<P> projectile_placable_points = {P::Space, P::Water};
    // vector of projectiles that are right now on the map
};

/**
 * @brief specific Tower - child of Tower class
 *
 */
class BasicTower : public Tower
{
public:
    BasicTower(std::string file_path) : Tower(file_path, "(BasicTower[^=]+)") { tower_type = 0; };
    ~BasicTower() = default;

    std::shared_ptr<Tower> createTower(Direction _direction) override;
    std::unique_ptr<Projectile> createProjectile(Map::Point &p) override { return std::make_unique<Projectile>(direction, projectile_display, p); }
};

// ICE TOWER
//========================================================
/**
 * @brief specific Tower - child of Tower class
 *
 */
class IceTower : public Tower
{
public:
    IceTower(std::string file_path) : Tower(file_path, "(IceTower[^=]+)") { tower_type = 1; };
    ~IceTower() = default;

    std::shared_ptr<Tower> createTower(Direction _direction) override;
    std::unique_ptr<Projectile> createProjectile(Map::Point &p) override { return std::make_unique<Projectile>(direction, projectile_display, p); }
};

// FIRE TOWER
//========================================================
/**
 * @brief specific Tower - child of Tower class
 *
 */
class FireTower : public Tower
{
public:
    FireTower(std::string file_path) : Tower(file_path, "(FireTower[^=]+)") { tower_type = 2; };
    ~FireTower() = default;

    std::shared_ptr<Tower> createTower(Direction _direction) override;
    std::unique_ptr<Projectile> createProjectile(Map::Point &p) override { return std::make_unique<Projectile>(direction, projectile_display, p); }
};

// GREAT TOWER
//========================================================
/**
 * @brief specific Tower - child of Tower class
 *
 */
class GreatTower : public Tower
{
public:
    GreatTower(std::string file_path) : Tower(file_path, "(GreatTower[^=]+)") { tower_type = 3; };

    ~GreatTower() = default;

    std::shared_ptr<Tower> createTower(Direction _direction) override;
    std::unique_ptr<Projectile> createProjectile(Map::Point &p) override { return std::make_unique<Projectile>(direction, projectile_display, p); }
};

//==================================================
//                  Enemies
//==================================================
/**
 * @brief "blueprint" for all the enemies in the game - child of Entity class
 *
 */
class Enemy : public Entity
{

public:
    virtual ~Enemy(){};
    const int getHealth()const{return health;};
    const int getCurrentPoint()const{return current_point;}
    void addCurrentPoint(){current_point++;}
    void setCurrentPointLoad(const int current_point_load){current_point = current_point_load;}
    void dealDamage(const int damage){health -= damage;}
    const int getMoney()const{return money;}
    const int getScore()const{return score;}
    const std::shared_ptr<std::vector<Map::Point>>& getPath()const{return path;}


protected:
    Enemy(const std::string &file_path, std::string enemy_name, std::shared_ptr<std::vector<Map::Point>> _path);
    int health;
    // point where enemy is on the map - it is used as an index of path
    int current_point = 0;

    // worthness of enemy - how much money and score it will give to player upon its death
    int money;
    int score;

    // when game is saved I have to know what tower to create to load it properly
    // int enemy_type;
    std::shared_ptr<std::vector<Map::Point>> path;
};

/**
 * @brief specific Enemy - child of Enemy class
 *
 */
class Goblin : public Enemy
{
public:
    Goblin(std::string file_path, std::shared_ptr<std::vector<Map::Point>> _path) : Enemy(file_path, "(Goblin[^=]+)", _path){};
    ~Goblin() = default;
};

/**
 * @brief specific Enemy - child of Enemy class
 *
 */
class Dragon : public Enemy
{
public:
    Dragon(std::string file_path, std::shared_ptr<std::vector<Map::Point>> _path) : Enemy(file_path, "(Dragon[^=]+)", _path){};
    ~Dragon() = default;
};

/**
 * @brief points specific enemies can walk on
 *
 */
class EnemiesPlacablePoint
{
public:
    using P = Map::Point::PointType;
    const std::vector<P> goblin_placable_points = {P::End, P::Space};
    const std::vector<P> dragon_placable_points = {P::End, P::Water, P::Space};
};
