#include "SummonSpell.h"
#include "IGameWorldFacade.h"
#include <iostream>

SummonSpell::SummonSpell(const std::string& name, AllyManager* am)
    : name_(name), allyManager_(am), summonBonus_(0) {}

bool SummonSpell::cast(SpellContext& context, EnhancementState& enhancements) {
    if (!context.world) {
        return false;
    }
    
    Pos targetPos = context.targetPosition;

    int summonCount = 1 + summonBonus_ + enhancements.summonBonus;

    if (context.world->summonAlly(targetPos, summonCount)) {
        std::cout << "Summoned " << summonCount << " ally/allies at (" << targetPos.x << "," << targetPos.y << ")!\n";
        enhancements.reset();
        return true;
    }
    
    std::cout << "No space to summon allies!\n";
    return false;
}

std::string SummonSpell::getName() const {
    return name_;
}

std::string SummonSpell::getDescription() const {
    return name_ + " (Summons ally near you)";
}

SpellType SummonSpell::getType() const {
    return SpellType::SUMMON;
}

std::unique_ptr<Spell> SummonSpell::clone() const {
    auto spell = std::make_unique<SummonSpell>(name_, allyManager_);
    if (summonBonus_ > 0) {
        EnhancementState bonus;
        bonus.summonBonus = summonBonus_;
        spell->applyPermanentBonus(bonus);
    }
    return spell;
}

void SummonSpell::applyPermanentBonus(const EnhancementState& bonus) {
    summonBonus_ += bonus.summonBonus;
    if (summonBonus_ < 0) summonBonus_ = 0;
}

