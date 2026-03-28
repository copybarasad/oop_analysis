#include "EnhancementSpell.hpp"
#include "GameState.hpp"
#include "SpellCaster.hpp"
#include "Hero.hpp"
#include <memory>
#include <iostream>

EnhancementSpell::EnhancementSpell(int damageBonus, int rangeBonus, int areaBonus, int summonBonus, int manaCost)
    : manaCost_(manaCost), type_(SpellType::Enhancement),
      damageBonus_(damageBonus), rangeBonus_(rangeBonus), areaBonus_(areaBonus), summonBonus_(summonBonus) {}

bool EnhancementSpell::cast(int targetX, int targetY, GameState& gameState, SpellCaster& caster) {
    Hero* hero = dynamic_cast<Hero*>(&caster);
    if (!hero) {
        hero = &gameState.player;
    }
    
    if (hero) {
        hero->addEnhancement();
        std::cout << "Улучшение применено! Текущий уровень улучшений: " << hero->getEnhancementCount() << std::endl;
        return true;
    }
    
    std::cout << "Ошибка: заклинание улучшения может быть применено только героем!" << std::endl;
    return false;
}

std::unique_ptr<ISpell> EnhancementSpell::clone() const {
    return std::make_unique<EnhancementSpell>(*this);
}

void EnhancementSpell::applyEnhancements(int enhancementCount) {
}

void EnhancementSpell::serialize(std::ostream& os) const {
    os.write(reinterpret_cast<const char*>(&manaCost_), sizeof(manaCost_));
    os.write(reinterpret_cast<const char*>(&damageBonus_), sizeof(damageBonus_));
    os.write(reinterpret_cast<const char*>(&rangeBonus_), sizeof(rangeBonus_));
    os.write(reinterpret_cast<const char*>(&areaBonus_), sizeof(areaBonus_));
    os.write(reinterpret_cast<const char*>(&summonBonus_), sizeof(summonBonus_));
}

void EnhancementSpell::deserialize(std::istream& is) {
    is.read(reinterpret_cast<char*>(&manaCost_), sizeof(manaCost_));
    is.read(reinterpret_cast<char*>(&damageBonus_), sizeof(damageBonus_));
    is.read(reinterpret_cast<char*>(&rangeBonus_), sizeof(rangeBonus_));
    is.read(reinterpret_cast<char*>(&areaBonus_), sizeof(areaBonus_));
    is.read(reinterpret_cast<char*>(&summonBonus_), sizeof(summonBonus_));
}