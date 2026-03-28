#include "SpellTower.h"

void SpellTower::activateSpellBook() {
    this->spell_book_.setOwner(this->getGameLogic(), this);
    this->is_active_ = true;
}

void SpellTower::update() {
    if (!this->is_active_) {
        this->activateSpellBook();
        for (int i = 0; i < this->sells_amount_; i++) {
            this->spell_book_.addSpell(new DirectDamageSpellEnemy());
        }
    }

    this->spell_book_.updateTargets();
    if (!this->spell_book_.castSpell()) {
        this->sells_amount_--;
    }
    this->spell_book_.changeMana(1);
    this->is_ready_ = this->spell_book_.getMana() >= 4;
    if (!this->spell_book_.haveSpells()) {
        this->getGameLogic()->removeObject(this->getId());
    }
}

bool SpellTower::isReady() const {
    return this->is_ready_;
}

void SpellTower::saveToJson(nlohmann::json *savefile) {
    if (!savefile) return;
    Object::saveToJson(savefile);

    (*savefile)["is_ready"] = is_ready_;
    (*savefile)["spells_amount"] = sells_amount_;
    (*savefile)["mana"] = this->spell_book_.getMana();
}

void SpellTower::loadFromJson(nlohmann::json *savefile) {
    if (!savefile) return;
    Object::loadFromJson(savefile);

    is_ready_ = savefile->value("is_ready", is_ready_);
    sells_amount_ = savefile->value("spells_amount", sells_amount_);
    spell_book_.changeMana(savefile->value("mana", 0));
}
