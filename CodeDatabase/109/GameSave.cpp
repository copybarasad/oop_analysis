#include "GameSave.h"
#include "Game.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cstdio> 
#include <tuple> 
#include <fstream>

GameSave::GameSave(const std::string& file) : filename(file) {}

void GameSave::saveGame(const Game& game) {
    try {
        SaveData data = serializeGame(game);
        writeToFile(data);
        
        std::ifstream check(filename);
        if (!check.good()) {
            throw SaveException("File was not created properly");
        }
        check.close();
        //std::cout << "Game saved to: " << filename << std::endl;
    }
    catch (const std::exception& e) {
        throw SaveException(std::string("Failed to save game: ") + e.what());
    }
}

void GameSave::loadGame(Game& game) {
    try {
        if (!saveExists()) {
        throw LoadException("Save file does not exist: " + filename);
        }
        
        SaveData data = readFromFile();
        deserializeGame(game, data);
        //std::cout << "Game loaded from: " << filename << std::endl;
    }
    catch (const std::exception& e) {
        throw LoadException(std::string("Failed to load game: ") + e.what());
    }
}

bool GameSave::saveExists() const {
    std::ifstream file(filename);
    bool exists = file.good();
    file.close();
    return exists;
}

GameSave::SaveData GameSave::serializeGame(const Game& game) {
    SaveData data;
    data.level = game.currentLevel;
    
    const Player& player = game.player;
    data.playerHealth = player.getHealth();
    data.playerMaxHealth = player.getMaxHealth();
    data.playerMeleeDamage = player.getMeleeDamage();
    data.playerRangedDamage = player.getRangedDamage();
    data.playerMana = player.getMana();
    data.playerMaxMana = player.getMaxMana();
    data.playerScore = player.getScore();
    data.playerPosX = player.getPos().getX();
    data.playerPosY = player.getPos().getY();
    
    const auto& spellHand = player.getSpellHand();
    for (int i = 0; i < spellHand.getSpellCount(); ++i) {
        Spell* spell = spellHand.getSpell(i);
        data.spellTypes.push_back(spell->getName());
    }

    const GameMap& map = game.map;
    data.mapWidth = map.getWidth();
    data.mapHeight = map.getHeight();
    
    for (int y = 0; y < data.mapHeight; ++y) {
        std::vector<char> row;
        for (int x = 0; x < data.mapWidth; ++x) {
            CellType cell = map.getCell(x, y);
            char symbol;
            switch (cell) {
                case CellType::EMPTY: symbol = '.'; break;
                case CellType::WALL: symbol = '#'; break;
                case CellType::PLAYER: symbol = 'P'; break;
                case CellType::ENEMY: symbol = 'E'; break;
                case CellType::TOWER: symbol = 'T'; break;
                case CellType::TRAP: symbol = 't'; break;
                case CellType::ALLY: symbol = 'A'; break;
                default: symbol = '.';
            }
            row.push_back(symbol);
        }
        data.mapGrid.push_back(row);
    }
    
    for (const auto& enemy : game.enemies) {
        if (enemy.isAlive()) {
            data.enemies.push_back(std::make_tuple(
                enemy.getPos().getX(),
                enemy.getPos().getY(),
                enemy.getHealth(),
                enemy.getDamage()
            ));
        }
    }
    
    for (const auto& tower : game.towers) {
        if (tower.isAlive()) {
            data.towers.push_back(std::make_tuple(
                tower.getPos().getX(),
                tower.getPos().getY(),
                tower.getHealth()
            ));
        }
    }
    
    for (const auto& trap : game.traps) {
        data.traps.push_back(std::make_tuple(
            trap.getX(),
            trap.getY()
        ));
    }
    
    for (const auto& ally : game.allies) {
        if (ally.isAlive()) {
            data.allies.push_back(std::make_tuple(
                ally.getPos().getX(),
                ally.getPos().getY(),
                ally.getHealth()
            ));
        }
    }
    
    return data;
}

void GameSave::writeToFile(const SaveData& data) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw SaveException("Cannot open file for writing: " + filename);
    }
    file << "GAME_SAVE_V1" << std::endl;
    file << data.level << std::endl;
    
    file << data.playerHealth << std::endl;
    file << data.playerMaxHealth << std::endl;
    file << data.playerMeleeDamage << std::endl;
    file << data.playerRangedDamage << std::endl;
    file << data.playerMana << std::endl;
    file << data.playerMaxMana << std::endl;
    file << data.playerScore << std::endl;
    file << data.playerPosX << std::endl;
    file << data.playerPosY << std::endl;

    file << data.spellTypes.size() << std::endl;
    for (const auto& spellType : data.spellTypes) {
        file << spellType << std::endl;
    }
    
    file << data.mapWidth << std::endl;
    file << data.mapHeight << std::endl;
    
    for (const auto& row : data.mapGrid) {
        for (char cell : row) {
            file << cell;
        }
        file << std::endl;
    }

    file << data.enemies.size() << std::endl;
    for (const auto& enemy : data.enemies) {
        file << std::get<0>(enemy) << " " 
            << std::get<1>(enemy) << " " 
            << std::get<2>(enemy) << " " 
            << std::get<3>(enemy) << std::endl;
    }
    
    file << data.towers.size() << std::endl;
    for (const auto& tower : data.towers) {
        file << std::get<0>(tower) << " " 
            << std::get<1>(tower) << " " 
            << std::get<2>(tower) << std::endl;
    }
    
    file << data.traps.size() << std::endl;
    for (const auto& trap : data.traps) {
        file << std::get<0>(trap) << " " 
                << std::get<1>(trap) << std::endl;
    }
    
    file << data.allies.size() << std::endl;
    for (const auto& ally : data.allies) {
        file << std::get<0>(ally) << " " 
            << std::get<1>(ally) << " " 
            << std::get<2>(ally) << std::endl;
    }
    
    file.close();
    
    if (file.fail()) {
        throw SaveException("Failed to write data to file");
    }
}

GameSave::SaveData GameSave::readFromFile() {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw LoadException("Cannot open file: " + filename);
    }
    SaveData data;
    std::string header;
    file >> header;
    
    if (header != "GAME_SAVE_V1") {
        throw LoadException("Invalid save file format. Expected: GAME_SAVE_V1, Got: " + header);
    }
    
    file >> data.level;
    
    file >> data.playerHealth;
    file >> data.playerMaxHealth;
    file >> data.playerMeleeDamage;
    file >> data.playerRangedDamage;
    file >> data.playerMana;
    file >> data.playerMaxMana;
    file >> data.playerScore;
    file >> data.playerPosX;
    file >> data.playerPosY;
    
    size_t spellCount;
    file >> spellCount;
    file.ignore();
    
    for (size_t i = 0; i < spellCount; ++i) {
        std::string spellType;
        std::getline(file, spellType);
        data.spellTypes.push_back(spellType);
    }
    
    file >> data.mapWidth;
    file >> data.mapHeight;
    file.ignore();
    
    data.mapGrid.resize(data.mapHeight);
    for (int y = 0; y < data.mapHeight; ++y) {
        std::string row;
        std::getline(file, row);
        data.mapGrid[y].assign(row.begin(), row.end());
    }
    
    size_t enemyCount;
    file >> enemyCount;
    for (size_t i = 0; i < enemyCount; ++i) {
        int x, y, health, damage;
        file >> x >> y >> health >> damage;
        data.enemies.push_back(std::make_tuple(x, y, health, damage));
    }
    
    size_t towerCount;
    file >> towerCount;
    for (size_t i = 0; i < towerCount; ++i) {
        int x, y, health;
        file >> x >> y >> health;
        data.towers.push_back(std::make_tuple(x, y, health));
    }
    
    size_t trapCount;
    file >> trapCount;
    for (size_t i = 0; i < trapCount; ++i) {
        int x, y;
        file >> x >> y;
        data.traps.push_back(std::make_tuple(x, y));
    }
    
    size_t allyCount;
    file >> allyCount;
    for (size_t i = 0; i < allyCount; ++i) {
        int x, y, health;
        file >> x >> y >> health;
        data.allies.push_back(std::make_tuple(x, y, health));
    }
    
    file.close();
    
    if (file.fail()) {
        throw LoadException("Failed to read data from file or file corrupted");
    }
    
    if (data.mapWidth <= 0 || data.mapHeight <= 0) {
        throw LoadException("Invalid map size in save file");
    }
    return data;
}

void GameSave::deserializeGame(Game& game, const SaveData& data) {    
    game.currentLevel = data.level;
    game.currentGameLevel = GameLevel(data.level);
    
    game.map = GameMap(data.mapWidth, data.mapHeight);
    
    for (int y = 0; y < data.mapHeight; ++y) {
        for (int x = 0; x < data.mapWidth; ++x) {
            char symbol = data.mapGrid[y][x];
            CellType cellType;
            switch (symbol) {
                case '.': cellType = CellType::EMPTY; break;
                case '#': cellType = CellType::WALL; break;
                case 'P': cellType = CellType::PLAYER; break;
                case 'E': cellType = CellType::ENEMY; break;
                case 'T': cellType = CellType::TOWER; break;
                case 't': cellType = CellType::TRAP; break;
                case 'A': cellType = CellType::ALLY; break;
                default: cellType = CellType::EMPTY;
            }
            game.map.setCell(x, y, cellType);
        }
    }
    
    game.player = Player(data.playerMaxHealth, data.playerMeleeDamage, 
                        data.playerRangedDamage, data.playerMaxMana, data.playerMaxMana);
    game.player.setHealth(data.playerHealth);
    game.player.addMana(data.playerMana - game.player.getMana());
    game.player.addScore(data.playerScore - game.player.getScore());
    game.player.setPos(data.playerPosX, data.playerPosY);
    
    game.player.getSpellHand().clearSpells();

    for (const auto& spellName : data.spellTypes) {
        Spell* newSpell = nullptr;
        if (spellName == "Fireball") {
            newSpell = new FireballSpell();
        }
        else if (spellName == "Explosion") {
            newSpell = new ExplosionSpell();
        }
        else if (spellName == "Trap") {
            newSpell = new TrapSpell();
        }
        else if (spellName == "Invisibility") {
            newSpell = new InvisibilitySpell();
        }
        else if (spellName == "Summon") {
            newSpell = new SummonSpell();
        }
        else if (spellName == "Enhance") {
            newSpell = new EnhanceSpell();
        }
        if (newSpell) {
            game.player.getSpellHand().addSpell(newSpell);
        }
    }

    game.enemies.clear();
    game.towers.clear();
    game.traps.clear();
    game.allies.clear();
    
    for (const auto& enemyData : data.enemies) {
        Enemy enemy(std::get<2>(enemyData), std::get<3>(enemyData));
        enemy.setPos(std::get<0>(enemyData), std::get<1>(enemyData));
        game.enemies.push_back(enemy);
    }
    
    for (const auto& towerData : data.towers) {
        EnemyTower tower(std::get<0>(towerData), std::get<1>(towerData));
        tower.setHealth(std::get<2>(towerData));
        game.towers.push_back(tower);
    }
    
    for (const auto& trapData : data.traps) {
        game.traps.push_back(Position(std::get<0>(trapData), std::get<1>(trapData)));
    }
    
    for (const auto& allyData : data.allies) {
        Ally ally(std::get<0>(allyData), std::get<1>(allyData));
        game.allies.push_back(ally);
    }
    
    game.gameLoaded = true;
}

std::vector<std::string> GameSave::getSaveFiles() {
    std::vector<std::string> saves;
    std::ifstream file("game_save.dat");
    if (file.good()) {
        saves.push_back("game_save.dat");
    }
    file.close();
    
    return saves;
}

bool GameSave::deleteSave(const std::string& filename) {
    return std::remove(filename.c_str()) == 0;
}