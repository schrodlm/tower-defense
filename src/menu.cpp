#include <iostream>
#include <ncurses.h>
#include "menu.h"
#include "terminal.h"

//==================================================
//                  Option
//==================================================

/**
 * @brief Construct a new Option:: Option object
 *
 * @param _name
 * @param _return_value
 */

Option::Option(std::string _name, OptionReturns _return_value)
{
    option_name = _name;
    value_change = false;
    return_value = _return_value;
}

// overloaded contructor add max possible value to a setting
Option::Option(std::string _name, int _max_value, OptionReturns _return_value)
{
    option_name = _name;
    max_value = _max_value;
    value_change = true;
    return_value = _return_value;
}

//==================================================
//                  Menu
//==================================================

/**
 * @brief Construct a new Menu object
 *
 * @param _text
 * @param _trigger
 */
Menu::Menu(std::string _text, char _trigger)
{
    text = _text;
    trigger = _trigger;
}

/**
 * @brief
 *
 * @param i
 */
void Menu::AddCurrentlySelected(int i)
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
/**
 * @brief adds option to a vector of options
 *
 * @param option - consist of name a optional int value
 */
void Menu::AddOption(const Option option)
{
    option_vector.push_back(option);
};

//==================================================
//                  MenuBar
//==================================================

/**
 * @brief Construct a new Menu Bar object
 *
 * @param _win
 */
MenuBar::MenuBar(WINDOW *_win)
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
 * @brief add Menu item to MenuBar, stores menu items in unique pointers
 *
 * @param menu          --> one item of class Menu
 * @return MenuBar&     --> returns *this
 */
MenuBar &MenuBar::add(Menu menu)
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
void MenuBar::drawOptions(Menu &opt_menu) const
{
    // clears a specific window
    wclear(win_options);
    box(win_options, 0, 0);

    // draws each item of that menu
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
int MenuBar::draw(const int _trigger)
{
    int selected_option;

    // draws the menu window
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

    // enter was pressed
    case ('\n'):
        selected_option = menus[selected_menu]->GetSelected();
        return menus[selected_menu]->option_vector[selected_option].getOptionReturn();
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
        break;
    
    default: 
        drawOptions(*menus[selected_menu]);
        wattron(win, A_STANDOUT);
        mvwprintw(win, 0, menus[selected_menu]->start_x, menus[selected_menu]->GetText().c_str());
        wattroff(win, A_STANDOUT);
    }
    return 0;
}

//==================================================
//                  GenericFunctions
//==================================================

/**
 * @brief Create a MenuBar object
 *
 * @param win       --> window to draw menu on
 * @return MenuBar  --> menu that will be printed on stdscr
 */
MenuBar createMenu(Terminal &term)
{

    // creates a window for menu
    WINDOW *win = newwin(term.max_y / 2, term.max_x / 2, term.max_y / 4, term.max_x / 4);

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

    // draws game menu
    wrefresh(win);
    menu.draw('g');
    return menu;
}

OptionReturns MenuBar::menuLoop()
{
    // moving in the menu
    int pressed_key;
    // chosen option from the menu
    OptionReturns chosen = NO_OPTION_CHOSEN;
    while (chosen == NO_OPTION_CHOSEN)
    {
        pressed_key = wgetch(win);

        int chosen_option = draw(pressed_key);
        chosen = menuHandler(chosen_option);
    }
    // clear window menu
    clearWindow(win);
    delwin(win);
    clear();

    return chosen;
}

OptionReturns menuHandler(const int chosen_option)
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
    {
     int option = 0;
    while (1)
    {
        int pressed_key = wgetch(stdscr);
       
        switch(pressed_key)
        {
            case(KEY_LEFT):
                option--;
                break;
            case(KEY_RIGHT):
                option++;
                break;
            case('\n'):
                std::string str = std::to_string(option);
                printw(str.c_str());
                refresh();
                getch();
                return DIFFICULTY_OPTION;
                
        }
    }
        return DIFFICULTY_OPTION;
        break;
    }
    case AUTHOR_OPTIONS:
        infoWindow("Author", "Quisque tincidunt scelerisque libero. Sed vel lectus. Donec odio tempus molestie, porttitor ut, iaculis quis, sem. Maecenas sollicitudin. Nulla quis diam. Phasellus enim erat, vestibulum vel, aliquam a, posuere eu, velit. Duis sapien nunc, commodo et, interdum suscipit, sollicitudin et, dolor.");
        return AUTHOR_OPTIONS;
        break;

    case GAME_OPTION:
        infoWindow("Game",
                   "Fusce tellus. Phasellus enim erat, vestibulum vel, aliquam a, posuere eu, velit. Mauris dolor felis, sagittis at, luctus sed, aliquam non, tellus. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Ut enim ad minima veniam, quis nostrum exercitationem ullam corporis suscipit laboriosam, nisi ut aliquid ex ea commodi consequatur? Neque porro quisquam est, qui dolorem ipsum quia dolor sit amet, consectetur, adipisci velit, sed quia non numquam eius modi tempora incidunt ut labore et dolore magnam aliquam quaerat voluptatem. Etiam sapien elit, consequat eget, tristique non, venenatis quis, ante.");
        return GAME_OPTION;
        break;

    case LEVELS_OPTION:
        infoWindow("Levels",
                   "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Sed convallis magna eu sem. Proin in tellus sit amet nibh dignissim sagittis. Fusce aliquam vestibulum ipsum. Maecenas lorem. Integer imperdiet lectus quis justo. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos hymenaeos. Integer vulputate sem a nibh rutrum consequat. Vivamus porttitor turpis ac leo. Sed ut perspiciatis unde omnis iste natus error sit voluptatem accusantium doloremque laudantium, totam rem aperiam, eaque ipsa quae ab illo inventore veritatis et quasi architecto beatae vitae dicta sunt explicabo. In rutrum. Etiam commodo dui eget wisi. Fusce nibh. Etiam quis quam. Integer tempor. Praesent id justo in neque elementum ultrices. Morbi scelerisque luctus velit. ");
        return LEVELS_OPTION;
        break;
    }
    return NO_OPTION_CHOSEN;
}
