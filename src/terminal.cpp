#include <ncurses.h>
#include <string>
#include <sstream>
#include <map>
#include <iostream>
#include <algorithm>
#include <regex>
#include <fstream>
#include "terminal.h"


/**
 * @brief Create a New Subwindow object
 *
 * @param win
 * @return WINDOW*
 */
WINDOW *createNewSubwindow(WINDOW *win, int height, int width, int starty, int startx)
{
    WINDOW *subwin;
    touchwin(win);
    subwin = derwin(win, height, width, starty, startx);
    box(subwin, 0, 0);
    wrefresh(subwin);
    return subwin;
}

/**
 * @brief
 *
 * @param win
 */
void clearWindow(WINDOW *win)
{
    wclear(win);
    box(win, 0, 0);
}

/**
 * @brief center a window based on stdscr (terminal screen)
 *
 * @param win   --> provided window
 */
void centerWindow(WINDOW *win)
{

    int max_y, max_x;
    int max_game_win_y, max_game_win_x;

    getmaxyx(stdscr, max_y, max_x);
    getmaxyx(win, max_game_win_y, max_game_win_x);
    mvwin(win, max_y / 2 - max_game_win_y / 2, max_x / 2 - max_game_win_x / 2);
}

WINDOW *infoWindow(std::string title, std::string text)
{
    clear();
    WINDOW *info = newwin((text.size() / 80) + 6, 80, 0, 0);
    centerWindow(info);
    box(info, 0, 0);
    int max_x = getmaxx(info);
    mvwprintw(info, 0, max_x / 2 - title.size(), title.c_str());
    wmove(info,2 , 2);

    int row_max = 0;
    int moving = 3;
    for (int i = 0; i < (int)text.size(); i++)
    {
        if (row_max == (max_x - 4))
        {
            wmove(info, moving, 2);
            row_max = 0;
            moving++;
        }

        waddch(info, text[i]);
        row_max++;
    }
    // waddnstr(info,text.c_str(),5);
    refresh();
    wrefresh(info);

    getch();
    wclear(info);
    wrefresh(info);
    delwin(info);

    return info;
}


//LOADING FROM A FILE
std::map<std::string,std::string> loadFromFile(const std::string &file_path, const std::string name)
{
        //FILE HANDLING
    //===========================================

    //pass file do a buffer
    std::stringstream buffer;
    std::ifstream file(file_path, std::ios::out);
    //error handling
    if (!file)
        throw std::runtime_error("File doesn't exist");

    buffer << file.rdbuf();

    //GETTING THE RIGHT PART OF FILE FOR A SPECIFIED TOWER
    //====================================================
    std::string str = buffer.str();
    //the paragraph of specified tower
    std::smatch matched_file_part;
    std::regex reg_file_part(name);
    regex_search(str, matched_file_part,reg_file_part);
    std::string file_part = matched_file_part.str(1);
    
    //EXTRACTING THE ATTRIBUTES FOR SPECIFIED TOWER
    //==========================================    
    
    //regex for a specified tower
    std::regex reg_tower("\n([A-Za-z_]+) : '([^']*)'");
    //map of names and values of attributes
    std::map<std::string, std::string> map;
    std::smatch matches;
    
    //PASSING THESE ATTRIBUTES INTO A MAP
    //=========================================
    while (regex_search(file_part, matches, reg_tower))
    {
        map.insert(make_pair(matches.str(1), matches.str(2)));
        file_part = matches.suffix().str();
    }

    return map;
}

// Returns true if terminal is 80x25
bool Terminal::right_size()
{
	getmaxyx(stdscr,max_y,max_x);

	x_ofs = (max_x-80)/2;
	y_ofs = (max_y-25)/2;

	return ((max_x>=80)&&(max_y>=25));
}