#include "SaveSystem.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "UpgradeSpell.h"
#include "GameController.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>

const std::string SaveSystem::SAVE_FILE_PATH = "game_save.dat";

void SaveSystem::saveGame(const GameController& controller, const LevelManager& levelManager) {
    try {
        const Player& player = controller.getPlayer();
        if (!player.isAlive()) {
            throw SaveException("Cannot save: player is dead");
        }
        
        int currentLevel = levelManager.getCurrentLevel();
        if (currentLevel < 1 || currentLevel > GameConfig::getMaxLevels()) {
            throw SaveException("Cannot save: invalid level " + std::to_string(currentLevel));
        }
        
        std::cout << "Saving game at level " << currentLevel << "..." << std::endl;
        
        GameSaveData data = serializeGameState(controller, levelManager);
        std::vector<SpellSaveData> spells = serializeSpells(controller.getHand());
        
        validateSaveData(data);
        
        std::cout << "Opening save file: " << SAVE_FILE_PATH << std::endl;
        std::ofstream file(SAVE_FILE_PATH, std::ios::binary);
        
        if (!file.is_open()) {
            throw SaveException("Cannot open save file for writing: " + SAVE_FILE_PATH);
        }
        
        const char* header = "RPGSAVE";
        file.write(header, 7);
        
        int version = 2;
        file.write(reinterpret_cast<const char*>(&version), sizeof(version));
        
        file.write(reinterpret_cast<const char*>(&data.currentLevel), sizeof(data.currentLevel));
        file.write(reinterpret_cast<const char*>(&data.playerHealth), sizeof(data.playerHealth));
        file.write(reinterpret_cast<const char*>(&data.playerMaxHealth), sizeof(data.playerMaxHealth));
        file.write(reinterpret_cast<const char*>(&data.playerMeleeDamage), sizeof(data.playerMeleeDamage));
        file.write(reinterpret_cast<const char*>(&data.playerRangedDamage), sizeof(data.playerRangedDamage));
        file.write(reinterpret_cast<const char*>(&data.playerScore), sizeof(data.playerScore));
        file.write(reinterpret_cast<const char*>(&data.playerCoins), sizeof(data.playerCoins));
        file.write(reinterpret_cast<const char*>(&data.playerPosition.x), sizeof(data.playerPosition.x));
        file.write(reinterpret_cast<const char*>(&data.playerPosition.y), sizeof(data.playerPosition.y));
        file.write(reinterpret_cast<const char*>(&data.playerCombatType), sizeof(data.playerCombatType));
        file.write(reinterpret_cast<const char*>(&data.fieldWidth), sizeof(data.fieldWidth));
        file.write(reinterpret_cast<const char*>(&data.fieldHeight), sizeof(data.fieldHeight));
        
        int cellsCount = data.fieldCells.size();
        file.write(reinterpret_cast<const char*>(&cellsCount), sizeof(cellsCount));
        for (const auto& cell : data.fieldCells) {
            file.write(reinterpret_cast<const char*>(&cell), sizeof(cell));
        }
        
        int spellsCount = spells.size();
        file.write(reinterpret_cast<const char*>(&spellsCount), sizeof(spellsCount));
        
        std::cout << "Saving " << spellsCount << " spells..." << std::endl;
        
        for (const auto& spell : spells) {
            file.write(reinterpret_cast<const char*>(&spell.type), sizeof(spell.type));
            file.write(reinterpret_cast<const char*>(&spell.damage), sizeof(spell.damage));
            file.write(reinterpret_cast<const char*>(&spell.range), sizeof(spell.range));
            file.write(reinterpret_cast<const char*>(&spell.width), sizeof(spell.width));
            file.write(reinterpret_cast<const char*>(&spell.height), sizeof(spell.height));
            file.write(reinterpret_cast<const char*>(&spell.count), sizeof(spell.count));
            file.write(reinterpret_cast<const char*>(&spell.power), sizeof(spell.power));
            file.write(reinterpret_cast<const char*>(&spell.improved), sizeof(spell.improved));
            
            int nameLength = spell.name.size();
            file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
            
            if (nameLength > 0) {
                file.write(spell.name.c_str(), nameLength);
            }
        }
        
        if (!file.good()) {
            throw SaveException("Error writing to save file");
        }
        
        file.close();
        std::cout << "Game saved successfully to: " << SAVE_FILE_PATH << std::endl;
        
    } catch (const std::exception& e) {
        throw SaveException(std::string("Failed to save game: ") + e.what());
    }
}

std::pair<std::unique_ptr<GameController>, std::unique_ptr<LevelManager>> SaveSystem::loadGame() {
    try {
        if (!saveExists()) {
            throw FileNotFoundException(SAVE_FILE_PATH);
        }
        
        std::ifstream file(SAVE_FILE_PATH, std::ios::binary);
        if (!file.is_open()) {
            throw LoadException("Cannot open save file for reading: " + SAVE_FILE_PATH);
        }
        
        char header[8] = {0};
        file.read(header, 7);
        if (std::string(header) != "RPGSAVE") {
            throw CorruptedSaveException("Invalid file header");
        }
        
        int version;
        file.read(reinterpret_cast<char*>(&version), sizeof(version));
        if (version < 1 || version > 2) {
            throw CorruptedSaveException("Unsupported save version: " + std::to_string(version));
        }
        
        GameSaveData data;
        
        file.read(reinterpret_cast<char*>(&data.currentLevel), sizeof(data.currentLevel));
        file.read(reinterpret_cast<char*>(&data.playerHealth), sizeof(data.playerHealth));
        file.read(reinterpret_cast<char*>(&data.playerMaxHealth), sizeof(data.playerMaxHealth));
        file.read(reinterpret_cast<char*>(&data.playerMeleeDamage), sizeof(data.playerMeleeDamage));
        file.read(reinterpret_cast<char*>(&data.playerRangedDamage), sizeof(data.playerRangedDamage));
        file.read(reinterpret_cast<char*>(&data.playerScore), sizeof(data.playerScore));
        file.read(reinterpret_cast<char*>(&data.playerCoins), sizeof(data.playerCoins));
        file.read(reinterpret_cast<char*>(&data.playerPosition.x), sizeof(data.playerPosition.x));
        file.read(reinterpret_cast<char*>(&data.playerPosition.y), sizeof(data.playerPosition.y));
        file.read(reinterpret_cast<char*>(&data.playerCombatType), sizeof(data.playerCombatType));
        file.read(reinterpret_cast<char*>(&data.fieldWidth), sizeof(data.fieldWidth));
        file.read(reinterpret_cast<char*>(&data.fieldHeight), sizeof(data.fieldHeight));
        
        int cellsCount;
        file.read(reinterpret_cast<char*>(&cellsCount), sizeof(cellsCount));
        data.fieldCells.resize(cellsCount);
        for (int i = 0; i < cellsCount; i++) {
            file.read(reinterpret_cast<char*>(&data.fieldCells[i]), sizeof(CellType));
        }
        
        int spellsCount;
        file.read(reinterpret_cast<char*>(&spellsCount), sizeof(spellsCount));
        
        if (spellsCount < 0 || spellsCount > 100) {
            throw CorruptedSaveException("Invalid spell count: " + std::to_string(spellsCount));
        }
        
        std::vector<SpellSaveData> spells;
        spells.reserve(spellsCount);
        
        for (int i = 0; i < spellsCount; i++) {
            SpellSaveData spell;
            
            file.read(reinterpret_cast<char*>(&spell.type), sizeof(spell.type));
            file.read(reinterpret_cast<char*>(&spell.damage), sizeof(spell.damage));
            file.read(reinterpret_cast<char*>(&spell.range), sizeof(spell.range));
            file.read(reinterpret_cast<char*>(&spell.width), sizeof(spell.width));
            file.read(reinterpret_cast<char*>(&spell.height), sizeof(spell.height));
            file.read(reinterpret_cast<char*>(&spell.count), sizeof(spell.count));
            file.read(reinterpret_cast<char*>(&spell.power), sizeof(spell.power));
            file.read(reinterpret_cast<char*>(&spell.improved), sizeof(spell.improved));
            
            int nameLength;
            file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
            
            if (nameLength < 0 || nameLength > 100) {
                throw CorruptedSaveException("Invalid spell name length: " + std::to_string(nameLength));
            }
            
            if (nameLength > 0) {
                spell.name.resize(nameLength);
                file.read(&spell.name[0], nameLength);
            }
            
            spells.push_back(spell);
        }
        
        if (!file.good() && !file.eof()) {
            throw CorruptedSaveException("Error reading save file data");
        }
        
        file.close();
        
        validateSaveData(data);
        
        auto controller = std::make_unique<GameController>(data.fieldWidth, data.fieldHeight);
        auto levelManager = std::make_unique<LevelManager>();
        
        deserializeGameState(*controller, *levelManager, data, spells);
        
        std::cout << "Game loaded successfully! Level: " << data.currentLevel << std::endl;
        std::cout << "Player health: " << data.playerHealth << "/" << data.playerMaxHealth << std::endl;
        std::cout << "Player coins: " << data.playerCoins << std::endl;
        std::cout << "Spells loaded: " << spells.size() << std::endl;
        
        return {std::move(controller), std::move(levelManager)};
        
    } catch (const std::exception& e) {
        throw LoadException(std::string("Failed to load game: ") + e.what());
    }
}

bool SaveSystem::saveExists() {
    std::ifstream file(SAVE_FILE_PATH);
    return file.good();
}

GameSaveData SaveSystem::serializeGameState(const GameController& controller, const LevelManager& levelManager) {
    GameSaveData data;
    const Player& player = controller.getPlayer();  
    const GameField& field = controller.getField();
    
    data.currentLevel = levelManager.getCurrentLevel();
    data.playerHealth = player.getHealth();
    data.playerMaxHealth = player.getMaxHealth();
    data.playerMeleeDamage = player.getMeleeDamage();
    data.playerRangedDamage = player.getRangedDamage();
    data.playerScore = player.getScore();
    data.playerCoins = player.getCoins();
    data.playerPosition = player.getPosition();
    data.playerCombatType = player.getCombatType();
    data.fieldWidth = field.getWidth();
    data.fieldHeight = field.getHeight();
    data.fieldCells.clear();
    data.fieldCells.reserve(data.fieldWidth * data.fieldHeight);
    
    for (int y = 0; y < data.fieldHeight; ++y) {
        for (int x = 0; x < data.fieldWidth; ++x) {
            Position pos(x, y);
            const GameCell& cell = field.getCell(pos);
            data.fieldCells.push_back(cell.getType());
        }
    }

    return data;
}

void SaveSystem::validateSaveData(const GameSaveData& data) {
    if (data.currentLevel < 1 || data.currentLevel > GameConfig::getMaxLevels()) {
        throw CorruptedSaveException("Invalid level number: " + std::to_string(data.currentLevel));
    }
    
    if (data.playerHealth < 0 || data.playerHealth > data.playerMaxHealth) {
        throw CorruptedSaveException("Invalid player health: " + std::to_string(data.playerHealth) + 
                                   "/" + std::to_string(data.playerMaxHealth));
    }
    
    if (data.playerMaxHealth < 50 || data.playerMaxHealth > 500) {
        throw CorruptedSaveException("Invalid max health: " + std::to_string(data.playerMaxHealth));
    }
    
    if (data.playerMeleeDamage < 1 || data.playerMeleeDamage > 100) {
        throw CorruptedSaveException("Invalid melee damage: " + std::to_string(data.playerMeleeDamage));
    }
    
    if (data.playerRangedDamage < 1 || data.playerRangedDamage > 100) {
        throw CorruptedSaveException("Invalid ranged damage: " + std::to_string(data.playerRangedDamage));
    }
    
    if (data.playerScore < 0) {
        throw CorruptedSaveException("Invalid player score: " + std::to_string(data.playerScore));
    }
    
    if (data.playerCoins < 0) {
        throw CorruptedSaveException("Invalid player coins: " + std::to_string(data.playerCoins));
    }
    
    if (data.fieldWidth < 10 || data.fieldWidth > 100 || 
        data.fieldHeight < 10 || data.fieldHeight > 100) {
        throw CorruptedSaveException("Invalid field dimensions: " + 
                                   std::to_string(data.fieldWidth) + "x" + std::to_string(data.fieldHeight));
    }
    
    size_t expectedCells = static_cast<size_t>(data.fieldWidth * data.fieldHeight);
    if (data.fieldCells.size() != expectedCells) {
        throw CorruptedSaveException("Field cells count mismatch. Expected " + 
                                   std::to_string(expectedCells) + ", got " + 
                                   std::to_string(data.fieldCells.size()));
    }
}

std::vector<SpellSaveData> SaveSystem::serializeSpells(const Hand& hand) {
    std::vector<SpellSaveData> spells;
    
    for (int i = 0; i < hand.size(); i++) {
        const ISpell* spell = hand.getAt(i);
        if (!spell) continue;
        
        SpellSaveData spellData;
        spellData.improved = spell->isImproved();
        spellData.name = spell->name();
        
        int damage = 0, range = 0, width = 0, height = 0, count = 0, power = 0;
        spell->getParameters(damage, range, width, height, count, power);
        
        spellData.type = spell->getType();
        spellData.damage = damage;
        spellData.range = range;
        spellData.width = width;
        spellData.height = height;
        spellData.count = count;
        spellData.power = power;
        
        std::cout << "Saving spell: " << spellData.name 
                  << ", damage: " << spellData.damage
                  << ", range: " << spellData.range
                  << ", improved: " << spellData.improved << std::endl;
        
        spells.push_back(spellData);
    }
    
    return spells;
}

void SaveSystem::deserializeSpells(Hand& hand, const std::vector<SpellSaveData>& spells) {
    hand = Hand(5);
    
    for (const auto& spellData : spells) {
        std::unique_ptr<ISpell> spell;
        
        std::cout << "Loading spell: " << spellData.name 
                  << ", type: " << static_cast<int>(spellData.type)
                  << ", damage: " << spellData.damage
                  << ", range: " << spellData.range
                  << ", improved: " << spellData.improved << std::endl;
        
        switch (spellData.type) {
            case SpellType::DIRECT_DAMAGE:
                spell = std::make_unique<DirectDamageSpell>(spellData.damage, spellData.range);
                break;
            case SpellType::AREA_DAMAGE:
                spell = std::make_unique<AreaDamageSpell>(spellData.damage, 
                                                         spellData.width, 
                                                         spellData.height, 
                                                         spellData.range);
                break;
            case SpellType::TRAP:
                spell = std::make_unique<TrapSpell>(spellData.damage);
                break;
            case SpellType::SUMMON:
                spell = std::make_unique<SummonSpell>(spellData.count);
                break;
            case SpellType::UPGRADE:
                spell = std::make_unique<UpgradeSpell>(spellData.power);
                break;
            default:
                std::cerr << "Warning: Unknown spell type in save file: " 
                         << static_cast<int>(spellData.type) << std::endl;
                continue;
        }
        
        if (spell) {
            if (spellData.improved) {
                spell->setImproved(true);
            }
            hand.addSpell(std::move(spell));
        }
    }
}

void SaveSystem::deserializeGameState(GameController& controller, LevelManager& levelManager, 
                                      const GameSaveData& data, const std::vector<SpellSaveData>& spells) {
    controller.getFieldMutable().initializeFromData(data.fieldWidth, data.fieldHeight, data.fieldCells);
    
    Player& player = controller.getPlayerMutable();
    
    player.setPosition(data.playerPosition);
    
    player.setMaxHealth(data.playerMaxHealth);
    int currentHealth = data.playerHealth;
    if (currentHealth > 0) {
        player.takeDamage(data.playerMaxHealth - currentHealth);
    }
    
    player.setMeleeDamage(data.playerMeleeDamage);
    player.setRangedDamage(data.playerRangedDamage);
    
    player.addScore(data.playerScore - player.getScore());
    
    player.addCoins(data.playerCoins - player.getCoins());
    
    while (player.getCombatType() != data.playerCombatType) {
        player.switchCombatType();
    }
    
    deserializeSpells(controller.getHand(), spells);
    
    levelManager.setCurrentLevel(data.currentLevel);
    
    controller.getEnemies().clear();
    controller.getBuildings().clear();
    controller.getTowers().clear();
    controller.getTraps().clear();
    controller.getAllies().clear();
}