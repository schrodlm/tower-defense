#include <iostream>
#include <ncurses.h>
#include "towerhandler.h"
#include "entity.h"
#include <vector>
#include <memory>


    std::vector<std::shared_ptr<Tower>> towerLoading()
    {
        const std::string file_path = "assets/entities/entities.txt"; 
        std::vector<std::shared_ptr<Tower>> towers;

        //TOwers loading
        towers.emplace_back(new BasicTower(file_path));
        towers.emplace_back(new IceTower(file_path));
        towers.emplace_back(new FireTower(file_path));
        towers.emplace_back(new GreatTower(file_path));

        return towers;

    }

