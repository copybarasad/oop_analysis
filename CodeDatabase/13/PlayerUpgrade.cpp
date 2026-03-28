#include "PlayerUpgrade.h"

void PlayerUpgrade::applyUpgrade(const std::string &name) {
    auto it = this->upgrades_.find(name);
    if (it != this->upgrades_.end()) {
        it->second();
    }
}

void PlayerUpgrade::addHealth() const {
    this->pl_->addMaxHp(5);
}

void PlayerUpgrade::refreshMana() const {
    this->pl_->getSpellBook()->changeMana(10);
}

void PlayerUpgrade::addSpells() const {
    this->pl_->getSpellBook()->addRandomSpell();
    this->pl_->getSpellBook()->addRandomSpell();
}

void PlayerUpgrade::addLevels() const {
    this->pl_->getSpellBook()->changeLevel(2);
}

void PlayerUpgrade::setPlayer(Player *pl) {
    this->pl_ = pl;
}
