#include "game_saver.h"
#include "field.h"
#include "vampire_hunter.h"
#include "dracula.h"
#include "vampire_tower.h"
#include "library.h"
#include "holy_water_spell.h"
#include "light_beam_spell.h"
#include "sacred_glow_spell.h"
#include <sstream>
#include <iostream>

GameSaver::GameSaver(const std::string& filename) : filename_(filename) {}

void GameSaver::saveGame(const Game& game) {
    std::ofstream file(filename_);
    
    if (!file.is_open()) {
        throw GameSaveException("Cannot open file for writing: " + filename_);
    }
    
    try {
        file << FILE_SIGNATURE << "\n";
        file << FILE_VERSION << "\n";
        
        saveGameState(file, game);
        saveField(file, game.getField());        
        saveHunter(file, game.getHunter());     
        saveDracula(file, game.getDracula());    
        
        std::cout << "💾 Игра успешно сохранена в: " << filename_ << "\n";
        
    } catch (const GameException& e) {
        file.close();
        std::remove(filename_.c_str());
        throw;
    } catch (const std::exception& e) {
        file.close();
        std::remove(filename_.c_str());
        throw GameSaveException(std::string("Unexpected error during save: ") + e.what());
    }
}

void GameSaver::loadGame(Game& game) {
    std::ifstream file(filename_);
    
    if (!file.is_open()) {
        throw GameLoadException("Cannot open file for reading: " + filename_);
    }
    
    try {
        validateSaveFile(file);
        loadGameState(file, game);
        loadField(file, game.getField());        
        loadHunter(file, game.getHunter());      
        loadDracula(file, game.getDracula());    
        
        std::cout << "📂 Игра успешно загружена из: " << filename_ << "\n";
        
    } catch (const GameException& e) {
        throw;
    } catch (const std::exception& e) {
        throw GameLoadException(std::string("Unexpected error during load: ") + e.what());
    }
}


void GameSaver::saveGameState(std::ofstream& file, const Game& game) {
    file << game.getTurnCount() << "\n";
    file << game.getLevelSystem().getCurrentLevel() << "\n";  
    file << game.getLevelSystem().getMaxLevels() << "\n";     
    file << (game.isGameRunning() ? "RUNNING" : "STOPPED") << "\n";
}

void GameSaver::saveField(std::ofstream& file, const Field& field) {
    int width = field.getWidth();
    int height = field.getHeight();
    
    file << width << " " << height << "\n";
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Position pos(x, y);
            CellType cell = field.getCellType(pos);
            file << static_cast<int>(cell) << " ";
        }
        file << "\n";
    }
    
    const auto& towers = field.getTowers();
    file << towers.size() << "\n";
    for (const auto& tower : towers) {
        savePosition(file, tower.getPosition());
        file << tower.getHealth() << "\n";
        file << (tower.isAlive() ? 1 : 0) << "\n";
    }
    
    const auto& libraries = field.getLibraries();
    file << libraries.size() << "\n";
    for (const auto& library : libraries) {
        savePosition(file, library.getPosition());
        file << static_cast<int>(library.getAvailableSpell()) << "\n";
        file << (library.isVisited() ? 1 : 0) << "\n";
    }
}

void GameSaver::saveHunter(std::ofstream& file, const VampireHunter& hunter) {
    savePosition(file, hunter.getPosition());
    file << hunter.getHealth() << "\n";
    file << hunter.getResearchProgress() << "\n";
    file << hunter.getSilverBullets() << "\n";
    file << hunter.getOakStakes() << "\n";
    file << (hunter.isRangedMode() ? 1 : 0) << "\n";
    
    // Сохраняем параметры прокачки
    file << hunter.getMaxHealth() << "\n";
    file << hunter.getAttackDamageBonus() << "\n";
    file << hunter.getSpellDamageBonus() << "\n";

    // Сохраняем заклинания
    const auto& hand = hunter.getSpellHand();
    file << hand.getSpellCount() << "\n";
    for (int i = 0; i < hand.getSpellCount(); ++i) {
        Spell* spell = hand.getSpell(i);
        if (spell) {
            file << static_cast<int>(spell->getType()) << "\n";
        }
    }
}

void GameSaver::saveDracula(std::ofstream& file, const Dracula& dracula) {
    savePosition(file, dracula.getPosition());
    file << dracula.getHealth() << "\n";
    file << dracula.getFearLevel() << "\n";
}

void GameSaver::savePosition(std::ofstream& file, const Position& pos) {
    file << pos.getX() << " " << pos.getY() << "\n";
}

void GameSaver::validateSaveFile(std::ifstream& file) {
    std::string signature;
    int version;
    
    file >> signature >> version;
    
    if (file.fail()) {
        throw GameLoadException("Corrupted save file header");
    }
    
    if (signature != FILE_SIGNATURE) {
        throw GameLoadException("Invalid file format: not a Vampire Hunter save file");
    }
    
    if (version != FILE_VERSION) {
        throw GameLoadException("Unsupported save file version: " + std::to_string(version));
    }
}

void GameSaver::loadGameState(std::ifstream& file, Game& game) {
    int turnCount, currentLevel, maxLevels;
    std::string gameRunningStr;
    
    file >> turnCount >> currentLevel >> maxLevels >> gameRunningStr;
    
    if (file.fail()) {
        throw GameLoadException("Corrupted game state data");
    }
    
    game.setTurnCount(turnCount);
    game.setGameRunning(gameRunningStr == "RUNNING");
    
}

void GameSaver::loadField(std::ifstream& file, Field& field) {
    int width, height;
    file >> width >> height;
    
    if (file.fail()) {
        throw GameLoadException("Corrupted field dimensions in save file");
    }
    
    // Создаем временное поле
    Field tempField(width, height);
    
    // Загружаем сетку клеток
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int cellType;
            file >> cellType;
            
            if (file.fail()) {
                throw GameLoadException("Corrupted cell data in save file");
            }
            
            if (cellType < 0 || cellType > 5) { 
                throw GameLoadException("Invalid cell type value: " + std::to_string(cellType));
            }
            
            tempField.setCellType(Position(x, y), static_cast<CellType>(cellType));
        }
    }
    
    // Загружаем башни
    size_t towerCount;
    file >> towerCount;
    
    if (file.fail()) {
        throw GameLoadException("Corrupted towers count in save file");
    }
    
    for (size_t i = 0; i < towerCount; ++i) {
        VampireTower tower = loadTower(file);
        tempField.addTower(tower);
    }
    
    // Загружаем библиотеки
    size_t libraryCount;
    file >> libraryCount;
    
    if (file.fail()) {
        throw GameLoadException("Corrupted libraries count in save file");
    }
    
    for (size_t i = 0; i < libraryCount; ++i) {
        Library library = loadLibrary(file);
        tempField.addLibrary(library);
    }
    
    field = tempField;
}

void GameSaver::loadHunter(std::ifstream& file, VampireHunter& hunter) {
    Position pos = loadPosition(file);
    int health, research, bullets, stakes, rangedMode;
    
    file >> health >> research >> bullets >> stakes >> rangedMode;
    
    if (file.fail()) {
        throw GameLoadException("Corrupted hunter data in save file");
    }

    // Загружаем параметры прокачки
    int maxHealth, attackBonus, spellBonus;
    file >> maxHealth >> attackBonus >> spellBonus;
    
    if (file.fail()) {
        throw GameLoadException("Corrupted hunter data in save file");
    }
    
    // Восстанавливаем все параметры
    hunter.moveToPosition(pos);
    hunter.setHealth(health);
    hunter.setResearchProgress(research);
    hunter.setSilverBullets(bullets);
    hunter.setOakStakes(stakes);
    hunter.setRangedMode(rangedMode == 1);
    
    // Восстанавливаем прокачку
    hunter.setMaxHealth(maxHealth);
    hunter.setAttackDamageBonus(attackBonus);
    hunter.setSpellDamageBonus(spellBonus);
    
    // Загружаем заклинания
    int spellCount;
    file >> spellCount;
    
    if (file.fail()) {
        throw GameLoadException("Corrupted spell count in save file");
    }
    
    // Создаем временную руку
    SpellHand tempHand(hunter.getSpellHand().getMaxSlots());
    
    for (int i = 0; i < spellCount; ++i) {
        Spell* spell = loadSpell(file);
        if (spell) {
            tempHand.addSpell(spell);
        }
    }
    
    hunter.getSpellHand() = tempHand;
}

void GameSaver::loadDracula(std::ifstream& file, Dracula& dracula) {
    Position pos = loadPosition(file);
    int health, fearLevel;
    
    file >> health >> fearLevel;
    
    if (file.fail()) {
        throw GameLoadException("Corrupted dracula data in save file");
    }
    
    dracula.moveToPosition(pos);
    dracula.setHealth(health);
    dracula.setFearLevel(fearLevel);
}


Position GameSaver::loadPosition(std::ifstream& file) {
    int x, y;
    file >> x >> y;
    
    if (file.fail()) {
        throw GameLoadException("Invalid position data");
    }
    
    return Position(x, y);
}

VampireTower GameSaver::loadTower(std::ifstream& file) {
    Position pos = loadPosition(file);
    int health;
    bool alive;
    
    file >> health >> alive;
    
    if (file.fail()) {
        throw GameLoadException("Corrupted tower data");
    }
    
    VampireTower tower(pos);
    tower.setHealth(health);
    return tower;
}

Library GameSaver::loadLibrary(std::ifstream& file) {
    Position pos = loadPosition(file);
    int spellType, visited;
    
    file >> spellType >> visited;
    
    if (file.fail()) {
        throw GameLoadException("Corrupted library data");
    }
    
    if (spellType < 0 || spellType > 2) {
        throw GameLoadException("Invalid spell type value: " + std::to_string(spellType));
    }
    
    Library library(pos, static_cast<SpellType>(spellType));
    
    if (visited == 1) {
        library.confirmInteraction();  
    }
    
    return library;
}

Spell* GameSaver::loadSpell(std::ifstream& file) {
    int spellType;
    file >> spellType;
    
    if (file.fail()) {
        throw GameLoadException("Corrupted spell data");
    }
    
    if (spellType < 0 || spellType > 2) {
        throw GameLoadException("Invalid spell type value: " + std::to_string(spellType));
    }
    
    switch (static_cast<SpellType>(spellType)) {
        case SpellType::HOLY_WATER: return new HolyWaterSpell();
        case SpellType::LIGHT_BEAM: return new LightBeamSpell();
        case SpellType::SACRED_GLOW: return new SacredGlowSpell();
        default: return nullptr;
    }
}

bool GameSaver::saveExists() const {
    std::ifstream file(filename_);
    return file.good();
}