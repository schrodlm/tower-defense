#ifndef TOWERHANDLER_H_
#define TOWERHANDLER_H_

#include <ncurses.h>
#include <vector>
#include <memory>
#include "entity.h"



std::vector<std::shared_ptr<class Tower>> towerLoading();
std::vector<std::shared_ptr<class Enemy>> enemyLoading();
#endif
