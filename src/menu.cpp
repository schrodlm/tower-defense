#include <iostream>
#include <ncurses.h>
#include "menu.h"

MenuBar create_menu(WINDOW *win)
{

    box(win, 0, 0);

    Menu Game("Game", 'g');
    Game.AddOption(Option("New Game", NEW_GAME_OPTION));
    Game.AddOption(Option("Load Game", LOAD_GAME_OPTION));

    Menu Settings("Settings", 's');
    Settings.AddOption(Option("Color Theme", 5, COLOR_THEME_OPTION));
    Settings.AddOption(Option("Difficulty", 3, DIFFICULTY_OPTION));

    Menu About("About", 'a');
    About.AddOption(Option("Author", AUTHOR_OPTIONS));
    About.AddOption(Option("Game", GAME_OPTION));
    About.AddOption(Option("Levels", LEVELS_OPTION));

    MenuBar menu(win);
    menu.add(Game)
        .add(Settings)
        .add(About);

    return menu;
}

OptionReturns menu_handler(const int chosen_option)
{
    switch (chosen_option)
    {
    case NEW_GAME_OPTION:
        return NEW_GAME_OPTION;

    case LOAD_GAME_OPTION:
        return LOAD_GAME_OPTION;

    case COLOR_THEME_OPTION:
        return COLOR_THEME_OPTION;
        break;

    case DIFFICULTY_OPTION:
        return DIFFICULTY_OPTION;
        break;

    case AUTHOR_OPTIONS:
        return AUTHOR_OPTIONS;
        break;

    case GAME_OPTION:
        return LEVELS_OPTION;
        break;

    case LEVELS_OPTION:
        wprintw(stdscr, "Case 5");
        wrefresh(stdscr);
        return LEVELS_OPTION;
        break;
    }
    return NO_OPTION_CHOSEN;
}
