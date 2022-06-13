#ifndef ENTITY_H_
#define ENTITY_H_
#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include "menu.h"
#include "map.h"
#include "entity.h"
#include "gameplay.h"
#include "terminal.h"
#include "towerhandler.h"

/**
 * @brief defines one level - its initialization and its loop
 * 
 */
class Level
{
    public:
    Level(const std::string map_file_path, const int level_number);

    ~Level(){};

    void initialization();
        enum class levelReturn : int
        {
            NEXT,
            LEAVE
        };
    levelReturn levelLoop();
    levelReturn levelWon();
    levelReturn levelLost();
    void loadGame();


    private:
        std::unique_ptr<Map> map;
        std::unique_ptr<class Game> game;

};



#endif

