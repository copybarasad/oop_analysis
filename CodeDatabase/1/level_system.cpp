#include "level_system.h"
#include <stdexcept>

LevelSystem::LevelSystem() : currentLevel_(1), maxLevels_(6) {
    initializeLevelConfigs();
}

void LevelSystem::initializeLevelConfigs() {
    levelConfigs_ = {
        // Уровень 1: обучение
        {1, 10, 10, 100, 1, false},
        
        // Уровень 2: усложнение  
        {2, 13, 13, 125, 1, false},
        
        // Уровень 3: развитие
        {3, 16, 16, 150, 2, false},
        
        // Уровень 4: продвинутый
        {4, 19, 19, 175, 2, false},
        
        // Уровень 5: сложный
        {5, 21, 21, 200, 3, false},
        
        // Уровень 6: ФИНАЛ с Гробницей Дракулы
        {6, 24, 24, 225, 3, true}
    };
}

LevelConfig LevelSystem::getCurrentLevelConfig() const {
    return getLevelConfig(currentLevel_);
}

LevelConfig LevelSystem::getLevelConfig(int level) const {
    if (level < 1 || level > maxLevels_) {
        throw std::out_of_range("Invalid level number: " + std::to_string(level));
    }
    return levelConfigs_[level - 1];
}

void LevelSystem::advanceToNextLevel() {
    if (hasMoreLevels()) {
        currentLevel_++;
    }
}

bool LevelSystem::hasMoreLevels() const {
    return currentLevel_ < maxLevels_;
}

bool LevelSystem::isFinalLevel() const {
    return currentLevel_ == maxLevels_;
}

void LevelSystem::applyLevelConfig(Field& field, VampireHunter& hunter, Dracula& dracula) const {
    LevelConfig config = getCurrentLevelConfig();
    
    // Создаем новое поле
    field = Field(config.fieldWidth, config.fieldHeight);
    
    // СБРОС ПОЗИЦИЙ НА НАЧАЛЬНЫЕ
    Position hunterStartPos(1, 1);  
    Position draculaStartPos(config.fieldWidth - 2, config.fieldHeight - 2);  
    
    hunter.moveToPosition(hunterStartPos);
    dracula.moveToPosition(draculaStartPos);
    
    // Устанавливаем здоровье Дракулы
    dracula.setHealth(config.draculaHealth);
    
    if (config.hasDraculaTomb) {
        dracula.setInvulnerable(true);
    } else {
        dracula.setInvulnerable(false); 
    }
    
    // Восстанавливаем охотника
    hunter.setHealth(hunter.getMaxHealth());                            
    hunter.setResearchProgress(0); 
    if (hunter.hasAmmoUpgrade()) {
        hunter.setSilverBullets(7);  
        hunter.setOakStakes(5);
    } else {
        hunter.setSilverBullets(5);  
        hunter.setOakStakes(3);
    }
    hunter.setAmmoUpgrade(false);
   // УДАЛЕНИЕ ПОЛОВИНЫ ЗАКЛИНАНИЙ
    SpellHand& hand = hunter.getSpellHand();
    int spellsToRemove = hand.getSpellCount() / 2;

    for (int i = 0; i < spellsToRemove; ++i) {
        if (hand.getSpellCount() > 0) {
            int randomIndex = rand() % hand.getSpellCount(); 
            hand.removeSpell(randomIndex);
        }
    }
    
    // Генерируем башни
    field.generateTowers(config.towersCount);
    
    // Размещаем гробницу
    if (config.hasDraculaTomb) {
        Position tombPos = getDraculaTombPosition(field);
        field.placeDraculaTomb(tombPos);
    }
}
Position LevelSystem::getDraculaTombPosition(const Field& field) const {
    int x = field.getWidth() - 3;
    int y = field.getHeight() - 3;
    return Position(x, y);
}