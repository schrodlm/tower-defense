#pragma once

#include <ncurses.h>
#include <map>
#include <string>


/**
 * @brief Sets up some useful generic functions and rules for ncurses to follow a behave by
 * 
 */
class Terminal
{
public:
    Terminal()
    {
        // ncurses setup --> later also in its own class Terminal
        initscr();
        cbreak();
        noecho();
        // no cursor is shown in terminal
        curs_set(0);
        start_color();
        keypad(stdscr, TRUE);
        getmaxyx(stdscr, max_y, max_x);

        init_pair(2, COLOR_CYAN, COLOR_BLACK);
        init_pair(1, COLOR_WHITE, COLOR_BLACK);
    }

    ~Terminal()
    {
        endwin();
    }

    // Returns true if terminal is 80x25
    bool right_size();
    
    
    int x_ofs = 0;
    int y_ofs = 0;
    int max_y, max_x = 0;

};
    WINDOW *createNewSubwindow( WINDOW* win, int height, int width, int starty, int startx);
    void clearWindow(WINDOW *win);
    void centerWindow(WINDOW *win);
    WINDOW* infoWindow(std::string title, std::string text);
    std::map<std::string, std::string> loadFromFile(const std::string &file_path, const std::string name);
    
    
