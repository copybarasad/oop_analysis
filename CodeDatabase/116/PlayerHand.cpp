#include "PlayerHand.h"

PlayerHand::PlayerHand(int maxSpells):maxCapacity(maxSpells){
    spells.resize(5);
}

PlayerHand::~PlayerHand() = default;

ISpell* PlayerHand::getSpell(int index) {
    if(spells[index].size() != 0){
        return spells[index][0].get();
    }
    return nullptr;
}

bool PlayerHand::isFull(){
    return maxCapacity == getCurrentSize();
}


void PlayerHand::addSpell(std::unique_ptr<ISpell> spell, int index){
    spells[index].push_back(std::move(spell));
}

void PlayerHand::removeSpell(int index){
    if (index >= 0 && index < spells.size() && !spells[index].empty()) {
        spells[index].pop_back();
    }
}
int PlayerHand::getCurrentSize(){
    int c = 0;
    for(int i = 0; i < 5; i++){
        c += spells[i].size();
    }
    return c;
}

int PlayerHand::getMaxSize(){
    return maxCapacity;
}

int PlayerHand::getSpellNum(int index){
    return spells[index].size();
}

void PlayerHand::clearAndResize() {
    for (auto& slot : spells) {
        slot.clear();
    }
    spells.resize(5);
}