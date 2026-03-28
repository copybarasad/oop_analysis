#include "LevelManager.h"
#include "Cell.h"
#include "EnemyController.h"
#include "EnemyBuildingController.h"
#include <random>
#include <utility>

LevelManager::LevelManager() : currentLevel(1) {
    field = std::make_unique<GameField>(15, 15);
}

void LevelManager::generateRandomField() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> rnd100(0, 99);
    int width = field->getWidth();
    int height = field->getHeight();

    for (int y = 1; y < height; ++y) {
        for (int x = 1; x < width; ++x) {
            if (rnd100(gen) < 15) {
                field->getCells()[y][x].setType(CellType::WALL);
            }
        }
    }
    for (int y = 1; y < height; ++y) {
        for (int x = 1; x < width; ++x) {
            if (field->getCells()[y][x].getType() == CellType::EMPTY && rnd100(gen) < 10) {
                field->getCells()[y][x].setType(CellType::SLOW);
            }
        }
    }
}

std::pair<int, int> LevelManager::getLevelDimensions(int level) const {
    if (level == 1) {
        return {15, 15};
    } else {
        return {20, 20};
    }
}

void LevelManager::transferPlayerState(Player& from, Player& to, bool upgradeHealth, bool upgradeDamage) {
    int savedMaxHealth = from.getMaxHealth();
    int savedHealth = from.getHealth();
    int savedMoney = from.getMoney();
    CombatMode savedCombatMode = from.getCombatMode();
    int savedMeleeDamage = from.getMeleeDamage();
    int savedRangedDamage = from.getRangedDamage();
    
    auto& hand = from.getHand();
    auto spells = hand.getSpells();
    
    if (!spells.empty()) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, spells.size() - 1);
        
        int removeCount = spells.size() / 2;
        for (int i = 0; i < removeCount && !spells.empty(); ++i) {
            int index = dis(gen) % spells.size();
            hand.removeSpell(index);
            spells = hand.getSpells();
        }
    }
    
    std::vector<SpellType> savedSpells = hand.getSpells();
    int savedPendingBoost = hand.getPendingBoostMultiplier();
    bool savedSpellDelay = hand.getSpellCastDelayActive();
    
    // Базовое увеличение здоровья на 50 при переходе на следующий уровень
    int newMaxHealth = savedMaxHealth + 50;
    int newHealth = savedHealth + 50;
    
    // Применяем улучшение здоровья на 10 единиц, если выбрано
    if (upgradeHealth) {
        newMaxHealth += 10;
        newHealth += 10;
    }
    
    // Применяем улучшение урона на 10 единиц, если выбрано
    int newMeleeDamage = savedMeleeDamage;
    int newRangedDamage = savedRangedDamage;
    if (upgradeDamage) {
        newMeleeDamage += 10;
        newRangedDamage += 10;
    }
    
    to.setMaxHealth(newMaxHealth);
    to.setHealth(newHealth);
    to.setMoney(savedMoney);
    to.setCombatMode(savedCombatMode);
    to.setMeleeDamage(newMeleeDamage);
    to.setRangedDamage(newRangedDamage);
    to.setCanMove(true);
    
    auto& newHand = to.getHand();
    newHand.setPendingBoostMultiplier(savedPendingBoost);
    newHand.setSpellCastDelayActive(savedSpellDelay);
    for (const auto& spell : savedSpells) {
        newHand.addSpell(spell);
    }
}

void LevelManager::initializeLevel(int level) {
    currentLevel = level;
    auto [width, height] = getLevelDimensions(level);
    field = std::make_unique<GameField>(width, height);
    generateRandomField();
    
    field->getPlayer().setPosition(0, 0);
    field->getCells()[0][0].setHasEntity(true);
}

void LevelManager::nextLevel(bool upgradeHealth, bool upgradeDamage) {
    if (field) {
        Player oldPlayer = field->getPlayer();
        currentLevel++;
        initializeLevel(currentLevel);
        transferPlayerState(oldPlayer, field->getPlayer(), upgradeHealth, upgradeDamage);
    }
}

