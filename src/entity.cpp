#include <iostream>
#include <fstream>
#include <iostream>
#include <ncurses.h>
#include <sstream>
#include <string>
#include <algorithm>
#include <map>
#include <regex>
#include "entity.h"

/**
 * @brief Construct a new Tower:: Tower object from a file
 *
 * @param file_path     --> path to a tower attributes file
 * @param tower_name    --> name of the child class that uses this constructor
 */
Tower::Tower(const std::string &file_path, const std::string tower_name)
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
    //===========================================
    std::string str = buffer.str();
    //the paragraph of specified tower
    std::smatch matched_file_part;  
    std::regex reg_file_part(tower_name);
    regex_search(str, matched_file_part,reg_file_part);
    std::string file_part = matched_file_part.str(1);
    
    //EXTRACTING THE ATTRIBUTES FOR SPECIFIED TOWER
    //==========================================    
    
    //regex for a specified tower
    std::regex reg_tower("\n([A-Za-z_]+) : '([^']*)'");
    //map of names and values of attributes
    std::map<std::string, std::string> tower_attributes;
    std::smatch matches;
    
    //PASSING THESE ATTRIBUTES INTO A MAP
    //=========================================
    while (regex_search(file_part, matches, reg_tower))
    {
        tower_attributes.insert(make_pair(matches.str(1), matches.str(2)));
        file_part = matches.suffix().str();
    }

    //DISTRIBUTTING THESE ATTRIBUTES FROM THE MAP TO VARIABLES
    //=========================================
    damage = atoi(tower_attributes["Damage"].c_str());
    seeing_distance = atoi(tower_attributes["Seeing_Distance"].c_str());
}
