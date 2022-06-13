#include "level.h"

int Game::GameInfo::score = 0;
Level::Level(const std::string map_file_path, const int level_number)
{
    map = std::make_unique<Map>(map_file_path);
    game = std::make_unique<Game>(*map, level_number);
}
void Level::initialization()
{

    // creates a window for this level
    game->createGameWindow();
    // creates info window for this level - based on game window size
    game->createInfoWindow();
    // creates a subwindow of game - map
    map->createMapSubwindow(game->game_win);

    // prints map it to terminal
    map->printMap();

    // creates all other subwindows for game - highlight, score, tower
    game->createSubwins();

    // tower definitions loaded from file into a vector - using abstract class Tower
    game->towerDefinition();

    // prints out score
    game->scoreHandler();

    //sets highscore from a file
    game->setHighscore();

    // prints out other information about current level - enemies, lives, etc.
    game->highlightHandler();

    // prints out towers available for purchase
    game->towerDraw();


    //creates a path for enemies with bfs algorithm
    game->entityPath();
    
    game->printInfo("New level!");
}

Level::levelReturn Level::levelLoop()
{
    while(game->gameinfo.enemies_alive > 0 && game->gameinfo.health > 0)
    {
        game->loop_counter++;
        // tower handler
        game->towerHandler();
        // projectile handler
        game->placeProjectile();
        //enemies moving every four rounds
        if(game->loop_counter % 4 == 0)
        {
            game->moveEnemies();
        }
        game->map.printMap();

        usleep(120000); // delay
    }

    //tady by mohla byt chyba - kdyz se ukonci hra
    game->deleteAllWindows();


    //game was won
    if(game->gameinfo.health > 0)
        return levelWon();
    else
        return levelLost();

}

Level::levelReturn Level::levelLost()
    {
        WINDOW* lostWin = newwin(10,40,0,0);
        box(lostWin, 0,0);
        
        centerWindow(lostWin);
        refresh();
        mvwprintw(lostWin,1,1, "You lost!");
        std::string score = "Score: ";
        score += std::to_string(game->gameinfo.score);
        mvwprintw(lostWin,3,1, score.c_str());
        mvwprintw(lostWin,5,1, "To exit press Enter" );
        wrefresh(lostWin);
        getch();
        return levelReturn::LEAVE;
    }


Level::levelReturn Level::levelWon()
{

        WINDOW* wonWin = newwin(10,40,0,0);
        box(wonWin, 0,0);
        centerWindow(wonWin);
        refresh();
        std::string highscore;
        if(game->gameinfo.score > game->gameinfo.highscore)
        {
            game->setHighscore();
            highscore = "You set a new highscore: ";
            highscore += std::to_string(game->gameinfo.score);
        }
        else
        {
            highscore = "Highscore: ";
            highscore += std::to_string(game->gameinfo.highscore);
        }
        mvwprintw(wonWin,1,1, "Level completed!");
        std::string score = "Score: ";
        score += std::to_string(game->gameinfo.score);
        mvwprintw(wonWin,3,1, score.c_str());
        mvwprintw(wonWin,5,1, highscore.c_str());
        mvwprintw(wonWin,7,1, "Enter next level y/n!");

        centerWindow(wonWin);
        wrefresh(wonWin);
        refresh();

                
        while(1){
            int direction_key = getch();
            switch (direction_key)
            {
            case ('y'):
                return levelReturn::NEXT;
                delwin(wonWin);
                break;
            case ('n'):
                delwin(wonWin);
                return levelReturn::LEAVE;
                break;
            }
        }
}

void Level::loadGame()
{
    game->loadGame();
}