#include <iostream>
#include <vector>
#include "hand.h"
#include "console_renderer.h"
#include "spellitems.h"
using namespace std;
    
void Hand::clearHand() {
    for (Spell* spell : cards) {
        delete spell;
    }
    cards.clear();
}
Hand::~Hand() {
        clearHand();
    }
bool Hand::addSpell(Spell* spell, string& message) {
    if (cards.size() < maxSize) {
        cards.push_back(spell);
        return true;
    } else {
        message = "Рука переполнена!";
        delete spell;
        return false;
    }
}
bool Hand::addSpell(Spell* spell) {
    if (cards.size() < maxSize){
        cards.push_back(spell);
        return true;
    } else {
        delete spell;
        return false;
    }
}

bool Hand::removeSpell(int index) {
    if (index < 0 || index >= cards.size()) return false;
    delete cards[index];
    cards.erase(cards.begin() + index);
    return true;
}
bool Hand::useSpell(int idx, int x, int y, Field& field, Player& player, Enemy& enemy, string& message) {
    if (isEmpty()) {
        return false;
    }
    if (idx < 0 || idx >= cards.size()) {
        message = "Неверный индекс заклинания!";
        return false;
    } else {
        string spellMessage;
        bool result = cards[idx]->do_spell(x, y, field, player, enemy, spellMessage);
        message = spellMessage;
        if (result) {
            string spellName = cards[idx]->getName();
            removeSpell(idx);
            return true;
        } else {
            return false;
        }
    }
}
int Hand::getSpellCount() const { 
    return cards.size(); 
}

bool Hand::isFull() const { 
    return cards.size() >= maxSize; 
}

bool Hand::isEmpty() const { 
    return cards.empty(); 
}

int Hand::getMaxSize() const { 
    return maxSize; 
}
void Hand::showHand(string& outputMessage) const {
    stringstream ss;
    ss << "=== Заклинания в руке ===" << endl;
    
    if (cards.empty()) {
        ss << "Рука пуста" << endl;
    } else {
        for (int i = 0; i < cards.size(); i++) {
            ss << i << ". " << cards[i]->getName() << " - стоимость: " << cards[i]->getCost() << endl;
        }
    }
    
    outputMessage = ss.str();
}
Spell* Hand::getSpell(int index) const {
    if (index < 0 || index >= cards.size()) return nullptr;
    return cards[index];
}

const vector<Spell*>& Hand::getAllSpells() const {
    return cards;
}
bool Hand::buySpell(int spellIndex, int& playerPoints, string& message) {
    if (isFull()) {
        message = "Рука переполнена!";
        return false;
    }
    
    Spell* newSpell = nullptr;
    int cost = 0;
    string spellName;
    
    switch(spellIndex) {
        case 0:
            newSpell = new Fireball();
            cost = 10;
            spellName = "Огненный шар";
            break;
        case 1:
            newSpell = new IceBolt();
            cost = 8;
            spellName = "Ледяная стрела";
            break;
        case 2:
            newSpell = new FireWave();
            cost = 20;
            spellName = "Огненный шторм";
            break;
        case 3:
            newSpell = new IceStorm();
            cost = 15;
            spellName = "Ледяная буря";
            break;
        case 4:
            newSpell = new PoisonTrap();
            cost = 6;
            spellName = "Ядовитая ловушка";
            break;
        default:
            message = "Неверный индекс заклинания!";
            return false;
    }
    
    if (playerPoints >= cost) {
        string addMessage;
        if (addSpell(newSpell, addMessage)) {
            playerPoints -= cost;
            message = "Вы купили заклинание: " + spellName + "!\n" + addMessage;
            return true;
        } else {
            message = addMessage;
            return false;
        }
    } else {
        message = "Недостаточно очков! Нужно: " + to_string(cost) + ", есть: " + to_string(playerPoints);
        delete newSpell;
        return false;
    }
    return false;
}
void Hand::removeHalfSpells() {
    if (cards.empty()) return;
    int spellsToRemove = cards.size() / 2;
    for (int i = 0; i < spellsToRemove; i++) {
        if (cards.empty()) break;
        int randomIndex = rand() % cards.size();     
        delete cards[randomIndex];
        cards.erase(cards.begin() + randomIndex);
    }
}