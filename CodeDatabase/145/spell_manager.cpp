#include "spell_manager.h"
#include "spell.h"
#include "player.h"
#include "game_field.h"
#include "enemy.h"
#include "enemy_building.h"
#include "enemy_tower.h"
#include "ally.h"
#include "spell_context_factory.h"
#include <iostream>
#include <string>

SpellManager::SpellManager(size_t maxHandSize) 
    : playerHand(maxHandSize), enhancementLevel(0) {}

bool SpellManager::addSpellToHand(std::unique_ptr<Spell> spell) {
    return playerHand.addSpell(std::move(spell));
}

PlayerHand& SpellManager::getHand() {
    return playerHand;
}

const PlayerHand& SpellManager::getHand() const {
    return playerHand;
}

bool SpellManager::useSpell(int spellIndex, Player& player, GameField& field,
                            std::vector<Enemy>& enemies,
                            std::vector<EnemyBuilding>& buildings,
                            std::vector<EnemyTower>& towers,
                            int targetX, int targetY) {
    auto* spell = getSpellAt(spellIndex);
    if (!spell || !canUseSpell(spellIndex, player, field, enemies, buildings, towers, targetX, targetY)) {
        return false;
    }

    bool isEnhancement = (std::string(spell->getName()) == "Enhancement");

    if (!isEnhancement) {
        for (int i = 0; i < enhancementLevel; ++i) {
            spell->applyEnhancement();
        }
        enhancementLevel = 0;
    }

    std::vector<Ally> allies;
    bool result = SpellContextFactory::applySpellWithContext(*spell, player, field, enemies, buildings, towers, allies, targetX, targetY);

    if (result && isEnhancement) {
        enhancementLevel++;
    }
    return result;
}

bool SpellManager::canUseSpell(int spellIndex, const Player& player,
                               const GameField& field,
                               const std::vector<Enemy>& enemies,
                               const std::vector<EnemyBuilding>& buildings,
                               const std::vector<EnemyTower>& towers,
                               int targetX, int targetY) const {
    auto* spell = getSpellAt(spellIndex);
    if (!spell) return false;

    std::vector<Ally> allies;
    return SpellContextFactory::canUseSpellWithContext(*spell, player, field, enemies, buildings, towers, allies, targetX, targetY);
}

Spell* SpellManager::getSpellAt(int index) const {
    const auto& spells = playerHand.getSpells();
    if (index < 0 || index >= (int)spells.size()) {
        return nullptr;
    }
    return spells[index].get();
}

int SpellManager::getEnhancementLevel() const {
    return enhancementLevel;
}

void SpellManager::resetEnhancements() {
    enhancementLevel = 0;
}

void SpellManager::setEnhancementLevel(int lvl) {
    enhancementLevel = lvl;
}