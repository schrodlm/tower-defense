#include "level.h"

using levelReturn = Level::levelReturn;

int main()
{

    Terminal term;

    //chcecks if terminal is right size
    if (!term.right_size())
	{
		endwin();
		fprintf(stderr,"TowerDefense must be run in an 80x25 terminal!\n");
		return -1;
	}

    //MENU
    //====================================================
    OptionReturns option = NO_OPTION_CHOSEN;
    while (option != NEW_GAME_OPTION && option != LOAD_GAME_OPTION)
    {
        // creates a menu in window
        MenuBar menu = createMenu(term);

        // looping menu until option is picked
        option = menu.menuLoop();
    }

    // user wants to load a game
    if (option == LOAD_GAME_OPTION)
    {
        Level level_load("assets/save/save_map.txt", 0);
        level_load.initialization();
        level_load.loadGame();
        switch (level_load.levelLoop())
        {
        case levelReturn::LEAVE:
            return 0;
        case levelReturn::NEXT:
            break;
        }
    }

    //INDIVIDUAL LEVELS
    //====================================================
    
    // LEVEL 1
    Level level1("assets/maps/map1.txt", 1);
    level1.initialization();
    switch (level1.levelLoop())
    {
    case levelReturn::LEAVE:
        return 0;
    case levelReturn::NEXT:
        break;
    }

    // LEVEL 2
    Level level2("assets/maps/map2.txt", 2);
    level2.initialization();
    switch (level2.levelLoop())
    {
    case levelReturn::LEAVE:
        return 0;
    case levelReturn::NEXT:
        break;
    }

    // LEVEL 3
    Level level3("assets/maps/map3.txt", 3);
    level3.initialization();
    switch (level3.levelLoop())
    {
    case levelReturn::LEAVE:
        return 0;
    case levelReturn::NEXT:
        break;
    }

    getch();
}
