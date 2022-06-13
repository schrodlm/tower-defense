#include <ncurses.h>
#include <string>
#include <vector>
#include <memory> //unique_ptr
#include "terminal.h"

#pragma once

/**
 * @brief return options for menu handler
 * 
 */
enum OptionReturns : char
{
    NO_OPTION_CHOSEN = 0 ,
    NEW_GAME_OPTION,
    LOAD_GAME_OPTION,
    COLOR_THEME_OPTION,
    DIFFICULTY_OPTION,
    AUTHOR_OPTIONS,
    GAME_OPTION,
    LEVELS_OPTION
};

//==================================================
//                  Option
//==================================================

/**
 * @brief This class defines options for each menu
 *          -- for example menu Game has options "New Game" and "Load Game"
 *
 */
class Option
{
public:
    Option(std::string _name, OptionReturns _return_value);
    // overloaded contructor add max possible value to a setting
    Option(std::string _name, int _max_value, OptionReturns _return_value);
    ~Option() = default;

    OptionReturns getOptionReturn() {return return_value; }
    std::string getOptionName() { return option_name; }
    int getOptionValue() { return option_value; }
    bool getChangeValue() { return value_change; }

private:

    std::string option_name;
    OptionReturns return_value;

    // bool if the option is changeable
    bool value_change;
    //for option witch changeable settings - like "Sound" or "Difficulty"
    int max_value = 0;
    int option_value = 0;
};

//==================================================
//                  Menu
//==================================================

/**
 * @brief one item of a MenuBar - consists of menu name and its options (class Option)
 *
 */
class Menu
{
public:
    Menu(std::string _text, char _trigger);
    ~Menu() = default;
    void AddOption(const Option option);
    void AddCurrentlySelected(int i);

    //Getters
    std::string GetText() const{ return text; }
    int GetTrigger() const{ return trigger; }
    int GetSelected() const{ return currently_selected; }


    // gives Menu information on where to print itself - start_x is set by MenuBar class
    int start_x;
    std::vector<Option> option_vector;

private:
    std::string text;
    char trigger;
    int currently_selected = 0;
};


/**
 * @brief holds all menu objects and its settings, this class also has fucntions that prints its objects to the terminal
 *
 */
class MenuBar
{
public:
    MenuBar(WINDOW *_win);
    ~MenuBar() = default;
    MenuBar &add(Menu menu);
    void drawOptions(Menu& opt_menu) const;
    int draw(const int _trigger);
    OptionReturns menuLoop();

private:
    //menu window
    WINDOW *win;
    // win_option parameters
    int beg_y, beg_x, max_y, max_x;
    WINDOW *win_options;
    // menu items
    std::vector<std::shared_ptr<Menu>> menus;

    //from where I want to start priting menu names
    int current_position = 2;
    int selected_menu = 0;
};

// declaration of function create_menu() used by menu.cpp
MenuBar createMenu(Terminal& term);
OptionReturns menuHandler(const int chosen_option);
