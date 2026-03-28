#ifndef WALLGENERATOR_H
#define WALLGENERATOR_H

#include "GameField.h"

class WallGenerator {
public:
    static void generateRandomWalls(GameField& field, double wallDensity);
    
    static bool shouldGenerateWall(double wallDensity);
};

#endif