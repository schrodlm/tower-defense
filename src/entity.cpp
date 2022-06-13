#include <iostream>
#include <fstream>
#include <iostream>
#include <ncurses.h>
#include <sstream>
#include <string>
#include <algorithm>
#include <map>
#include <regex>
#include "entity.h"
#include "terminal.h"

/**
 * @brief Construct a new Tower:: Tower object from a file
 *
 * @param file_path     --> path to a tower attributes file
 * @param tower_name    --> name of the child class that uses this constructor
 */
Tower::Tower(const std::string &file_path, const std::string tower_name)
{

    std::map<std::string, std::string> tower_attributes = loadFromFile(file_path, tower_name);
    //DISTRIBUTTING THESE ATTRIBUTES FROM THE MAP TO VARIABLES
    //=========================================
    damage = atoi(tower_attributes["Damage"].c_str());
    display = tower_attributes["Display"][0];
    price = atoi(tower_attributes["Price"].c_str());
    projectile_frequency = atoi(tower_attributes["Projectile_frequency"].c_str());
    projectile_display = tower_attributes["Projectile_display"][0];
}


//===========ENEMIES CONSTRUCTOR


Enemy::Enemy(const std::string &file_path, const std::string enemy_name, std::shared_ptr<std::vector<Map::Point>> _path )
{
    path = _path;
    std::map<std::string, std::string> enemies_attributes = loadFromFile(file_path, enemy_name);

    //DISTRIBUTTING THESE ATTRIBUTES FROM THE MAP TO VARIABLES
    //========================================================
    
    //TODO: udelat to pres std::to_int
    damage = atoi(enemies_attributes["Damage"].c_str());
    display = enemies_attributes["Display"][0];
    health = atoi(enemies_attributes["Health"].c_str());
    score = atoi(enemies_attributes["Score"].c_str());
    money = atoi(enemies_attributes["Money"].c_str());
}

    /**
     * @brief creates new tower of its own type
     * 
     * @param _direction 
     * @return std::shared_ptr<Tower> 
     */
        std::shared_ptr<Tower> BasicTower::createTower(Direction _direction) 
    {
        auto ptr = std::make_shared<BasicTower>(file_path);
        ptr->direction = _direction;
        return ptr;
    }

        std::shared_ptr<Tower> IceTower::createTower(Direction _direction)
    {
        auto ptr = std::make_shared<IceTower>(file_path);
        ptr->direction = _direction;
        return ptr;
    }


    std::shared_ptr<Tower> FireTower::createTower(Direction _direction)
    {
        auto ptr = std::make_shared<FireTower>(file_path);
        ptr->direction = _direction;
        return ptr;
    }

        std::shared_ptr<Tower> GreatTower::createTower(Direction _direction)
    {
        auto ptr = std::make_shared<GreatTower>(file_path);
        ptr->direction = _direction;
        return ptr;
    }

    void Tower::addActiveProjectile(std::shared_ptr<Projectile> projectile)
    {
        active_projectiles.emplace_back(projectile);
    }

    void Tower::assignStartToPoint()
    {
        active_projectiles.back()->point = active_projectiles.back()->starting_point;
    }

    void Tower::eraseActiveProjectile(std::vector<std::shared_ptr<Projectile>>::const_iterator it)
    {
        active_projectiles.erase(it);
    }

  void Tower::clearActiveProjectiles()
  {
    active_projectiles.clear();
  }
