#include "SaveLoadManager.h"
#include "GameState.h"
#include "Field.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBase.h"
#include "EnemyTower.h"
#include "Ally.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "BuffSpell.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <vector>
#include <tuple>
#include <utility>

class FileHandler {
private:
    std::fstream file_;
    
public:
    FileHandler(const std::string& filename, std::ios_base::openmode mode) {
        file_.open(filename, mode);
        if (!file_.is_open()) {
            if (mode & std::ios::in) {
                throw FileNotFoundException();
            } else {
                throw WritePermissionException();
            }
        }
    }
    
    ~FileHandler() {
        if (file_.is_open()) {
            file_.close();
        }
    }
    
    std::fstream& get() { return file_; }
};


std::string SaveLoadManager::serializeEnemy(std::shared_ptr<Enemy> enemy) {
    if (!enemy || !enemy->isAlive()) return "";
    return std::to_string(enemy->getHp()) + "," + 
           std::to_string(enemy->getDamage());
}

std::string SaveLoadManager::serializeEnemyBase(std::shared_ptr<EnemyBase> base) {
    if (!base || !base->isAlive()) return "";
    return std::to_string(base->getHp()) + "," +
           std::to_string(base->getMaxHp()) + "," +
           std::to_string(base->getSpawnInterval()) + "," +
           std::to_string(base->getTurnsUntilSpawn());
}

std::string SaveLoadManager::serializeEnemyTower(std::shared_ptr<EnemyTower> tower) {
    if (!tower || !tower->isAlive()) return "";
    return std::to_string(tower->getHp()) + "," +
           std::to_string(tower->getMaxHp()) + "," +
           std::to_string(tower->getDamage()) + "," +
           std::to_string(tower->getCurrentCooldown()) + "," +
           std::to_string(tower->getAttackRadius());
}

std::string SaveLoadManager::serializeAlly(std::shared_ptr<Ally> ally) {
    if (!ally || !ally->isAlive()) return "";
    return std::to_string(ally->getHp()) + "," +
           std::to_string(ally->getDamage());
}

std::string SaveLoadManager::serializeCell(int x, int y, const Cell& cell) {
    std::stringstream ss;
    ss << "CELL:" << x << "," << y << ",";
    
    switch (cell.getType()) {
        case CellType::WALL: ss << "WALL"; break;
        case CellType::SLOW: ss << "SLOW"; break;
        default: ss << "EMPTY"; break;
    }
    
    if (cell.hasTrap()) {
        ss << ",TRAP";
    }
    
    if (auto enemy = cell.getEnemy()) {
        if (enemy->isAlive()) {
            auto data = serializeEnemy(enemy);
            if (!data.empty()) {
                ss << ",ENEMY:" << data;
            }
        }
    }
    
    if (auto base = cell.getBase()) {
        if (base->isAlive()) {
            auto data = serializeEnemyBase(base);
            if (!data.empty()) {
                ss << ",BASE:" << data;
            }
        }
    }
    
    if (auto tower = cell.getTower()) {
        if (tower->isAlive()) {
            auto data = serializeEnemyTower(tower);
            if (!data.empty()) {
                ss << ",TOWER:" << data;
            }
        }
    }
    
    if (auto ally = cell.getAlly()) {
        if (ally->isAlive()) {
            auto data = serializeAlly(ally);
            if (!data.empty()) {
                ss << ",ALLY:" << data;
            }
        }
    }
    
    if (cell.getPlayer()) {
        ss << ",PLAYER";
    }
    
    return ss.str();
}

std::string SaveLoadManager::serializeGameState(const GameState& state) {
    std::stringstream ss;
    ss << "GAME_STATE v2.0\n";
    ss << "LEVEL:" << state.getLevel() << "\n";
    ss << "TURN:" << state.getTurnCount() << "\n";
    ss << "SCORE:" << state.getScore() << "\n";
    
    auto player = state.getPlayer();
    if (player) {
        ss << "PLAYER_HP:" << player->getHp() << "\n";
        ss << "PLAYER_MAX_HP:" << player->getMaxHp() << "\n";
        ss << "PLAYER_DAMAGE:" << player->getDamage() << "\n";
        ss << "PLAYER_POINTS:" << player->getPoints() << "\n";
        ss << "PLAYER_ATTACK_MODE:" << (player->getAttackMode() == AttackMode::MELEE ? 0 : 1) << "\n";
        ss << "PLAYER_SLOWED:" << (player->isSlowed() ? 1 : 0) << "\n";
        ss << "PLAYER_RANGED_RANGE:" << player->getRangedRange() << "\n";
        ss << "PLAYER_SLOW_RESISTANCE:" << player->getSlowResistance() << "\n";
        
        auto spells = player->getHand().getAvailableSpells();
        if (!spells.empty()) {
            ss << "SPELL_COUNT:" << spells.size() << "\n";
            for (size_t i = 0; i < spells.size(); ++i) {
                ss << "SPELL_" << i << ":" << spells[i]->getName() << "\n";
            }
        }
    }
    
    auto field = state.getField();
    if (field) {
        ss << "FIELD_SIZE:" << field->getWidth() << "x" << field->getHeight() << "\n";
        
        for (int y = 0; y < field->getHeight(); ++y) {
            for (int x = 0; x < field->getWidth(); ++x) {
                const auto& cell = field->getCell(x, y);
                std::string cellData = serializeCell(x, y, cell);
                if (!cellData.empty()) {
                    ss << cellData << "\n";
                }
            }
        }
    }
    
    ss << "END_GAME_STATE";
    return ss.str();
}


std::shared_ptr<Enemy> SaveLoadManager::deserializeEnemy(const std::string& data) {
    auto [hp, damage] = Enemy::parseData(data);
    auto enemy = std::make_shared<Enemy>(hp, damage);
    enemy->setHp(hp);
    return enemy;
}

std::shared_ptr<EnemyBase> SaveLoadManager::deserializeEnemyBase(const std::string& data) {
    auto [hp, maxHp, spawnInterval, turnsUntilSpawn] = EnemyBase::parseData(data);
    auto base = std::make_shared<EnemyBase>(spawnInterval, maxHp);
    base->setHp(hp);
    base->setTurnsUntilSpawn(turnsUntilSpawn);
    return base;
}

std::shared_ptr<EnemyTower> SaveLoadManager::deserializeEnemyTower(const std::string& data) {
    auto [hp, maxHp, damage, currentCooldown, attackRadius] = EnemyTower::parseData(data);
    auto tower = std::make_shared<EnemyTower>(attackRadius, damage, 2, maxHp);
    tower->setHp(hp);
    tower->setCurrentCooldown(currentCooldown);
    return tower;
}

std::shared_ptr<Ally> SaveLoadManager::deserializeAlly(const std::string& data) {
    auto [hp, damage] = Ally::parseData(data);
    auto ally = std::make_shared<Ally>(hp, damage);
    ally->setHp(hp);
    return ally;
}

void SaveLoadManager::deserializeCell(const std::string& line, Field& field) {
    if (line.find("CELL:") != 0) return;
    
    std::string data = line.substr(5);
    std::stringstream ss(data);
    std::string token;
    std::vector<std::string> tokens;
    
    while (std::getline(ss, token, ',')) {
        tokens.push_back(token);
    }
    
    if (tokens.size() < 3) return;
    
    int x = std::stoi(tokens[0]);
    int y = std::stoi(tokens[1]);
    std::string cellType = tokens[2];
    
    if (cellType == "WALL") {
        field.getCell(x, y) = Cell(CellType::WALL);
    } else if (cellType == "SLOW") {
        field.getCell(x, y) = Cell(CellType::SLOW);
    } else {
        field.getCell(x, y) = Cell(CellType::EMPTY);
    }
    
    for (size_t i = 3; i < tokens.size(); ++i) {
        if (tokens[i] == "TRAP") {
            field.getCell(x, y).setTrap(true);
        }
        else if (tokens[i] == "PLAYER") {
        }
        else if (tokens[i].find("ENEMY:") == 0) {
            std::string enemyData = tokens[i].substr(6);
            auto enemy = deserializeEnemy(enemyData);
            field.getCell(x, y).setEnemy(enemy);
        }
        else if (tokens[i].find("BASE:") == 0) {
            std::string baseData = tokens[i].substr(5);
            auto base = deserializeEnemyBase(baseData);
            field.getCell(x, y).setBase(base);
        }
        else if (tokens[i].find("TOWER:") == 0) {
            std::string towerData = tokens[i].substr(6);
            auto tower = deserializeEnemyTower(towerData);
            field.getCell(x, y).setTower(tower);
        }
        else if (tokens[i].find("ALLY:") == 0) {
            std::string allyData = tokens[i].substr(5);
            auto ally = deserializeAlly(allyData);
            field.getCell(x, y).setAlly(ally);
        }
    }
}

std::unique_ptr<GameState> SaveLoadManager::deserializeGameState(const std::string& data) {
    std::stringstream ss(data);
    std::string line;
    
    int level = 1;
    int turnCount = 0;
    int score = 0;
    
    int playerHp = 100;
    int playerMaxHp = 100;
    int playerDamage = 10;
    int playerPoints = 0;
    AttackMode attackMode = AttackMode::MELEE;
    bool playerSlowed = false;
    int playerRangedRange = 3;
    float playerSlowResistance = 0.0f;
    
    int fieldWidth = 15;
    int fieldHeight = 15;
    std::vector<std::string> cellData;
    
    while (std::getline(ss, line)) {
        if (line.find("LEVEL:") == 0) {
            level = std::stoi(line.substr(6));
        }
        else if (line.find("TURN:") == 0) {
            turnCount = std::stoi(line.substr(5));
        }
        else if (line.find("SCORE:") == 0) {
            score = std::stoi(line.substr(6));
        }
        else if (line.find("PLAYER_HP:") == 0) {
            playerHp = std::stoi(line.substr(10));
        }
        else if (line.find("PLAYER_MAX_HP:") == 0) {
            playerMaxHp = std::stoi(line.substr(14));
        }
        else if (line.find("PLAYER_DAMAGE:") == 0) {
            playerDamage = std::stoi(line.substr(14));
        }
        else if (line.find("PLAYER_POINTS:") == 0) {
            playerPoints = std::stoi(line.substr(14));
        }
        else if (line.find("PLAYER_ATTACK_MODE:") == 0) {
            attackMode = std::stoi(line.substr(19)) == 0 ? AttackMode::MELEE : AttackMode::RANGED;
        }
        else if (line.find("PLAYER_SLOWED:") == 0) {
            playerSlowed = std::stoi(line.substr(14)) != 0;
        }
        else if (line.find("PLAYER_RANGED_RANGE:") == 0) {
            playerRangedRange = std::stoi(line.substr(20));
        }
        else if (line.find("PLAYER_SLOW_RESISTANCE:") == 0) {
            playerSlowResistance = std::stof(line.substr(23));
        }
        else if (line.find("FIELD_SIZE:") == 0) {
            std::string sizeStr = line.substr(11);
            size_t xPos = sizeStr.find('x');
            if (xPos != std::string::npos) {
                fieldWidth = std::stoi(sizeStr.substr(0, xPos));
                fieldHeight = std::stoi(sizeStr.substr(xPos + 1));
            }
        }
        else if (line.find("CELL:") == 0) {
            cellData.push_back(line);
        }
    }
    
    auto field = std::make_shared<Field>(fieldWidth, fieldHeight);
    auto player = std::make_shared<Player>(playerHp, playerDamage, playerPoints);
    
    player->setAttackMode(attackMode);
    player->setSlowed(playerSlowed);
    player->increaseRangedRange(playerRangedRange - 3);
    player->increaseSlowResistance(playerSlowResistance);
    
    for (const auto& cellLine : cellData) {
        deserializeCell(cellLine, *field);
    }
    
    bool playerPlaced = false;
    for (const auto& cellLine : cellData) {
        if (cellLine.find("PLAYER") != std::string::npos) {
            std::string data = cellLine.substr(5);
            std::stringstream ss(data);
            std::string token;
            std::getline(ss, token, ',');
            int x = std::stoi(token);
            std::getline(ss, token, ',');
            int y = std::stoi(token);
            
            if (field->inBounds(x, y)) {
                field->getCell(x, y).setPlayer(player);
                playerPlaced = true;
                break;
            }
        }
    }
    
    // Если игрок не найден, помещаем его в (0,0)
    if (!playerPlaced && field->inBounds(0, 0)) {
        field->getCell(0, 0).setPlayer(player);
    }
    
    if (player->getHand().size() == 0) {
        player->getHand().addSpell(std::make_shared<DirectDamageSpell>());
        player->getHand().addSpell(std::make_shared<AreaDamageSpell>());
    }
    
    auto gameState = std::make_unique<GameState>(field, player, level);
    gameState->setScore(score);
    
    for (int i = 0; i < turnCount; ++i) {
        gameState->incrementTurn();
    }
    
    return gameState;
}

std::unique_ptr<GameState> SaveLoadManager::loadGame(const std::string& filename) {
    if (!std::filesystem::exists(filename)) {
        throw FileNotFoundException();
    }
    
    try {
        FileHandler file(filename, std::ios::in);
        
        std::stringstream buffer;
        buffer << file.get().rdbuf();
        std::string data = buffer.str();
        
        if (data.empty()) {
            throw CorruptedSaveException();
        }
        
        validateSaveData(data);
        return deserializeGameState(data);
        
    } catch (const FileNotFoundException&) {
        std::cerr << "Ошибка: файл сохранения '" << filename << "' не найден." << std::endl;
        throw;
    } catch (const CorruptedSaveException&) {
        std::cerr << "Ошибка: файл сохранения '" << filename << "' поврежден или имеет неверный формат." << std::endl;
        throw;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при загрузке файла '" << filename << "': " << e.what() << std::endl;
        throw CorruptedSaveException();
    }
}

void SaveLoadManager::saveGame(const GameState& state, const std::string& filename) {
    try {
        FileHandler file(filename, std::ios::out | std::ios::trunc);
        
        std::string data = serializeGameState(state);
        file.get() << data;
        
        if (file.get().fail()) {
            throw WritePermissionException();
        }
        
        std::cout << "Игра успешно сохранена в " << filename << std::endl;
        
    } catch (const WritePermissionException&) {
        std::cerr << "Ошибка: нет прав на запись в файл '" << filename << "'." << std::endl;
        throw;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при сохранении в файл '" << filename << "': " << e.what() << std::endl;
        throw CorruptedSaveException();
    }
}

void SaveLoadManager::validateSaveData(const std::string& data) {
    if (data.empty()) {
        throw CorruptedSaveException();
    }
    
    if (data.find("GAME_STATE") == std::string::npos) {
        throw CorruptedSaveException();
    }
    
    if (data.find("END_GAME_STATE") == std::string::npos) {
        throw CorruptedSaveException();
    }
}