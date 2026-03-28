//
// Created by Mac on 09.11.2025.
//

#include "LevelManager.h"

#include "SpellShop.h"
#include "spells/AreaDamageSpell.h"
#include "spells/DirectDamageSpell.h"
#include "spells/TrapSpell.h"


LevelManager::LevelManager(const int &currentLevel)
    :currentLevel(currentLevel){
    if (currentLevel <= 0)
        this->currentLevel = DEFAULT_CURRENT_LEVEL;
}

int LevelManager::getCurrentLevel() const {
    return this->currentLevel;
}

void LevelManager::setCurrentLevel(const int &currentLevel) {
    this->currentLevel = currentLevel;

    if (currentLevel <= 0)
        this->currentLevel = DEFAULT_CURRENT_LEVEL;
}

void LevelManager::save(Saver &saver) const {
    saver.save(currentLevel);
}

void LevelManager::load(Reader &reader) {
    reader.read(currentLevel);
}



