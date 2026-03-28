#include "Player.h"

#include "../basics/Map.h"


void Player::onDeath() {
    this->is_alive_ = false;
}


bool Player::isAlive() const {
    return this->is_alive_;
}

void Player::findTarget(int dx, int dy) {
    if (!this->getStun()) {
        Object *obj = nullptr;
        Coordinates pos = this->getPosition();
        for (int i = 0; i < this->getWeapon()->range_; i++) {
            if (abs(dx) == 1) {
                pos.x_ += dx;
                obj = this->getMap()->getFromCell(pos);
            } else if (abs(dy) == 1) {
                pos.y_ += dy;
                obj = this->getMap()->getFromCell(pos);
            }
            if (obj != nullptr && obj->getType() == ObjTypes::Hostile) {
                int miss = this->attack(obj);
                if (!miss && this->getWeapon()->range_ == 1) {
                    this->getSpellBook()->changeMana(1);
                }
            }
        }
    } else {
        this->addStun(-1);
    }
}

void Player::activateSpellBook() {
    this->spell_book_.setOwner(this->getGameLogic(), this);
}

void Player::switchBookState() {
    this->is_book_open_ = !this->is_book_open_;
    if (this->isBookOpen()) {
        this->addWeapon(this->book_);
    } else {
        this->getSpellBook()->changeSpell(-this->getSpellBook()->getChosenSpellId());
        this->deleteWeapon();
    }
}

bool Player::isBookOpen() const {
    return this->is_book_open_;
}

SpellManager *Player::getSpellBook() {
    return &this->spell_book_;
}

void Player::healFull() {
    this->onHit(this->getHp() - this->max_hp_);
}

void Player::addMaxHp(const int hp) {
    this->max_hp_ += hp;
    this->healFull();
}

void Player::saveToJson(nlohmann::json *savefile) {
    if (!savefile) return;
    Entity::saveToJson(savefile);

    (*savefile)["max_hp"] = this->max_hp_;

    nlohmann::json spellJson;
    this->spell_book_.saveToJson(&spellJson);
    (*savefile)["spell_book"] = spellJson;

    std::string w = this->getWeapon()->name_;
    if (w == "SpellBook") {
        this->deleteWeapon();
    }
    if (this->getWeapon()->name_ == "Pan") {
        (*savefile)["weapon"] = 0;
    } else {
        (*savefile)["weapon"] = 1;
    }
    if (w == "SpellBook") {
        this->addWeapon(new Book(this));
    }
}

void Player::loadFromJson(nlohmann::json *savefile) {
    if (!savefile) return;
    Entity::loadFromJson(savefile);

    this->max_hp_ = savefile->value("max_hp", this->max_hp_);
    this->activateSpellBook();

    nlohmann::json spellJson = (*savefile)["spell_book"];
    this->spell_book_.loadFromJson(&spellJson);

    int w = savefile->value("weapon", 0);
    if (w) {
        int stun = this->getStun();
        this->addStun(-stun);
        this->changeWeapon();
        if (stun) {
            this->addStun(stun);
        }
    }
}
