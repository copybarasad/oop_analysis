#include "SaveManager.h"
#include "Player.h"
#include "Field.h"
#include "Enemy.h"
#include "EnemySpawner.h"
#include "EnemyTower.h"
#include <fstream>

bool SaveManager::save(const std::string& filename, 
    int level,
    const Player& player, 
    const Field& field,
    const std::vector<Enemy>& enemies,
    const std::vector<EnemySpawner>& spawners,
    const std::vector<EnemyTower>& towers,
    EventLogger& logger) 
{
    std::ofstream out(filename);
    if (!out.is_open()) {
        logger.log(LogMsg::SAVE_FAIL);
        return false;
    }

    out << level << "\n";
    try {
        player.save(out);
    } catch (...) {
        logger.log(LogMsg::SAVE_FAIL);
        return false;
    }
    if (!field.save(out, logger)) {
        return false; 
    }
    
    out << enemies.size() << "\n";
    for (const auto& e : enemies) {
        out << e.getX() << " " << e.getY() << " " << e.getHealth() << " " << e.getDamage() << "\n"; 
    }
    
    out << spawners.size() << "\n";
    for (const auto& s : spawners) {
        out << s.getX() << " " << s.getY() << " " << s.getHealth() << "\n";
    }
    
    out << towers.size() << "\n";
    for (const auto& t : towers) {
        out << t.getX() << " " << t.getY() << " " << t.getHealth() << "\n";
    }

    out.close();
    logger.log(LogMsg::SAVE_SUCCESS);
    return true;
}

bool SaveManager::load(const std::string& filename, 
    int& level,
    Player& player, 
    Field& field,
    std::vector<Enemy>& enemies,
    std::vector<EnemySpawner>& spawners,
    std::vector<EnemyTower>& towers,
    EventLogger& logger) 
{
    std::ifstream in(filename);
    if (!in.is_open()) {
        logger.log(LogMsg::LOAD_FAIL);
        return false;
    }
    
    std::vector<Enemy> tempEnemies;
    std::vector<EnemySpawner> tempSpawners;
    std::vector<EnemyTower> tempTowers;
    auto check = [&](bool condition) -> bool {
        if (!condition) {
            logger.log(LogMsg::LOAD_FAIL);
            return false;
        }
        return true;
    };
    if (!check(bool(in >> level))) return false;
    
    try {
        player.load(in);
    } catch (...) {
        logger.log(LogMsg::LOAD_FAIL);
        return false;
    }

    if (!field.load(in, logger)) {
        return false;
    }
    
    size_t count;

    if (!check(bool(in >> count))) return false;
    for (size_t i = 0; i < count; ++i) {
        int ex, ey, eh, ed;
        if (!check(bool(in >> ex >> ey >> eh >> ed))) return false;
        
        tempEnemies.emplace_back(ex, ey, eh, ed);
        if (field.isValidPosition(ex, ey)) {
            field.setCellType(ex, ey, CellType::ENEMY);
        }
    }
    
    if (!check(bool(in >> count))) return false;
    for (size_t i = 0; i < count; ++i) {
        int sx, sy, sh;
        if (!check(bool(in >> sx >> sy >> sh))) return false;
        
        tempSpawners.emplace_back(sx, sy, 5, sh); 
        if (field.isValidPosition(sx, sy)) {
            field.setCellType(sx, sy, CellType::SPAWNER);
        }
    }
    
    if (!check(bool(in >> count))) return false;
    for (size_t i = 0; i < count; ++i) {
        int tx, ty, th;
        if (!check(bool(in >> tx >> ty >> th))) return false;
        
        tempTowers.emplace_back(tx, ty, th);
        if (field.isValidPosition(tx, ty)) {
            field.setCellType(tx, ty, CellType::ENEMY_TOWER);
        }
    }
    enemies = std::move(tempEnemies);
    spawners = std::move(tempSpawners);
    towers = std::move(tempTowers);
    
    logger.log(LogMsg::LOAD_SUCCESS);
    return true;
}