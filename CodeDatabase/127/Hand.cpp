#include "Hand.h"
#include "Spell.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "BuffSpell.h"
#include "Game.h"
#include "Player.h"

#include <random>
#include <iostream>
#include <string>
#include <memory>
#include <algorithm>

static std::mt19937 handRng(std::random_device{}());

Hand::Hand(int capacity) : capacity_(capacity) {}

int Hand::size() const {
    return static_cast<int>(spells_.size());
}

bool Hand::addSpell(std::unique_ptr<Spell> s) {
    if (!s) return false;
    if (static_cast<int>(spells_.size()) >= capacity_) return false;
    spells_.push_back(std::move(s));
    return true;
}

bool Hand::drawRandomSpell() {
    if (static_cast<int>(spells_.size()) >= capacity_) return false;
    std::uniform_int_distribution<int> dist(0, 4);
    int r = dist(handRng);
    std::unique_ptr<Spell> p;
    switch (r) {
    case 0: p = std::make_unique<DirectDamageSpell>(); break;
    case 1: p = std::make_unique<AreaDamageSpell>(); break;
    case 2: p = std::make_unique<TrapSpell>(); break;
    case 3: p = std::make_unique<SummonSpell>(); break;
    default: {
        SpellEnhancement enh;
        enh.extraDamage = 10;
        p = std::make_unique<BuffSpell>(enh);
        break;
    }
    }
    if (!p) return false;
    spells_.push_back(std::move(p));
    return true;
}

bool Hand::buySpell(std::unique_ptr<Spell> s, Player& player) {
    if (!s) return false;
    int cost = s->cost();
    if (!player.spendScore(cost)) return false;
    if (!addSpell(std::move(s))) {
        player.addScore(cost);
        return false;
    }
    return true;
}

static std::string formatEnhancement(const SpellEnhancement& enh) {
    std::string out;
    if (enh.extraDamage != 0) out += "+Урон:" + std::to_string(enh.extraDamage) + " ";
    if (enh.extraRange != 0)  out += "+Радиус:" + std::to_string(enh.extraRange) + " ";
    if (enh.extraArea != 0)   out += "+Область:" + std::to_string(enh.extraArea) + " ";
    if (enh.extraSummons != 0)out += "+Призыв:" + std::to_string(enh.extraSummons) + " ";
    if (!out.empty() && out.back() == ' ') out.pop_back();
    return out;
}

bool Hand::useSpell(int index, Game& game, Player& player, int tx, int ty, std::string* errorMessage) {
    if (index < 0 || index >= static_cast<int>(spells_.size())) {
        if (errorMessage) *errorMessage = "Неверный индекс заклинания.";
        return false;
    }
    if (!spells_[index]) {
        if (errorMessage) *errorMessage = "Заклинание под индексом " + std::to_string(index) + " пустое.";
        return false;
    }

    Spell* spell = spells_[index].get();
    if (!spell) {
        if (errorMessage) *errorMessage = "Внутренняя ошибка: spell указывает на nullptr.";
        return false;
    }

    const char* rawName = spell->name();
    std::string spellName = rawName ? std::string(rawName) : std::string("<unnamed>");

    SpellEnhancement pending = player.getPendingEnhancement();
    bool hasEnh = (pending.extraDamage != 0 || pending.extraRange != 0 ||
        pending.extraArea != 0 || pending.extraSummons != 0);

    bool used = spell->apply(game, player, tx, ty, pending, errorMessage);

    if (used) {
        spells_.erase(spells_.begin() + index);
        player.clearPendingEnhancement();
        // Сообщения об успешном использовании заклинания выводятся через рендерер или логирование
    }
    else {
        if (errorMessage) *errorMessage = "Заклинание '" + spellName + "' не удалось применить.";
    }
    return used;
}

void Hand::printSpellsWithDescription() const {
    std::cout << "Рука (" << spells_.size() << "):\n";
    for (int i = 0; i < static_cast<int>(spells_.size()); ++i) {
        const auto& up = spells_[i];
        if (!up) {
            std::cout << "  [" << i << "]: <пустое> - <неизвестно>\n";
            continue;
        }
        const Spell* sp = up.get();
        if (!sp) {
            std::cout << "  [" << i << "]: <ошибка> - <неизвестно>\n";
            continue;
        }
        const char* rawName = sp->name();
        std::string name = rawName ? std::string(rawName) : std::string("<unnamed>");
        std::string desc = sp->getDescription();
        std::cout << "  [" << i << "]: " << name << " - " << desc << "\n";
    }
}

const std::vector<std::unique_ptr<Spell>>& Hand::getSpells() const {
    return spells_;
}

void Hand::removeSpellAt(int index) {
    if (index < 0 || index >= static_cast<int>(spells_.size())) return;
    spells_.erase(spells_.begin() + index);
}

void Hand::removeRandomHalf() {
    int n = static_cast<int>(spells_.size());
    if (n <= 1) {
        // ничего не удаляем, оставляем 0 или 1
        if (n == 1) {
            // оставляем одну карту 
            return;
        }
        return;
    }
    int toRemove = n / 2;
    std::uniform_int_distribution<int> dist(0, n - 1);
    for (int i = 0; i < toRemove; ++i) {
        if (spells_.empty()) break;
        int idx = dist(handRng) % static_cast<int>(spells_.size());
        spells_.erase(spells_.begin() + idx);
    }
}
