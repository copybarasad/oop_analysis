#include "WallGenerator.h"
#include <cstdlib>
#include <ctime>
#include <vector>

void WallGenerator::generateRandomWalls(GameField& field, double wallDensity) {
    int width = field.getWidth();
    int height = field.getHeight();
    
    std::srand(std::time(nullptr));
    
    std::vector<Position> protectedPositions = {
        Position(0, 0),
        Position(0, 1),
        Position(0, 2),
        Position(1, 0),
        Position(1, 1),
        Position(1, 2),
        Position(2, 0),
        Position(2, 1),
        Position(2, 2),
        Position(width - 8, height - 8),
        Position(width - 3, height - 7),
        Position(width - 7, height - 3),
        Position(width - 3, height - 5),
        Position(width - 5, height - 3),
    };
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Position currentPos(x, y);
            
            bool isProtected = false;
            for (const auto& protectedPos : protectedPositions) {
                if (currentPos == protectedPos) {
                    isProtected = true;
                    break;
                }
            }
            
            if (!isProtected && shouldGenerateWall(wallDensity)) {
                field.setCellType(currentPos, CellType::WALL);
            }
        }
    }
}

bool WallGenerator::shouldGenerateWall(double wallDensity) {
    return (std::rand() % 100) < (wallDensity * 100);
}