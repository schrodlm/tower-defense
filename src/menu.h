#include <ncurses.h>
#include <string>
#include <vector>
#include <memory> //unique_ptr

#ifndef _MENU_H_
#define _MENU_H_

/**
 * @brief 
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

/**
 * @brief This class defines options for each menu
 *          -- for example menu Game has options "New Game" and "Load Game"
 *
 */
class Option
{
public:
    Option(std::string _name, OptionReturns _return_value)
    {
        option_name = _name;
        value_change = false;
        return_value = _return_value;
    }

    // overloaded contructor add max possible value to a setting
    Option(std::string _name, int _max_value, OptionReturns _return_value)
    {
        option_name = _name;
        max_value = _max_value;
        value_change = true;
        return_value = _return_value;
    }
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

/**
 * @brief one item of a MenuBar - consists of menu name and its options (class Option)
 *
 */
class Menu
{
public:
    Menu(std::string _text, char _trigger)
    {
        text = _text;
        trigger = _trigger;
    }

    /**
     * @brief adds option to a vector of options
     *
     * @param option - consist of name a optional int value
     */
    void AddOption(const Option option)
    {
        option_vector.push_back(option);
    };

    /**
     * @brief Getters
     *
     */
    std::string GetText() const{ return text; }
    int GetTrigger() const{ return trigger; }
    int GetSelected() const{ return currently_selected; }

    /**
     * @brief 
     * 
     * @param i 
     */
    void AddCurrentlySelected(int i)
    {
        currently_selected += i;
        if (currently_selected > (int)option_vector.size() - 1)
        {
            currently_selected = 0;
        }
        if (currently_selected < 0)
        {
            currently_selected = (int)option_vector.size() - 1;
        }
    }

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
    MenuBar(WINDOW *_win)
    {
        win = _win;
        keypad(win, TRUE);
        // getting x,y values of outside menu to create a smaller one for options
        getbegyx(win, beg_y, beg_x);
        getmaxyx(win, max_y, max_x);
        // window for options
        win_options = newwin(max_y - 2, max_x - 2, beg_y + 1, beg_x + 1);
    }

    /**
     * @brief add Menu item to itself, stores menu items in unique pointers
     *
     * @param menu          --> one item of class Menu
     * @return MenuBar&     --> returns *this
     */
    MenuBar &add(Menu menu)
    {
        menu.start_x = current_position;
        current_position += menu.GetText().length() + 1;
        menus.push_back(std::make_shared<Menu>(menu));
        return *this;
    }

    /**
     * @brief draws options for each menu item, everytime a designed trigger key is pressed
     *
     * @param opt_menu  --> menu item of class Menu
     */
    void drawOptions(Menu& opt_menu) const
    {
        // clears a specific window
        wclear(win_options);
        box(win_options, 0, 0);

        // draws each item of that menu item
        for (int i = 0; i < (int)opt_menu.option_vector.size(); i++)
        {
            // if option is selected right now - highlight it
            if (i == opt_menu.GetSelected())
                wattron(win_options, A_STANDOUT);
            mvwprintw(win_options, i + 1, 1, opt_menu.option_vector[i].getOptionName().c_str());
            wattroff(win_options, A_STANDOUT);

            // if option has an integer value show it
            if (opt_menu.option_vector[i].getChangeValue() == true)
            {
                std::string s = std::to_string(opt_menu.option_vector[i].getOptionValue());
                s = "< " + s + " >";
                mvwprintw(win_options, i + 1, max_x - 20, s.c_str());
            }
        }

        wrefresh(win_options);
    }

    /**
     * @brief draws the menu window on the terminal and also handles pressed keys
     *
     */
    int draw(const int _trigger) 
    {
        int selected_option;

        for (int i = 0; i < (int)menus.size(); i++)
        {
            int start_x = menus[i]->start_x;
            if (menus[i]->GetTrigger() == _trigger)
            {
                drawOptions(*menus[i]);
                selected_menu = i;
                wattron(win, A_STANDOUT);
            }
            mvwprintw(win, 0, start_x, menus[i]->GetText().c_str());
            wattroff(win, A_STANDOUT);
        }
        switch (_trigger)
        {
        //enter pressed
        case ('\n'):
            selected_option = menus[selected_menu]->GetSelected();
            return menus[selected_menu]->option_vector[selected_option].getOptionReturn() ;
            break;

        case (KEY_DOWN):

            menus[selected_menu]->AddCurrentlySelected(1);
            drawOptions(*menus[selected_menu]);
            wattron(win, A_STANDOUT);
            mvwprintw(win, 0, menus[selected_menu]->start_x, menus[selected_menu]->GetText().c_str());
            wattroff(win, A_STANDOUT);
            break;

        case (KEY_UP):

            menus[selected_menu]->AddCurrentlySelected(-1);
            drawOptions(*menus[selected_menu]);
            wattron(win, A_STANDOUT);
            mvwprintw(win, 0, menus[selected_menu]->start_x, menus[selected_menu]->GetText().c_str());
            wattroff(win, A_STANDOUT);
            break;

        case (KEY_LEFT):
            selected_menu -= 1;
            if (0 > selected_menu)
                selected_menu = menus.size() - 1;
            drawOptions(*menus[selected_menu]);
            wattron(win, A_STANDOUT);
            mvwprintw(win, 0, menus[selected_menu]->start_x, menus[selected_menu]->GetText().c_str());
            wattroff(win, A_STANDOUT);
            break;

        case (KEY_RIGHT):
            selected_menu += 1;
            selected_menu %= menus.size();
            drawOptions(*menus[selected_menu]);
            wattroff(win, A_STANDOUT);
            
            wattron(win, A_STANDOUT);
            mvwprintw(win, 0, menus[selected_menu]->start_x, menus[selected_menu]->GetText().c_str());
            wattroff(win, A_STANDOUT);
        }
        return 0;
    }

private:
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
MenuBar create_menu(WINDOW* win);
OptionReturns menu_handler(const int chosen_option);

#endif