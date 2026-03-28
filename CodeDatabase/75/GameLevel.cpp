#include "GameLevel.h"
#include <iostream>
#include "WorldObject.h" 
#include "Enemy.h" 
#include <set>

GameLevel::GameLevel(LevelType /*type*/, int levelNumber) {
    m_config = LevelConfig();
    setupLevel(levelNumber);
}

void GameLevel::setupLevel(int n) {
    float diff = 1.0f + (n * 0.25f);
    m_config.enemyHealth = static_cast<int>(80 * diff);
    m_config.enemyDamage = static_cast<int>(10 * diff);
    m_config.initialEnemyCount = 5 + (n / 2); 
    
    m_config.playerHealthMultiplier = 1.0f; 
    if (n >= 8) m_config.playerHealthMultiplier = 5.0f; // 5x HP on levels 8, 9, 10
    
    m_config.fieldWidth = 50;
    m_config.fieldHeight = 35;
    m_config.obstaclePlacements.clear();
    m_config.towerPlacements.clear();
    m_config.allowedEnemyTypes.clear();

    auto addType = [&](Enemy::Type t) { m_config.allowedEnemyTypes.push_back(static_cast<int>(t)); };
    
    int w = m_config.fieldWidth;
    int h = m_config.fieldHeight;
    sf::Color cWhite = sf::Color::White;

    if (n <= 3) {
        m_config.name = "FOREST OF WHISPERS";
        m_config.floorTexture = "floor_grass"; 
        m_config.playerStartX = 5; m_config.playerStartY = h/2;
        addType(Enemy::Type::MELEE); 
        if(n>1) addType(Enemy::Type::RANGED);
        
        for(int x=0; x<w; ++x) for(int y=0; y<h; ++y) {
            if(rand()%3==0) m_config.obstaclePlacements.push_back({x, y, (int)WorldObject::GRASS, 0, cWhite});
            if(rand()%15==0) m_config.obstaclePlacements.push_back({x, y, (int)WorldObject::FLOWER, 0, cWhite});
            if(rand()%25==0) m_config.obstaclePlacements.push_back({x, y, (int)WorldObject::MUSHROOM, 0, cWhite});
        }
        for(int i=0; i<60; ++i) {
            int cx = rand() % (w-2) + 1; int cy = rand() % (h-2) + 1;
            if (abs(cx - m_config.playerStartX) < 5) continue;
            if (rand()%5 == 0) m_config.obstaclePlacements.push_back({cx, cy, (int)WorldObject::STUMP, 0, cWhite});
            else m_config.obstaclePlacements.push_back({cx, cy, (int)WorldObject::TREE, 0, cWhite});
        }
    }
    else if (n <= 6) {
        m_config.name = "DEEP DUNGEON";
        m_config.floorTexture = "floor_stone"; 
        m_config.playerStartX = 3; m_config.playerStartY = 3;
        addType(Enemy::Type::FAST); addType(Enemy::Type::TANK);
        
        for(int x=10; x<40; x+=2) m_config.obstaclePlacements.push_back({x, 15, (int)WorldObject::WALL, 0, cWhite});
        for(int y=5; y<25; y+=2) m_config.obstaclePlacements.push_back({25, y, (int)WorldObject::WALL, 0, cWhite});
        for(int i=0; i<80; ++i) {
            int rx = rand()%(w-2)+1; int ry = rand()%(h-2)+1; int r = rand()%100;
            if (r < 50) m_config.obstaclePlacements.push_back({rx, ry, (int)WorldObject::ROCK, 0, cWhite});
            else if (r < 75) m_config.obstaclePlacements.push_back({rx, ry, (int)WorldObject::CRYSTAL, 0, cWhite});
            else m_config.obstaclePlacements.push_back({rx, ry, (int)WorldObject::CRATE, 0, cWhite});
        }
        m_config.towerPlacements.push_back({w/2, h/2});
    }
    else if (n <= 9) {
        m_config.name = "LAVA KEEP";
        m_config.floorTexture = "floor_stone";
        m_config.playerStartX = 5; m_config.playerStartY = h-5;
        addType(Enemy::Type::MAGE); addType(Enemy::Type::TANK); addType(Enemy::Type::RANGED);
        
        sf::Color lavaColor(220, 50, 0);
        for(int i=0; i<5; ++i) {
            int cx = rand()%(w-5) + 2; int cy = rand()%(h-5) + 2;
            for(int j=0; j<25; ++j) {
                m_config.obstaclePlacements.push_back({cx, cy, (int)WorldObject::WATER, 0, lavaColor});
                cx += (rand()%3 - 1); cy += (rand()%3 - 1);
            }
        }
        m_config.towerPlacements.push_back({15, 15}); m_config.towerPlacements.push_back({35, 20});
    }
    else if (n == 10) {
        m_config.name = "DEMON THRONE";
        m_config.floorTexture = "floor_stone";
        m_config.playerStartX = w/2; m_config.playerStartY = h-5;
        m_config.initialEnemyCount = 1;
        addType(Enemy::Type::BOSS);
        
        m_config.obstaclePlacements.push_back({10, 10, (int)WorldObject::PILLAR, 0, cWhite});
        m_config.obstaclePlacements.push_back({w-10, 10, (int)WorldObject::PILLAR, 0, cWhite});
        m_config.obstaclePlacements.push_back({10, h-10, (int)WorldObject::PILLAR, 0, cWhite});
        m_config.obstaclePlacements.push_back({w-10, h-10, (int)WorldObject::PILLAR, 0, cWhite});
    }
}