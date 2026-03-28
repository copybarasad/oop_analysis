#include "Hand.h"
#include "Field.h"
#include "Player.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "BuffSpell.h"
#include "EventSystem.h"
#include <algorithm>
#include <random>

Hand::Hand(size_t maxSize) : maxSize_(maxSize) {}

bool Hand::addSpell(std::shared_ptr<Spell> spell) {
    if (spells_.size() >= maxSize_) return false;
    spells_.push_back(spell);
    
    GameEvent event;
    event.type = EventType::SPELL_OBTAINED;
    event.actor = "Player";
    event.details = "Получено заклинание: " + spell->getName();
    EventSystem::getInstance().notify(event);
    
    return true;
}

std::vector<std::shared_ptr<Spell>> Hand::getAvailableSpells() const {
    return spells_;
}

void Hand::addBuff(BuffSpell::BuffType type) {
    pendingBuffs_.push_back(type);
}

void Hand::applyBuffsTo(Spell& spell) {
    for (auto type : pendingBuffs_) {
        if (auto d = dynamic_cast<DirectDamageSpell*>(&spell)) {
            if (type == BuffSpell::BuffType::DAMAGE) d->setDamageMultiplier(d->getDamageMultiplier() + 1);
        } else if (auto a = dynamic_cast<AreaDamageSpell*>(&spell)) {
            if (type == BuffSpell::BuffType::AREA) a->setSizeMultiplier(a->getSizeMultiplier() + 1);
        } else if (auto t = dynamic_cast<TrapSpell*>(&spell)) {
            if (type == BuffSpell::BuffType::TRAP) t->setDamageMultiplier(t->getDamageMultiplier() + 1);
        } else if (auto s = dynamic_cast<SummonSpell*>(&spell)) {
            if (type == BuffSpell::BuffType::SUMMON) s->setSummonCount(s->getSummonCount() + 1);
        }
    }
}

void Hand::clearBuffs() {
    pendingBuffs_.clear();
}

bool Hand::useSpell(size_t index, Field& field, Player& player, int tx, int ty) {
    if (index >= spells_.size()) return false;
    auto& spell = spells_[index];

    if (spell->isBuff()) {
        auto buff = static_cast<BuffSpell*>(spell.get());
        addBuff(buff->getBuffType());
        spells_.erase(spells_.begin() + index);
        return true;
    }

    applyBuffsTo(*spell);
    bool success = spell->apply(field, player, tx, ty);
    if (success) {
        spells_.erase(spells_.begin() + index);
        clearBuffs();
    }
    return success;
}

bool Hand::removeRandomSpell() {
    if (spells_.empty()) return false;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(0, spells_.size() - 1);
    
    size_t index = dist(gen);
    spells_.erase(spells_.begin() + index);
    return true;
}

bool Hand::removeSpell(size_t index) {
    if (index >= spells_.size()) return false;
    spells_.erase(spells_.begin() + index);
    return true;
}

void Hand::addRandomSpell() {
    if (spells_.size() >= maxSize_) return;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 3);
    
    std::shared_ptr<Spell> newSpell;
    
    switch (dist(gen)) {
        case 0:
            newSpell = std::make_shared<DirectDamageSpell>();
            break;
        case 1:
            newSpell = std::make_shared<AreaDamageSpell>();
            break;
        case 2:
            newSpell = std::make_shared<TrapSpell>();
            break;
        case 3:
            newSpell = std::make_shared<SummonSpell>();
            break;
    }
    
    if (newSpell) {
        spells_.push_back(newSpell);
        
        GameEvent event;
        event.type = EventType::SPELL_OBTAINED;
        event.actor = "Player";
        event.details = "Получено случайное заклинание: " + newSpell->getName();
        EventSystem::getInstance().notify(event);
    }
}
