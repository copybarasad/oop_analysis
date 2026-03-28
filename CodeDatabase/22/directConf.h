#pragma once
#include <vector>
#include <string>
#include "field/position.h"

struct directConf{
    std::vector<std::string> directNames = {"Огненный шар", "Малый разряд", "Большой разряд"};
    std::vector<std::string> directDescriptions = {"Поражает врага адским пламенем", "Поражает врага коротким зарядом", "Разряд, от которого жарится плоть"};
    std::vector<int> directDamage = {50, 30, 100};
    std::vector<int> directRadius = {5, 7, 4};

    Position trapPos = {0, 0};
};