#include "SpellManager.h"
#include <cmath>
#include <random>


void SpellManager::setOwner(GameLogic *glogic, Object *owner) {
    this->glogic_ = glogic;
    this->owner_ = owner;
}

void SpellManager::changeMana(const int mana) {
    this->mana_ += mana;
    if (this->mana_ < 0) {
        this->mana_ = 0;
    } else if (this->mana_ > this->max_mana_) {
        this->mana_ = this->max_mana_;
    }
}

int SpellManager::getMana() const {
    return this->mana_;
}


std::vector<std::tuple<std::string, std::string, int, int> > SpellManager::getSpells() {
    std::vector<std::tuple<std::string, std::string, int, int> > spells;
    for (const auto &name: this->keys_) {
        spells.push_back({
            name,
            this->spells_[name].first->description_,
            this->spells_[name].second,
            this->spells_[name].first->cost_
        });
    }
    return spells;
}

int SpellManager::addSpell(Spell *spell) {
    if (this->spells_count_ < this->max_spell_) {
        auto it = spells_.find(spell->name_);
        if (it != spells_.end()) {
            delete spell;
            it->second.second++;
        } else {
            this->keys_.push_back(spell->name_);
            spell->setLevel(this->level_);
            spells_.emplace(spell->name_, std::make_pair(spell, 1));
        }
        if (std::ranges::find(known_spells_, spell->name_) == known_spells_.end()) {
            known_spells_.push_back(spell->name_);
        }

        this->spells_count_++;
        return 0;
    }
    return 1;
}

void SpellManager::changeSpell(const int delta) {
    if (!this->spells_.empty()) {
        this->chosenSpell_ += delta + this->spells_.size();
        this->chosenSpell_ %= this->spells_.size();
        this->updateTargets();
    }
}

int SpellManager::getChosenSpellId() const {
    return this->chosenSpell_;
}

void SpellManager::changeLevel(const int delta) {
    this->level_ += delta;
    for (auto i: this->keys_) {
        this->spells_.at(i).first->setLevel(this->level_);
    }
}

int SpellManager::getLevel() const {
    return this->level_;
}

void SpellManager::showTargets() {
    if (this->spells_.size() > 0) {
        Spell *spell = this->spells_.at(this->keys_[this->chosenSpell_]).first;
        Coordinates pos = this->owner_->getPosition();
        Coordinates new_pos;
        new_pos.layer_ = 2;
        Map &map = this->glogic_->getMap();
        for (auto i: this->targets_) {
            if (i == this->targets_[this->chosenTarget_]) {
                for (int dy = 0; dy < spell->area_ - this->areaDy0_; dy++) {
                    for (int dx = 0; dx < spell->area_ - this->areaDx0_; dx++) {
                        new_pos = i;
                        new_pos.x_ += dx;
                        new_pos.y_ += dy;
                        this->marks_.push_back(new Mark("Chosen"));

                        new_pos.layer_ = 2;
                        this->glogic_->addObject(this->marks_.back(), new_pos);
                    }
                }
                continue;
            }
            new_pos = i;
            this->marks_.push_back(new Mark("Available"));
            new_pos.layer_ = 2;
            this->glogic_->addObject(this->marks_.back(), new_pos);
        }

        for (int dx = -spell->range_; dx < 1 + spell->range_; dx++) {
            for (int dy = -spell->range_; dy < 1 + spell->range_; dy++) {
                new_pos.x_ = pos.x_ + dx;
                new_pos.y_ = pos.y_ + dy;
                new_pos.layer_ = 2;
                if (!map.isOccupied(new_pos)) {
                    this->marks_.push_back(new Mark());
                    this->glogic_->addObject(this->marks_.back(), new_pos);
                }
            }
        }
    }
}


void SpellManager::hideTargets() {
    for (auto i: this->marks_) {
        delete i;
    }
    this->marks_.clear();
}

void SpellManager::updateTargets() {
    this->chosenTarget_ = 0;
    this->targets_.clear();
    if (!this->spells_.empty()) {
        Spell *spell = this->spells_.at(this->keys_[this->chosenSpell_]).first;

        Coordinates pos = this->owner_->getPosition();
        Coordinates new_pos = pos;
        Map &map = this->glogic_->getMap();

        this->areaDx0_ = -std::min(0, static_cast<int>(pos.x_) + 1 + spell->range_ - spell->area_);
        this->areaDy0_ = -std::min(0, static_cast<int>(pos.y_) + 1 + spell->range_ - spell->area_);
        this->areaDx1_ = std::max(0, static_cast<int>(pos.x_) - spell->range_ + spell->area_
                                     - static_cast<int>(map.getSize().x_));
        this->areaDy1_ = std::max(0, static_cast<int>(pos.y_) - spell->range_ + spell->area_
                                     - static_cast<int>(map.getSize().y_));

        for (int dy = -spell->range_; dy < 1 + spell->range_; dy++) {
            for (int dx = -spell->range_; dx < 1 + spell->range_; dx++) {
                new_pos.x_ = pos.x_ + dx;
                new_pos.y_ = pos.y_ + dy;
                new_pos.layer_ = pos.layer_;
                if (spell->targetType_ == ObjTypes::Neutral) {
                    if (spell->mustBeFree_) {
                        bool free = true;
                        for (int i = 0; i < map.getSize().layer_; i++) {
                            new_pos.layer_ = i;
                            if (map.isOccupied(new_pos)) {
                                free = false;
                                break;
                            }
                        }
                        if (free) {
                            new_pos.layer_ = pos.layer_;
                            this->targets_.push_back(new_pos);
                        }
                    } else if (!map.isOccupied(new_pos) || map.getFromCell(new_pos)) {
                        this->targets_.push_back(new_pos);
                    }
                } else if (Object *obj = map.getFromCell(new_pos);
                    obj && obj->getType() == spell->targetType_) {
                    this->targets_.push_back(new_pos);
                }
            }
        }
    }
}

void SpellManager::changeTargets(int dx, int dy) {
    if (!this->spells_.empty() && this->targets_.size() > 1) {
        Spell *spell = this->spells_.at(this->keys_[this->chosenSpell_]).first;
        if (dx) {
            this->chosenTarget_ += dx + this->targets_.size();
            this->chosenTarget_ %= this->targets_.size();
            if (this->targets_[this->chosenTarget_].y_ !=
                this->targets_[(this->chosenTarget_ + spell->area_ -
                                this->areaDx0_ - this->areaDx1_ - 1) % this->targets_.size()].y_
                || this->targets_[this->chosenTarget_].y_ + spell->area_ - this->areaDy0_ -
                this->areaDy1_ - 1 > this->targets_[this->targets_.size() - 1].y_) {
                this->changeTargets(dx, 0);
            }
        }
        if (dy) {
            int x = this->targets_[this->chosenTarget_].x_;
            this->changeTargets(dy, 0);
            while (this->targets_[this->chosenTarget_].x_ != x) {
                this->changeTargets(dy, 0);
            }
        }
    }
}

void SpellManager::removeSpell(const int index) {
    if (index >= 0 && index < this->keys_.size()) {
        this->spells_count_--;
        this->spells_.at(this->keys_[index]).second -= 1;
        if (this->spells_.at(this->keys_[index]).second < 1) {
            delete this->spells_.at(this->keys_[index]).first;
            this->spells_.erase(this->keys_[index]);
            this->keys_.erase(this->keys_.begin() + index);
            if (this->chosenSpell_ >= this->keys_.size()) {
                this->changeSpell(-1);
            } else {
                this->updateTargets();
            }
        }
    }
}

int SpellManager::castSpell() {
    Spell *spell = this->spells_.at(this->keys_[this->chosenSpell_]).first;
    if (this->mana_ >= spell->cost_ && !this->targets_.empty()) {
        this->level_ = 0;
        this->targets_.at(this->chosenTarget_).x_ -= this->areaDx0_;
        this->targets_.at(this->chosenTarget_).y_ -= this->areaDy0_;
        spell->spellCast(this->targets_.at(this->chosenTarget_), *this->glogic_);
        this->removeSpell(this->chosenSpell_);
        this->changeLevel(0);
        this->changeMana(-spell->cost_);
        return 0;
    }
    return 1;
}

bool SpellManager::haveSpells() const {
    return !this->spells_.empty();
}

void SpellManager::addRandomSpell() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int>
            spell_dist(0, 4);

    const int sp = spell_dist(gen);
    this->addSpell(this->factory.create(known_spells_[sp]));
}

void SpellManager::removeRandomSpells() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    int del_sp = this->spells_count_ / 2;
    for (int i = 0; i < del_sp; i++) {
        std::uniform_int_distribution<int> spell_dist(0,
                                                      static_cast<int>(this->keys_.size()) - 1);
        this->removeSpell(spell_dist(gen));
    }
}

void SpellManager::saveToJson(nlohmann::json *savefile) {
    if (!savefile) return;

    (*savefile)["mana"] = this->mana_;
    (*savefile)["max_mana"] = this->max_mana_;
    (*savefile)["level"] = this->level_;

    /*nlohmann::json known = nlohmann::json::array();
    for (int i = 5; i < this->known_spells_.size(); i++) {
        auto &name = this->known_spells_[i];
        known.push_back(name);
    }
    (*savefile)["known_spells"] = known;
    */

    nlohmann::json spells = nlohmann::json::array();
    for (const auto &name: this->keys_) {
        const auto &entry = this->spells_.at(name);
        nlohmann::json s;
        s["name"] = name;
        s["count"] = entry.second;
        spells.push_back(s);
    }
    (*savefile)["spells"] = spells;
}

void SpellManager::loadFromJson(nlohmann::json *savefile) {
    if (!savefile) return;

    this->mana_ = savefile->value("mana", this->mana_);
    this->max_mana_ = savefile->value("max_mana", this->max_mana_);
    this->level_ = savefile->value("level", this->level_);

    /*if (savefile->contains("known_spells") && (*savefile)["known_spells"].is_array()) {
        for (const auto &v: (*savefile)["known_spells"]) {
            this->known_spells_.push_back(v.get<std::string>());
        }
    }*/

    if (savefile->contains("spells") && (*savefile)["spells"].is_array()) {
        for (const auto &s: (*savefile)["spells"]) {
            std::string name = s["name"];
            int count = s["count"];

            Spell *spell = this->factory.create(name);
            if (!spell) {
                continue;
            }

            this->addSpell(spell);
            this->spells_[name].second = count;
            this->spells_count_ += count - 1;
        }
    }
}
