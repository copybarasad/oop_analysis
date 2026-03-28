#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

struct Decoration {
    enum Type { RUG, RUNE, FLOOR_TILE, FLOWER };
    Type type;
    int x, y;
    sf::Color color;
};

struct LevelConfig {
    std::string name = "Unknown";
    std::string floorTexture = "floor_tex"; 
    
    int fieldWidth = 50;
    int fieldHeight = 35;
    int playerStartX = 5;
    int playerStartY = 5;
    
    int initialEnemyCount = 0;
    int enemyHealth = 100;
    int enemyDamage = 10;
    
    float playerHealthMultiplier = 1.0f;
    
    std::vector<int> allowedEnemyTypes;
    
    struct ObstaclePlacement {
        int x, y, type, size;
        sf::Color color;
    };
    struct TowerPlacement {
        int x, y;
    };

    std::vector<ObstaclePlacement> obstaclePlacements;
    std::vector<TowerPlacement> towerPlacements;
};
