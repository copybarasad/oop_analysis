#include "map_generator.h"

#include <iostream>
#include <stdexcept>

namespace game {
    // -------------- Generate random walls ---------------
    // Walls: line or corner (like Г)
    void MapGenerator::generateWalls(const GameConfig& cfg, std::mt19937& rng)
    {
        int wallMaxCount = map_.width * map_.height * cfg.wallMaxDensity;

        std::uniform_int_distribution<int> xDist(1, map_.width  - 1);
        std::uniform_int_distribution<int> yDist(1, map_.height - 1);

        std::uniform_int_distribution<int> typeDist(0, 99);
        std::uniform_int_distribution<int> lenDist(1, wallMaxCount/4);
        std::uniform_int_distribution<int> boolDist(0, 1);

        int wallCount = 0;
        while (wallCount < wallMaxCount) {
            int x = xDist(rng);
            int y = yDist(rng);

            if (Pos(x, y) == cfg.playerInitialPosition) continue;

            int len = std::min(lenDist(rng), wallMaxCount - wallCount);
            
            
            // Line-wall
            if (typeDist(rng) >= cfg.wallCornerChance) {
                bool horizontal = boolDist(rng);

                for (int j = 0; j < len; j++) {
                    int nx = x + (horizontal ? j : 0);
                    int ny = y + (horizontal ? 0 : j);

                    if (inBounds(Pos(nx, ny), 1) &&
                        Pos(nx, ny) != cfg.playerInitialPosition)
                    {   
                        if (!map_.cells[ny][nx].isWalkable()) break;

                        map_.cells[ny][nx].setWall();
                        wallCount++;
                    }
                }
            } else {    // Corner-wall
                int lenH = lenDist(rng);
                int lenV = lenDist(rng);

                for (int j = 0; j < lenH; j++) {
                    int nx = x + j;

                    if (inBounds(Pos(nx, y), 1) &&
                        Pos(nx, y) != cfg.playerInitialPosition)
                    {
                        if (!inBounds(Pos(x, y), 1) && !map_.cells[y][nx].isWalkable()) break;     // Breakes double-walls (+)

                        map_.cells[y][nx].setWall();
                        wallCount++;
                    }
                }
                for (int j = 0; j < lenV; j++) {
                    int ny = y + j;

                    if (inBounds(Pos(x, ny), 1) &&
                        Pos(x, ny) != cfg.playerInitialPosition)
                    {   
                        if (!inBounds(Pos(x, y), 1) && !map_.cells[ny][x].isWalkable()) break;
                        
                        map_.cells[ny][x].setWall();
                        wallCount++;
                    }
                }
            }
        }
    }

    // ----------- Generate random slime cells ------------
    void MapGenerator::generateSlimes(const GameConfig& cfg, std::mt19937& rng)
    {
        int slimeMaxCount = map_.width * map_.height * cfg.slimeMaxDensity;

        std::uniform_int_distribution<int> xDist(1, map_.width  - 1);
        std::uniform_int_distribution<int> yDist(1, map_.height - 1);

        int slimeCount = 0;
        while (slimeCount < slimeMaxCount) {
            int x = xDist(rng);
            int y = yDist(rng);

            if (map_.cells[y][x].isWalkable() &&
                Pos(x, y) != cfg.eBuildingInitialPosition &&
                Pos(x, y) != cfg.playerInitialPosition)
            {
                map_.cells[y][x].setSlime();

                slimeCount++;
            }
        }
    }

    // --------------- Spawn enemy building ---------------
    // --------------- at initial position ----------------
    void MapGenerator::placeEnemyBuilding(const GameConfig& cfg)
    {
        map_.enemyBuilding = cfg.eBuildingInitialPosition;

        Cell& cell = map_.cells[map_.enemyBuilding.y][map_.enemyBuilding.x];

        cell.setWall();
        cell.placeEBuilding();
    }

    void MapGenerator::placeEnemyTower(const GameConfig& cfg)
    {
        map_.enemyTower = cfg.eTowerInitialPosition;

        Cell& cell = map_.cells[map_.enemyTower.y][map_.enemyTower.x];

        cell.setWall();
        cell.placeETower();
    }

    // ---------------- Verify position -----------------
    bool MapGenerator::inBounds(Pos p, int shift) const
    {
        return (p.x >= shift && p.x < map_.width - shift &&
                p.y >= shift && p.y < map_.height - shift);
    }

    // --------- Public: generate all map (cells) ---------
    MapData MapGenerator::generateMap(int w, int h, const GameConfig& cfg, std::mt19937& rng)
    {
        if (w < cfg.minSize || w > cfg.maxSize ||
            h < cfg.minSize || h > cfg.maxSize)
        {
            throw std::invalid_argument("Field size must be in ["
                + std::to_string(cfg.minSize) + "; "
                + std::to_string(cfg.maxSize) + "]");
        }
        
        map_ = MapData();
        map_.width = w;
        map_.height = h;

        map_.cells.assign(map_.height, std::vector<Cell>(map_.width));  // default - floor

        map_.playerStart = cfg.playerInitialPosition;

        generateWalls(cfg, rng);
        generateSlimes(cfg, rng);

        placeEnemyBuilding(cfg);
        placeEnemyTower(cfg);

        return map_;
    }

    #ifdef TESTING
    MapData MapGenerator::generateWallTestMap(int w, int h, const GameConfig& cfg, std::mt19937& rng)
    {
        if (w < cfg.minSize || w > cfg.maxSize ||
            h < cfg.minSize || h > cfg.maxSize)
        {
            throw std::invalid_argument("Field size must be in ["
                + std::to_string(cfg.minSize) + "; "
                + std::to_string(cfg.maxSize) + "]");
        }

        map_ = generateMap(w, h, cfg, rng);
        map_.cells[0][1].setWall();
        map_.cells[1][0].setWall();

        map_.playerStart = Pos();
        map_.enemyBuilding = Pos(w-1, h-1);

        return map_;
    }

    MapData MapGenerator::generateSlimeTestMap(int w, int h, const GameConfig& cfg, std::mt19937& rng)
    {
        if (w < cfg.minSize || w > cfg.maxSize ||
            h < cfg.minSize || h > cfg.maxSize)
        {
            throw std::invalid_argument("Field size must be in ["
                + std::to_string(cfg.minSize) + "; "
                + std::to_string(cfg.maxSize) + "]");
        }

        map_ = generateMap(w, h, cfg, rng);
        map_.cells[0][1].setSlime();
        map_.cells[1][0].setSlime();

        map_.playerStart = Pos();
        map_.enemyBuilding = Pos(w-1, h-1);

        return map_;
    }
    #endif
}