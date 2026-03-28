#include "LevelManager.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "GameVisualize.h"

#include <iostream>
#include <ctime>

LevelManager::LevelManager() 
    : currentLevel(1), 
      scoreForNextLevel(100), 
      rng(std::time(nullptr)),
      visualizer() {}  // Используем конструктор по умолчанию

void LevelManager::resetToLevel(int level) {
    currentLevel = level;
    scoreForNextLevel = calculateLevelScore(level);
}

bool LevelManager::shouldLevelUp(const Player& player, const GameField& field) const {
    return field.getEnemyCount() == 0;
}

void LevelManager::advanceLevel(Player& player, GameField& field) {
    currentLevel++;
    scoreForNextLevel = calculateLevelScore(currentLevel);
    
    // Определяем размер поля для нового уровня
    auto size = calculateLevelSize(currentLevel);
    int width = size.first;
    int height = size.second;

    visualizer.visualizeLevelTransition(currentLevel, width, height, scoreForNextLevel);
    
    showLevelUpMenu(player);
    
    // хил
    player.heal(player.getMaxHealth() - player.getHealth());
    
    // уираем рандомное заклинание/решаем убирать ли ззаклинание
    int spellsLost = removeRandomSpells(player);
    visualizer.visualizeSpellLoss(spellsLost);
    
    // Создаем новое поле
    field = GameField(width, height);
    setupEnemiesForLevel(field, currentLevel);
}

int LevelManager::removeRandomSpells(Player& player) {
    auto& spellHand = player.getSpellHand();
    int spellCount = spellHand.getSpellCount();
    int spellsLost = 0;
    
    if (spellCount == 0) return 0;
    
    if (spellCount <= 2) {
        if (std::uniform_int_distribution<>(0, 1)(rng) == 0) {
            int spellToRemove = std::uniform_int_distribution<>(0, spellCount - 1)(rng);
            spellHand.removeSpell(spellToRemove);
            spellsLost = 1;
        }
    } else {
        spellsLost = spellCount / 2;
        for (int i = 0; i < spellsLost; i++) {
            if (spellHand.getSpellCount() > 0) {
                int spellToRemove = std::uniform_int_distribution<>(0, spellHand.getSpellCount() - 1)(rng);
                spellHand.removeSpell(spellToRemove);
            }
        }
    }
    
    return spellsLost;
}

int LevelManager::calculateLevelScore(int level) const {
    return 100 + (level - 1) * 50;
}

std::pair<int, int> LevelManager::calculateLevelSize(int level) const {
    int size = 15 + (level - 1) * 2;
    if (size > 25) size = 25;
    return std::make_pair(size, size);;
}

void LevelManager::setupEnemiesForLevel(GameField& field, int level) {
    int enemyCount = level + 1;
    
    field.getEnemies().clear();
    field.getEnemyPositionsRef().clear();
    
    field.addRandomEnemies(enemyCount);
    
    auto& enemies = field.getEnemies();
    for (auto& enemy : enemies) {
        int baseHealth = enemy.getHealth(); 
        int baseDamage = enemy.getDamage();
        
        enemy.setHealth(baseHealth + (level - 1) * 5);
        enemy.setDamage(baseDamage + (level - 1) * 2);
    }
    
    visualizer.visualizeEnemiesSpawned(enemyCount, level);
}

void LevelManager::setLevelState(int level, int requiredScore) {
    currentLevel = level;
    scoreForNextLevel = requiredScore;
}

void LevelManager::showLevelUpMenu(Player& player) {
    visualizer.visualizeLevelUpMenu(player);
    
    int choice;
    std::cin >> choice;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    switch (choice) {
        case 1:
            player.setMaxHealth(player.getMaxHealth() + 25);
            player.setHealth(player.getHealth() + 25);
            visualizer.visualizeUpgradeResult("player_stat", "", "Health", player.getMaxHealth());
            break;
        case 2:
            player.setMaxMana(player.getMaxMana() + 20);
            player.setMana(player.getMana() + 20); 
            visualizer.visualizeUpgradeResult("player_stat", "", "Mana", player.getMaxMana());
            break;
        case 3:
            player.setMeleeDamage(player.getMeleeDamage() + 5);
            visualizer.visualizeUpgradeResult("player_stat", "", "Melee damage", player.getMeleeDamage());
            break;
        case 4:
            player.setRangedDamage(player.getRangedDamage() + 5);
            visualizer.visualizeUpgradeResult("player_stat", "", "Ranged damage", player.getRangedDamage());
            break;
        case 5:
            upgradeSpell(player);
            break;
        default:
            visualizer.visualizeOperationResult("choice", "invalid", "Health upgraded by default.");
            player.setMaxHealth(player.getMaxHealth() + 25);
            player.setHealth(player.getHealth() + 25);
    }
    
    displayPlayerStats(player);
}

void LevelManager::upgradeSpell(Player& player) {
    SpellHand& spellHand = player.getSpellHand();
    
    if (spellHand.getSpellCount() == 0) {
        visualizer.visualizeNoSpellsToUpgrade();
        player.setMaxHealth(player.getMaxHealth() + 25);
        player.setHealth(player.getHealth() + 25);
        return;
    }
    
    // Используем метод, который рисует меню выбора заклинания
    visualizer.visualizeSpellUpgradeMenu(player);
    
    int spellChoice;
    std::cin >> spellChoice;
    
    if (spellChoice < 1 || spellChoice > spellHand.getSpellCount()) {
        visualizer.visualizeOperationResult("choice", "invalid", "Health upgraded instead.");
        player.setMaxHealth(player.getMaxHealth() + 25);
        player.setHealth(player.getHealth() + 25);
        return;
    }
    
    Spell* spell = spellHand.getSpell(spellChoice - 1);
    
    // улучшение заклинания
    DirectDamageSpell* directSpell = dynamic_cast<DirectDamageSpell*>(spell);
    AreaDamageSpell* areaSpell = dynamic_cast<AreaDamageSpell*>(spell);
    
    if (directSpell) {
        // Рисуем опции для улучшения DirectDamageSpell
        visualizer.visualizeSpellSpecificUpgradeOptions(directSpell);
        
        int upgradeChoice;
        std::cin >> upgradeChoice;
        
        if (upgradeChoice == 1) {
            directSpell->increaseDamage(10);
            visualizer.visualizeSpellSpecificUpgradeOptions(directSpell);
        } else if (upgradeChoice == 2) {
            directSpell->increaseRange(1);
            visualizer.visualizeUpgradeResult("spell", "range", directSpell->getName(), directSpell->getCurrentRange());
        } else {
            visualizer.visualizeOperationResult("choice", "invalid", "Damage upgraded by default.");
            directSpell->increaseDamage(10);
        }
        
    } else if (areaSpell) {
        // Рисуем опции для улучшения AreaDamageSpell
        visualizer.visualizeSpellSpecificUpgradeOptions(areaSpell);
        
        int upgradeChoice;
        std::cin >> upgradeChoice;
        
        if (upgradeChoice == 1) {
            areaSpell->increaseDamage(8);
            visualizer.visualizeUpgradeResult("spell", "damage", areaSpell->getName(), areaSpell->getCurrentDamage());
        } else if (upgradeChoice == 2) {
            areaSpell->increaseArea(1);
            visualizer.visualizeUpgradeResult("spell", "area size", areaSpell->getName(), areaSpell->getCurrentAreaSize());
        } else {
            visualizer.visualizeOperationResult("choice", "invalid", "Damage upgraded by default.");
            areaSpell->increaseDamage(8);
        }
    } else {
        visualizer.visualizeUnknownSpellType();
        player.setMaxHealth(player.getMaxHealth() + 25);
        player.setHealth(player.getHealth() + 25);
    }
}

void LevelManager::displayPlayerStats(const Player& player) {
    visualizer.visualizePlayerStats(player, currentLevel);
}