#include <iostream>
#include <ncurses.h>
#include "menu.h"
#include "map.h"
#include "entity.h"
int main()
{
    //Entity loading --> later will be in its on header, here just for debugging
    BasicTower tower = BasicTower("data/entities/entities.txt");
    FireTower tower1 = FireTower("data/entities/entities.txt");
    IceTower tower2 = IceTower("data/entities/entities.txt");

    //ncurses setup --> later also in its own class Terminal
    initscr();
    cbreak();
    noecho();
    //no cursor is shown in terminal
    curs_set(0);
    keypad(stdscr, TRUE);



    // gets information about terminal
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    // creates a window for menu
    WINDOW *menu_win = newwin(max_y / 2, max_x / 2, max_y / 4, max_x / 4);

    // creates a menu in window menu
    MenuBar menu = create_menu(menu_win);

    // draws game menu
    wrefresh(menu_win);
    menu.draw('g');

    // moving in the menu
    int pressed_key;
    //chosen option from the menu
    OptionReturns chosen = NO_OPTION_CHOSEN;
    while (chosen != NEW_GAME_OPTION)
    {
        pressed_key = wgetch(menu_win);
        int chosen_option = menu.draw(pressed_key);

       chosen = menu_handler(chosen_option);
       
    }

    // clear window menu
    wclear(menu_win);
    wrefresh(menu_win);
  
    //generates map from a file to a string
    std::string map_str = read_map("data/maps/map1.txt");
    //std::string map_str = generate_map("../data/maps/map1.txt");
    
    Map map(map_str);

    //prints out a map
    map.print_map();
    
   

    getch();

    endwin();
}
