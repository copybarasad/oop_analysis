#include "hand.h"
#include "direct_damage_spell.h"
#include "area_damage_spell.h"
#include "trap_spell.h"
#include "field.h"
#include "player.h"
#include "enemy.h"
#include <iostream>
using namespace std;

Hand::Hand(int maxSize) : maxSize(maxSize) {
    /*cards.push_back(new DirectDamageSpell());
    cards.push_back(new AreaDamageSpell());
    cards.push_back(new TrapSpell());*/
}

Hand::~Hand() {
    for (Spell* spell : cards) {
        delete spell;
    }
    cards.clear();
}

bool Hand::addSpell(Spell* spell) {
    if (cards.size() < static_cast<size_t>(maxSize)) {
        cards.push_back(spell);
        return true;
    } else {
        cout << "Рука переполнена!" << endl;
        delete spell;
        return false;
    }
}

bool Hand::removeSpell(int index) {
    if (index < 0 || index >= static_cast<int>(cards.size())) {
        return false;
    }
    delete cards[index];
    cards.erase(cards.begin() + index);
    return true;
}

int Hand::useSpell(int idx, int targetX, int targetY, int enemyX, int enemyY, Field& field, Player& player) {
    if (idx < 0 || idx >= static_cast<int>(cards.size())) {
        cout << "Неверный индекс заклинания!" << endl;
        return 0;
    } else {
        int result = cards[idx]->cast_spell(targetX, targetY, enemyX, enemyY, field, player);
        return result;
    }
}

int Hand::getSpellCount() const { 
    return static_cast<int>(cards.size()); 
}

bool Hand::isFull() const { 
    return cards.size() >= static_cast<size_t>(maxSize); 
}

bool Hand::isEmpty() const { 
    return cards.empty(); 
}

int Hand::getMaxSize() const { 
    return maxSize; 
}

void Hand::showHand() const {
    cout << "Заклинания в руке" << endl;
    cout << "Размер: " << cards.size() << "/" << maxSize << endl;
    
    if (isEmpty()) {
        cout << "Рука пуста!" << endl;
        return;
    }
    
    for (size_t i = 0; i < cards.size(); i++) {
        Spell* spell = cards[i];
        cout << i << ". " << spell->getName() 
             << " - стоимость: " << spell->getCost() 
             << ", радиус: " << spell->getRange() << endl;
    }
}

Spell* Hand::getSpell(int index) const {
    if (index < 0 || index >= static_cast<int>(cards.size())) {
        return nullptr;
    }
    return cards[index];
}

const vector<Spell*>& Hand::getAllSpells() const {
    return cards;
}

bool Hand::buySpell(int& playerPoints) {
    if (isFull()) {
        cout << "Рука переполнена!" << endl;
        return false;
    }
    
    cout << "Доступные к покупке заклинания:" << endl;
    cout << "0. Магическая стрела - урон: " << constants::DIRECT_SPELL_POWER << ", стоимость: " << constants::DIRECT_SPELL_COST << ", радиус: " << constants::DIRECT_SPELL_RADIUS << endl;
    cout << "1. Огненный шар - урон: " << constants::AREA_SPELL_POWER << ", стоимость: " << constants::AREA_SPELL_COST << ", радиус: " << constants::AREA_SPELL_RADIUS << endl;
    cout << "2. Магическая ловушка - урон: " << constants::TRAP_SPELL_POWER << ", стоимость: " << constants::TRAP_SPELL_COST << ", радиус: " << constants::TRAP_SPELL_RADIUS << endl;
    cout << "Ваши очки: " << playerPoints << endl;
    cout << "Введите индекс заклинания для покупки: ";
    
    int spellIndex;
    cin >> spellIndex;
    cin.ignore();
    
    Spell* newSpell = nullptr;
    
    switch(spellIndex) {
        case 0:
            newSpell = new DirectDamageSpell();
            break;
        case 1:
            newSpell = new AreaDamageSpell();
            break;
        case 2:
            newSpell = new TrapSpell();
            break;
        default:
            cout << "Неверный индекс заклинания!" << endl;
            return false;
    }
    
    if (playerPoints >= newSpell->getCost()) {
        if (addSpell(newSpell)) {
            playerPoints -= newSpell->getCost();
            cout << "Вы купили заклинание: " << newSpell->getName() << "!" << endl;
            cout << "Осталось очков: " << playerPoints << endl;
            return true;
        }
    } else {
        cout << "Недостаточно очков! Нужно: " << newSpell->getCost() << ", есть: " << playerPoints << endl;
        delete newSpell;
        return false;
    }
    return false;
}
