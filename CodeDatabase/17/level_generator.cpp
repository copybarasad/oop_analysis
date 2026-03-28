#include "level_generator.h"

bool LevelGenerator::flag = true;

void LevelGenerator::generateLevel(GameMap& map, EntityManager& entities,
                                  int width, int height, int level, int enemyCount, int buildingCount, int EnemyTowerCount) {
    
    int maxLow = (width * height) / LOWPROCENT;
    int lowCount = rand() % maxLow;
    generateRandom(map, lowCount, MapCell::Type::LOW);
    
    Position playerPos = findRandomEmptyPosition(map);
    entities.createPlayer(playerPos.getX(), playerPos.getY(), 2000);
    MapCell& cell = map.getCell(playerPos);
    cell.setEntity(&entities.getPlayer()); 
    cell.setUsed(true);
    
    for (int i = 0; i < enemyCount; i++) {
        spawnEntity<Enemy>(map, entities.getEnemies(), level);
    }

    for (int i = 0; i < buildingCount; i++) {
        Position buildingPos = findRandomEmptyPosition(map);
        MapCell& cell = map.getCell(buildingPos);
        cell.setUsed(true);
        entities.getBuildings().push_back(new EnemyBuilding(buildingPos.getX(), buildingPos.getY(), level));
    }

    for (int i = 0; i < EnemyTowerCount; i++) {
        spawnEntity<EnemyTower>(map, entities.getTowers(), level);
    }

    int maxWalls = (width * height) / WALLPROCENT;
    int wallCount = rand() % maxWalls;
    generateWalls(map, wallCount);
}

template<typename T>
void LevelGenerator::spawnEntity(GameMap& map, std::vector<T*>& entityList, int level) {
    Position pos = findRandomEmptyPosition(map);
    MapCell& cell = map.getCell(pos);
    T* entity = new T(pos.getX(), pos.getY(), level);
    cell.setEntity(entity);
    cell.setUsed(true);
    entityList.push_back(entity);
}

void LevelGenerator::generateWalls(GameMap& map, int wallCount) {
    int placed = 0;
    int attempts = 0;
    int maxAttempts = wallCount; 
    
    while (placed < wallCount && attempts < maxAttempts) {
        attempts++;
        Position pos = findRandomEmptyPosition(map);
        
        if (canPlaceWall(map, pos)) {      
            map.getCell(pos).setType(MapCell::Type::WALL);
            placed++;
        }
    }
}

void LevelGenerator::generateRandom(GameMap& map, int count, MapCell::Type cellType) {
    int placed = 0;
    int width = map.getWidth();
    int height = map.getHeight();
    
    while (placed < count) {
        int x = rand() % width;
        int y = rand() % height;
        Position pos(x, y);
        
        if (map.getCell(pos).getType() == MapCell::Type::EMPTY) {
            map.getCell(pos).setType(cellType);
            placed++;
        }
    }
}

Position LevelGenerator::findRandomEmptyPosition(const GameMap& map) {
    while (flag) {
        int x = rand() % map.getWidth();
        int y = rand() % map.getHeight();
        Position pos(x, y);
        
        if (isPositionValidForSpawn(map, pos)) {
            return pos;
        }
    }
    return Position(0, 0);
}

bool LevelGenerator::isPositionValidForSpawn(const GameMap& map, const Position& pos) {
    if (!map.isPositionValid(pos) || map.getCell(pos).getType() != MapCell::Type::EMPTY || map.getCell(pos).isUsed()){
        return false;
    }
    return true;
}

bool LevelGenerator::canPlaceWall(const GameMap& map, const Position& pos) {
    int freeCount = 0;
    int totalNeighbors = 0;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue; 
            
            Position neighbor(pos.getX() + dx, pos.getY() + dy);
            
            if (map.isPositionValid(neighbor)) {
                totalNeighbors++;
                const MapCell& cell = map.getCell(neighbor);
                
                if (cell.getType() == MapCell::Type::EMPTY && !cell.isUsed()) {
                    freeCount++;
                }
            }
        }
    }
    return (freeCount >= 7);
}