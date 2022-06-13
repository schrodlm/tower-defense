#include <iostream>
#include <ncurses.h>
#include <cxxabi.h>
#include <algorithm>
#include "gameplay.h"
#include "map.h"
#include "terminal.h"
#include "entity.h"
#include "towerhandler.h"

/**
 * @brief creates a window for a game
 *
 * @return WINDOW*
 */
void Game::createGameWindow()
{
    game_win = newwin(map.map_size.second + 11, map.map_size.first + 31, 0, 0);

    getmaxyx(game_win, game_win_size.second, game_win_size.first);
    centerWindow(game_win);

    box(game_win, 0, 0);
}

/**
 * @brief creates a small window above the game window that prints info about your actions
 *
 */
void Game::createInfoWindow()
{

    // create info window one row above game win
    int game_win_y, game_win_x;
    getbegyx(game_win, game_win_y, game_win_x);
    info_win = newwin(1, 40, game_win_y - 1, game_win_x);

    wrefresh(info_win);
}

/**
 * @brief   creates all subwindows, each for different part of the game
 *          - uses generic function from Terminal class
 *
 */
void Game::createSubwins()
{
    // createNewSubwindow(game_win, height, width, starty,startx);

    score_subwin = createNewSubwindow(game_win, 3, game_win_size.first - map.map_win_size.first - 2, 1, map.map_win_size.first + 1);
    highlight_subwin = createNewSubwindow(game_win, map.map_size.second - 1, game_win_size.first - map.map_win_size.first - 2, 4, map.map_size.first + 3);
    tower_subwin = createNewSubwindow(game_win, game_win_size.second - map.map_win_size.second - 2, game_win_size.first - 2, map.map_win_size.second + 1, 1);
    wrefresh(game_win);
}

//==================================================
//                  Handlers
//==================================================

/**
 * @brief prints out score
 *
 */
void Game::scoreHandler()const
{
    wmove(score_subwin, 1, 1);
    wprintw(score_subwin, "%s%d", "Score: ", gameinfo.score);
    wrefresh(score_subwin);
}

/**
 * @brief prints out game stats
 *
 */
void Game::highlightHandler()const
{
    wclear(highlight_subwin);
    box(highlight_subwin, 0, 0);
    wmove(highlight_subwin, 1, 1);
    wprintw(highlight_subwin, "%s%d", "Level: ", gameinfo.level);
    wmove(highlight_subwin, 3, 1);
    wprintw(highlight_subwin, "%s%d", "Money: ", gameinfo.money);
    wmove(highlight_subwin, 5, 1);
    wprintw(highlight_subwin, "%s%d", "Enemies: ", gameinfo.enemies_alive);
    wmove(highlight_subwin, 6, 1);
    wprintw(highlight_subwin, "%s%d", "Health: ", gameinfo.health);

    wrefresh(highlight_subwin);
}

/**
 * @brief creates a vector of all existing towers so that I can use it to create towers
 *
 */
void Game::towerDefinition()
{

    int max_height, max_width;
    getmaxyx(tower_subwin, max_height, max_width);
    int lenght_x = 1;

    // pointers to all existing towers
    std::vector<std::shared_ptr<Tower>> towers = towerLoading();

    // creates subwindow of tower_subwin for each tower
    for (auto &i : towers)
    {
        WINDOW *subwindow = createNewSubwindow(tower_subwin, max_height - 2, max_width / 4, 1, lenght_x);
        subwins.push_back(std::make_pair(i, subwindow));
        lenght_x += max_width / 4;
    }
}

/**
 * @brief draws tower and information about it to a window - highlight the one player has currently picked
 *
 */
void Game::towerDraw() const
{
    for (int i = 0; i < (int)subwins.size(); i++)
    {

        if (i == selected_tower)
            wbkgd(subwins[i].second, COLOR_PAIR(2));
        else
        {
            wbkgd(subwins[i].second, COLOR_PAIR(1));
        }
        // print tower display
        wmove(subwins[i].second, 0, 9);
        waddch(subwins[i].second, subwins[i].first->getDisplay());

        // print name of the tower
        wmove(subwins[i].second, 1, 2);
        // wattron(tower_subwin,COLOR_PAIR(2));
        int status;
        // prints demangled version of class name
        char *demangled = abi::__cxa_demangle(typeid(*(subwins[i].first)).name(), 0, 0, &status);
        wprintw(subwins[i].second, demangled);
        // wattroff(tower_subwin,COLOR_PAIR(2));

        wmove(subwins[i].second, 2, 2);

        wprintw(subwins[i].second, "%s%d", "Damage: ", subwins[i].first->getDamage());
        wmove(subwins[i].second, 3, 2);
        wprintw(subwins[i].second, "%s%d", "Price: ", subwins[i].first->getPrice());

        wrefresh(subwins[i].second);
        free(demangled);
    }
}

/**
 * @brief submethod of directionDraw() - prints out info into a single window - there are 4 windows in total
 *
 * @param win               --> one of 4 windows
 * @param str_direction     --> string with direction info
 * @param str_press         --> string with info about pressing a key
 */
void Game::individualDirection(WINDOW *win, std::string str_direction, std::string str_press)
{
    int max_x = getmaxx(win);

    wbkgd(win, COLOR_PAIR(1));
    wclear(win);
    box(win, 0, 0);
    wmove(win, 1, max_x / 2 - str_direction.length() / 2);

    wprintw(win, "%s", str_direction.c_str());
    wmove(win, 3, max_x / 2 - str_press.length() / 2);
    wprintw(win, "%s", str_press.c_str());
    wrefresh(win);
}
/**
 * @brief prints out a direction window for tower that was just bought
 *
 */
void Game::directionDraw()
{

    individualDirection(subwins[0].second, " Left ", "Press <");
    individualDirection(subwins[1].second, " Up ", "Press ^");
    individualDirection(subwins[2].second, " Down ", "Press v");
    individualDirection(subwins[3].second, " Right ", "Press >");
}

/**
 * @brief handles buying a putting tower on the map - also moving in the tower menu
 *
 */
void Game::towerHandler() 
{
    Direction direction;
    nodelay(tower_subwin, TRUE);
    keypad(tower_subwin, TRUE);
    int pressed_key;
    pressed_key = wgetch(tower_subwin);
    wrefresh(tower_subwin);

    // wait for pressed key - moving between towers
    switch (pressed_key)
    {
        // pause the game - 27 - code for ESC
    case ('p'):
    case (27):
        printInfo("Game Paused!");
        getch();
        printInfo("Game Resumed!");
        towerDraw();
        break;

    case ('s'):
        saveGame();
        printInfo("Game saved!");
        break;

    case (KEY_RIGHT):

        selected_tower++;
        selected_tower %= subwins.size();
        towerDraw();
        break;

    case (KEY_LEFT):
        selected_tower--;

        if (0 > selected_tower)
            selected_tower = subwins.size() - 1;

        towerDraw();
        break;

    case ('\n'):

        int y = 0;
        int x = 0;
        int exit = 0;
        Map::Point prev(x, y, map._map[y][x].c);

        // check if there is enough money to buy the tower
        if (gameinfo.money >= subwins[selected_tower].first->getPrice())
        {
            // this command unblock getch() so the game can move even when player isnt picking anything
            nodelay(tower_subwin, false);

            // SETTING UP DIRECTION FOR THE TOWER THAT WILL BE CREATED
            //========================================================
            directionDraw();

            int direction_key = wgetch(tower_subwin);

            switch (direction_key)
            {

            case (KEY_RIGHT):
                direction = Direction::Right;
                break;
            case (KEY_LEFT):
                direction = Direction::Left;
                break;
            case (KEY_UP):
                direction = Direction::Up;
                break;
            case (KEY_DOWN):
                direction = Direction::Down;
                break;
            // if player didnt choose a direction - give him right - most common one
            default:
                direction = Direction::NoDirection;
                break;
            }
            for (auto &i : subwins)
            {
                wclear(i.second);
                box(i.second, 0, 0);
            }
            towerDraw();
            nodelay(tower_subwin, true);
            placeTower(x, y, prev);
            gameinfo.money -= subwins[selected_tower].first->getPrice();
            highlightHandler();
        }
        else
        {
            printInfo("Not enough money!");
            break;
        }

        while (exit == 0)
        {

            int key = wgetch(tower_subwin);
            switch (key)
            {
            case (KEY_RIGHT):
                x++;
                if (x >= map.map_size.first)
                    x = 0;
                placeTower(x, y, prev);
                break;
            case (KEY_LEFT):
                x--;
                if (x < 0)
                    x = map.map_size.first - 1;
                placeTower(x, y, prev);
                break;

            case (KEY_UP):
                y--;
                if (y < 0)
                    y = map.map_size.second - 1;
                placeTower(x, y, prev);
                break;

            case (KEY_DOWN):
                y++;
                if (y > map.map_size.second - 1)
                    y = 0;
                placeTower(x, y, prev);
                break;

            // PLACING TOWER
            case ('\n'):
                // checks if tower is placeable on that specific point
                if (std::find(subwins[selected_tower].first->getPlacablePoints().begin(),
                              subwins[selected_tower].first->getPlacablePoints().end(), prev.type) != subwins[selected_tower].first->getPlacablePoints().end())
                {
                    // tower was placed
                    placeTower(x, y);
                    printInfo("Tower placed!");
                    Map::Point p(x, y, subwins[selected_tower].first->getDisplay());
                    // add that tower to active_towers
                    active_towers.emplace_back(std::make_pair(subwins[selected_tower].first->createTower(direction), p));
                    exit = 1;
                    break;
                }
                else
                {
                    // tower was not in a placeable point
                    printInfo("Tower cannot be placed here!");
                    exit = 0;
                    break;
                }
            }
        }
    }
}

/**
 * @brief places a tower on the map and updates the places where tower was (returns the original display)
 *
 * @param x     --> x position
 * @param y     --> y position
 * @param prev  --> previous point (the point tower was before movement)
 */
void Game::placeTower(int x, int y, Map::Point &prev)
{
    Map::Point p(x, y, subwins[selected_tower].first->getDisplay());

    map._map[prev.y][prev.x] = prev;
    prev = map._map[y][x];

    map._map[y][x] = p;

    map.printMap();
}
/**
 * @brief overloaded placeTower() - used when we don't need to check prev point -when placing the tower on final spot
 *
 * @param x     --> x position
 * @param y     --> y position
 */
void Game::placeTower(int x, int y)
{
    Map::Point p(x, y, subwins[selected_tower].first->getDisplay());
    map._map[y][x] = p;
    map.printMap();
}
/**
 * @brief overloaded placeTower() - used when we don't need to check prev point - like when loading saved game
 *
 * @param x     --> x position
 * @param y     --> y position
 */
void Game::placeTower(int x, int y, int tower_type)
{
    Map::Point p(x, y, subwins[tower_type].first->getDisplay());
    map._map[y][x] = p;
    map.printMap();
}
void Game::placeProjectile()
{
    for (auto &i : active_towers)
    {
        //  tower generates projectile every projectile_frequency
        if (loop_counter % i.first->getProjectileFrequency() == 0)
        {

            i.first->addActiveProjectile(i.first->createProjectile(i.second));
            // assigning starting point on the map (next to its tower)
            i.first->assignStartToPoint();
        }
        // if tower has active projectiles -- move them
        for (auto l = i.first->getActiveProjectiles().begin(); l != i.first->getActiveProjectiles().end(); l++)
        {

            if ((*l)->prev_point != nullptr)
            {
                // if projectile has already moved -- delete its previous print on the map
                map._map[(*l)->prev_point->y][(*l)->prev_point->x] = *(*l)->prev_point;
            }
            (*l)->prev_point = std::make_unique<Map::Point>(map._map[(*l)->point->y][(*l)->point->x].x, map._map[(*l)->point->y][(*l)->point->x].y, map._map[(*l)->point->y][(*l)->point->x].c);

            // places projectile on the map
            map._map[(*l)->point->y][(*l)->point->x] = *(*l)->point;

            // checks if collision has occured
            if ((*l)->collisionCheck(map._map, i.first->getPlacablePoints()))
            {

                // check if it is an enemy
                if ((*l)->prev_point->type == Map::Point::PointType::Enemy)
                {
                    // if it is- find that enemy and deal damage
                    printInfo("Enemy hit");
                    for (auto it = enemies.begin(); it != enemies.end(); it++)
                    {
                        if ((*(*it)->getPath())[(*it)->getCurrentPoint()] == *(*l)->prev_point)
                        {
                            (*it)->dealDamage(i.first->getDamage());
                            if ((*it)->getHealth() < 0)
                            {
                                // if enemy died - delete it from map
                                map._map[(*(*it)->getPath())[(*it)->getCurrentPoint()].y][(*(*it)->getPath())[(*it)->getCurrentPoint()].x] = (*(*it)->getPath())[(*it)->getCurrentPoint()];
                                gameinfo.enemies_alive--;
                                gameinfo.money += (*it)->getMoney();
                                gameinfo.score += (*it)->getScore();
                                // print new info onto the screen
                                highlightHandler();
                                enemies.erase(it);

                                printInfo("Enemy killed");
                            }
                            else
                            {
                                map._map[(*l)->prev_point->y][(*l)->prev_point->x] = *(*l)->prev_point;

                            } // i have to break the loop if vector is empty
                            if (enemies.empty())
                                break;
                        }
                    }
                }

                // if it collided with anything else I have to print it back

                else
                {
                    map._map[(*l)->prev_point->y][(*l)->prev_point->x] = *(*l)->prev_point;
                }
                i.first->eraseActiveProjectile(l);
                // it the break isnt here, if you place two towers next to each other - it will create a segment fault because l will be undefined
                break;

                // i have to break the loop if vector is empty
                if (i.first->getActiveProjectiles().empty())
                    break;
            }
            // move the point of the projectile
            (*l)->moving();
        }

        // after all projectiles have been moved -- print them on the map
    }
    // map.printMap();
}

/**
 * @brief   prints string to the info_win - that is placed right above game_win
 *          --> used to print players actions
 *
 */
void Game::printInfo(std::string str)
{
    wclear(info_win);
    wprintw(info_win, str.c_str());
    wrefresh(info_win);
}

/**
 * @brief creates a new enemy
 *
 */
void Game::loadEnemies()
{
    const std::string file_path = "assets/entities/enemies.txt";

    int spawn = rand() % 2;
    if (spawn == 1)
        enemies.emplace_back(new Goblin(file_path, std::make_shared<std::vector<Map::Point>>(map.path1)));

    else
        enemies.emplace_back(new Dragon(file_path, std::make_shared<std::vector<Map::Point>>(map.path2)));
}

/**
 * @brief move existing enemies on their path - created by bfs beforehand
 *
 */
void Game::moveEnemies()
{
    for (auto it = enemies.begin(); it != enemies.end(); it++)
    {
        (*it)->addCurrentPoint();
        Map::Point p = (*(*it)->getPath())[(*it)->getCurrentPoint()];

        map._map[p.y][p.x].c = (*it)->getDisplay();
        map._map[(*(*it)->getPath())[(*it)->getCurrentPoint() - 1].y][(*(*it)->getPath())[(*it)->getCurrentPoint() - 1].x] = (*(*it)->getPath())[(*it)->getCurrentPoint() - 1];

        if (map._map[p.y][p.x] == *map.start_end_points.second)
        {
            printInfo("Enemy reached the end");
            gameinfo.health -= (*it)->getDamage();
            gameinfo.enemies_alive--;
            highlightHandler();
            enemies.erase(it);

            // i have to break the loop if vector is empty
            if (enemies.empty())
                break;
        }

        // enemy hit tower - find what tower did he hit and delete it
        // TODO: predelat active towers na mapu<point,vez>
        if (map._map[p.y][p.x].type == Map::Point::PointType::Tower)
        {
            for (auto i = active_towers.begin(); i != active_towers.end(); i++)
            {
                if (i->second.x == p.x && i->second.y == p.y)
                {
                    //deletes all towers projectiles on the map
                    for (auto l = i->first->getActiveProjectiles().begin(); l != i->first->getActiveProjectiles().end(); l++)
                    {
                        map._map[(*l)->prev_point->y][(*l)->prev_point->x] = *(*l)->prev_point;
                    }
                    i->first->clearActiveProjectiles();
                    active_towers.erase(i);
                    printInfo("Tower was destroyed!");
                    break;
                }
            }
        }
    }
    // 20 - because one enemy spawns every 4 rounds and I wanted them to have 5 spaces between them - 4*5=20
    if (loop_counter % 20 == 0 && gameinfo.enemies > created_enemies)
    {
        created_enemies++;
        loadEnemies();
    }
}
/**
 * @brief saves the game to a file - later can be loaded
 *
 */
void Game::saveGame()
{
    std::string map_string;

    for (auto &i : map._map_const)
    {
        for (auto &j : i)
        {
            map_string += j.c;
        }
        map_string += '\n';
    }

    std::ofstream out("assets/save/save_map.txt");

    out << map_string;

    // structure of the string has to be like this becuase thats what my regex handler later works with
    std::map<std::string, std::string> game_info_map;
    std::string game_info = "GAMEINFO\n--------------------\n";

    // GAMEINFO
    game_info_map.emplace("score : '", std::to_string(gameinfo.score));
    game_info_map.emplace("level : '", std::to_string(gameinfo.level));
    game_info_map.emplace("money : '", std::to_string(gameinfo.money));
    game_info_map.emplace("enemies : '", std::to_string(gameinfo.enemies));
    game_info_map.emplace("enemies_alive : '", std::to_string(gameinfo.enemies_alive));
    game_info_map.emplace("health : '", std::to_string(gameinfo.health));

    game_info_map.emplace("loop : '", std::to_string(loop_counter));
    game_info_map.emplace("created_enemies : '", std::to_string(created_enemies));

    for (auto &i : game_info_map)
    {
        game_info += i.first;
        game_info += i.second + '\'';
        game_info += '\n';
    }
    game_info += "====================\n";

    std::ofstream out_info("assets/save/save_game_info.txt");
    std::ofstream out_entities("assets/save/save_entities_info.txt");
    out_info << game_info;

    // TOWERS
    std::string towers_str;
    for (auto &i : active_towers)
    {
        towers_str += "tower_type " + std::to_string(i.first->getTowerType()) + '\n';
        towers_str += "direction " + std::to_string(static_cast<int>(i.first->getDirection())) + '\n';
        towers_str += "x " + std::to_string(i.second.x) + '\n';
        towers_str += "y " + std::to_string(i.second.y) + '\n';
        towers_str += '\n';
    }

    // ENEMIES
    std::string enemies_str;

    for (auto &i : enemies)
    {
        enemies_str += "current_point " + std::to_string(i->getCurrentPoint()) + '\n';
    }

    out_entities << towers_str;
    out_entities << enemies_str;
}

/**
 * @brief frees the memory after level has been completed
 *
 */
void Game::deleteAllWindows()
{
    clear();
    delwin(tower_subwin);
    delwin(score_subwin);
    delwin(highlight_subwin);
    delwin(game_win);
    delwin(info_win);
    refresh();
}

void Game::entityPath()
{
    EnemiesPlacablePoint p;
    map.path1 = map.findPath(p.goblin_placable_points);
    map.path2 = map.findPath(p.dragon_placable_points);
}
/**
 * @brief loads up saved game from save files
 * 
 */
void Game::loadGame()
{
//GAME INFO
//========================================================

    //game info loaded from generic function loadFromFile()

    std::map<std::string, std::string> game_info = loadFromFile("assets/save/save_game_info.txt", "(GAMEINFO[^=]+)");
    gameinfo.enemies = std::stoi(game_info["enemies"]);
    gameinfo.score = std::stoi(game_info["score"]);
    gameinfo.level = std::stoi(game_info["level"]);
    gameinfo.money = std::stoi(game_info["money"]);
    gameinfo.enemies_alive = std::stoi(game_info["enemies_alive"]);
    gameinfo.health = std::stoi(game_info["health"]);
    created_enemies = std::stoi(game_info["created_enemies"]);

    //prints it out for player to see
    highlightHandler();
    scoreHandler();

//LOADING ENEMIES AND TOWERS
//========================================================

    std::fstream savefile;
    std::string fileLine;
    std::string name;
    std::map<std::string, int> m;
    savefile.open("assets/save/save_entities_info.txt", std::ios::in); // open a file to perform read operation using file object

    if (!savefile.is_open())
        throw std::runtime_error("Mistake reading file"); // checking whether the file is open

    while (getline(savefile, fileLine))
    { 
        // line was empty
        if (fileLine.empty())
            continue;

        std::stringstream ss(fileLine);

        int numvalue = -1;
        name.clear();
        ss >> name;
        ss >> numvalue;
        if (numvalue == -1)
            throw std::runtime_error("Mistake reading the value");

        m.emplace(name, numvalue);

        // tower loaded
        if (name == "y")
        {
            placeTower(m["x"], m["y"], m["tower_type"]);
            Map::Point p(m["x"], m["y"], subwins[m["tower_type"]].first->getDisplay());
            active_towers.emplace_back(std::make_pair(subwins[m["tower_type"]].first->createTower(static_cast<Direction>(m["direction"])), p));
            m.clear();
        }
        // enemy loaded
        if (name == "current_point")
        {
            loadEnemies();
            enemies.back()->setCurrentPointLoad(m["current_point"]);
            m.clear();
        }
    }
    savefile.close(); // close the file object.
}

void Game::loadHighscore()
{
    std::string filename("assets/highscore/highscore.txt");
    int number;

     std::ifstream input_file(filename);
    if (!input_file.is_open()) 
        throw std::runtime_error("File doesn't exist");
     
     input_file >> number;

     gameinfo.highscore = number;
    
}

void Game::setHighscore()
{
    std::string filename("assets/highscore/highscore.txt");

    std::ofstream out_file(filename);

     
     out_file << std::to_string(gameinfo.highscore);
    
}