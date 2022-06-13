#ifndef GAME_H_
#define GAME_H_

#include <iostream>
#include <ncurses.h>
#include <unordered_map>
#include "map.h"
#include "terminal.h"
#include "towerhandler.h"
#include "entity.h"



/**
 * @brief represents one level - it creates windows for the level, have count of all the active towers, money, etc.
 *      - level manager
 * 
 */
class Game
{
public:
        Game(Map &_map, const int level_number) : map(_map)
        {
                gameinfo.level = level_number;
        };
        ~Game() = default;


        void createGameWindow();
        void createInfoWindow();
        // create subwins of game window
        void createSubwins();
        void scoreHandler()const;
        void highlightHandler()const;
        void towerHandler();
        void towerDefinition();
        void towerDraw() const;
        void directionDraw();
        void individualDirection(WINDOW* win,std::string str_direction, std::string str_press);
        void placeTower(int x, int y, Map::Point &prev);
        void placeTower(int x, int y);
        void placeTower(int x, int y, int tower_type);
        void placeProjectile();
        void printInfo(std::string str);
        
        void deleteAllWindows();
        void moveEnemies();
        void loadEnemies();
        void saveGame();
        void loadGame();
        void entityPath();
        void loadHighscore();
        void setHighscore();


        /**
         * @brief information about the game - displayed in highlight and score subwindow
         *
         */
        struct GameInfo
        {
                static int score;
                int highscore;
                int level = 0;
                int money = 100;
                //first is current enemies, second is max enemies - first is counting down ot zero
                int enemies = 20;
                int enemies_alive = enemies;
                int health = 30;
                
        } gameinfo;

       //game window and its size 
        WINDOW *game_win; 
        std::pair<int, int> game_win_size;
       
        //subwindows of game_win
        WINDOW *tower_subwin, *score_subwin, *highlight_subwin;
        //small window above game win that prints current information for user
        WINDOW *info_win;
        //map generated for this game - level
        Map &map;
        int loop_counter = 0;
        int selected_tower = 0;
        //using it in moveEnemies - counts how many enemies were created
        int created_enemies = 0;

        std::vector<std::shared_ptr<Enemy>> enemies;
        // contains subwindow that is holding specific tower -> used when player is purchasing tower
        std::vector<std::pair<std::shared_ptr<Tower>, WINDOW *>> subwins;
        // towers placed on map
        std::vector<std::pair<std::shared_ptr<Tower>, Map::Point>> active_towers;
};

#endif