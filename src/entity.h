#include <iostream>
#include <fstream>
#include <iostream>
#include <ncurses.h>
#include <sstream>
#include <string>
#include <algorithm>
#include <map>
#include <regex>

#ifndef _ENTITY_H_
#define _ENTITY_H_

class Entity
{
public:
   
    void operator= (const Entity&) = delete;
    virtual ~Entity(){};

    virtual void print(const int x, const int y) = 0;



    protected:
    Entity(){};
    int damage;
    int seeing_distance;
};

//===============Towers================

class Tower : public Entity
{
public:
    Tower(const std::string &file_path, const std::string tower_name);
    virtual ~Tower(){};
    virtual void print(const int x, const int y) = 0;
};

class BasicTower : public Tower
{
public:
    BasicTower(std::string file_path) : Tower(file_path, "(BasicTower[^=]+)"){};

    ~BasicTower() = default;
    void print(const int x, const int y){};
};

class IceTower : public Tower
{
    public:
    IceTower(std::string file_path) : Tower(file_path, "(IceTower[^=]+)"){};

    ~IceTower() = default;
    void print(const int x, const int y){};
};

class FireTower : public Tower
{
    public:
    FireTower(std::string file_path) : Tower(file_path, "(FireTower[^=]+)"){};

    ~FireTower() = default;
    void print(const int x, const int y){};
};

// //===============Enemies================

// class Enemy : Entity
// {
//     Enemy();
//     virtual ~Enemy();
//     int speed;
//     bool flying;
// };

// class Goblin : Enemy
// {

// };

// class Dragon : Enemy
// {

// };

#endif