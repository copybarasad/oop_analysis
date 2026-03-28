#include "Hand.h"

Hand::Hand(int maxsize) : maxSize(maxsize) {
    srand(static_cast<unsigned>(time(nullptr)));
    addRandomSpell();
}

bool Hand::isFull() const {
    return spells.size() >= maxSize;
}

int Hand::getSize() const {
    return spells.size();
}

void Hand::addSpell(std::unique_ptr<Spell> spell) {
    if (isFull()) {
        std::cout << "Рука заполнена!\n";
        return;
    }
    spells.push_back(std::move(spell));
    std::cout << "Добавлено заклинание: " << spells.back()->getName() << "\n";
}

void Hand::showSpells() const {
    std::cout << "Заклинания в руке:\n";
    if (spells.empty()) {
        std::cout << " (нет заклинаний)\n";
        return;
    }
    for (size_t i = 0; i < spells.size(); i++) {
        std::cout << i + 1 << ") " << spells[i]->getName() << "\n";
    }
}

void Hand::useSpell(int index, Player& player, std::vector<Enemy>& enemies, Field& field) {
    if (index < 1 || index > (int)spells.size()) {
        std::cout << "Неверный номер заклинания!\n";
        return;
    }

    std::cout << "Используется заклинание: " << spells[index - 1]->getName() << "\n";
    spells[index - 1]->cast(player, enemies, field);
    spells.erase(spells.begin() + (index - 1));
}

void Hand::spellBounty(int turnCount) {
    if (turnCount > 0 && turnCount % 10 == 0) {
        std::cout << "Награда за " << turnCount << " ходов! ";
        
        if (!isFull()) {
            std::cout << "Вы получаете заклинание!\n";
            addRandomSpell();
        } else {
            std::cout << "Рука заполнена, заклинание не получено!\n";
        }
    }
}

void Hand::addRandomSpell() {
    int r = rand() % 4;
    switch (r) {
        case 0: addSpell(std::make_unique<Fireball>()); break;
        case 1: addSpell(std::make_unique<Heal>()); break;
        case 2: addSpell(std::make_unique<Lighting>()); break;
        case 3: addSpell(std::make_unique<Trap>()); break;
    }
}

void Hand::removeSpell(int index) {
    if (index >= 1 && index <= (int)spells.size()) {
        std::cout << "Удалено заклинание: " << spells[index - 1]->getName() << "\n";
        spells.erase(spells.begin() + (index - 1));
    }
}