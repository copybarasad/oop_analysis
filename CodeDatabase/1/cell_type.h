#pragma once
#include <string>

enum class CellType {
    EMPTY,          
    WALL,                 
    CHAPEL,         
    VAMPIRE_LAIR,   
    ARMORY,  
    DRACULA_TOMB        
};

bool isPassable(CellType cellType);
bool hasSpecialEffect(CellType cellType);
std::string cellTypeToString(CellType cellType);
