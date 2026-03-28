#include "Hand.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <random>
#include <chrono>
#include "Player.h"
#include "Enemy.h"
#include "Field.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"

Hand::Hand(int maxSize) : maxSize(maxSize) {}
Hand::~Hand() {}

void Hand::addSpell(std::unique_ptr<Spell> spell) {
    if (spells.size() < maxSize) {
        std::cout << "В руку добавлено новое заклинание: " << spell->getName() << "!" << std::endl;
        spells.push_back(std::move(spell));
    } else {
        std::cout << "Рука переполнена, новое заклинание сброшено." << std::endl;
    }
}

void Hand::useSpell(int index, Player& caster, std::vector<Enemy>& enemies, Field& field) {
    if (index >= 0 && index < spells.size()) {
        std::cout << "Введите координаты цели (X Y) для заклинания '" << spells[index]->getName() << "': ";
        int x, y;
        
        if (!(std::cin >> x >> y)) {
            std::cout << "Некорректный ввод координат." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        spells[index]->cast(caster, enemies, field, x, y);
        spells.erase(spells.begin() + index);
    } else {
        std::cout << "Неверный номер заклинания в руке." << std::endl;
    }
}

size_t Hand::getSpellCount() const {
    return spells.size();
}

void Hand::printHand() const {
    std::cout << "--- Заклинания в руке ---" << std::endl;
    if (spells.empty()) {
        std::cout << "(пусто)" << std::endl;
    } else {
        for (size_t i = 0; i < spells.size(); ++i) {
            std::cout << i << ": " << spells[i]->getName() 
                      << " (цена: " << spells[i]->getCost() 
                      << ", дальность: " << spells[i]->getRange() << ")" << std::endl;
        }
    }
    std::cout << "-------------------------" << std::endl;
}

void Hand::removeRandomSpells() {
    if (spells.empty()) return;

    unsigned seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 g(seed);

    std::shuffle(spells.begin(), spells.end(), g);

    size_t countToRemove = spells.size() / 2;

    if (countToRemove > 0) {
        spells.erase(spells.end() - countToRemove, spells.end());
        std::cout << "Было сброшено " << countToRemove << " случайных заклинаний." << std::endl;
    }
}

void Hand::save(std::ostream& os) const {
    os << spells.size() << std::endl;
    for (const auto& spell : spells) {
        spell->save(os);
    }
}

void Hand::load(std::istream& is) {
    spells.clear();
    size_t count;
    is >> count;
    std::string spellType;
    is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    for (size_t i = 0; i < count; ++i) {
        std::getline(is, spellType);
        std::unique_ptr<Spell> newSpell = nullptr;
        
        if (spellType == "DirectDamageSpell") {
            newSpell = std::make_unique<DirectDamageSpell>("", 0, 0, 0);
        } else if (spellType == "AreaDamageSpell") {
            newSpell = std::make_unique<AreaDamageSpell>("", 0, 0, 0);
        } else if (spellType == "TrapSpell") {
            newSpell = std::make_unique<TrapSpell>("", 0, 0, 0);
        }
        
        if (newSpell) {
            newSpell->load(is);
            spells.push_back(std::move(newSpell));
        }
    }
}