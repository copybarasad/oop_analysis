//
// Created by Artem on 30.10.2025.
//

#include "SummoningSpell.h"
#include <vector>
#include "../../../../Exceptions/UniversalStringException.h"
#include "../../../../Logger/Logger.h"
#include <iostream>

void SummoningSpell::castSpell(int gradeLevel, Field &field, Constants::XYPair from, Constants::XYPair to) const {
    Logger::info("Применяем заклинание призыва!");
    int allyCount = this->levels.at(gradeLevel < this->levels.size() ? gradeLevel : (this->levels.size() - 1));

    std::vector<Constants::dxdy> dxdys = Constants::dxdys;
    dxdys.push_back({0, 0 });

    while(!dxdys.empty() && allyCount > 0){
        int pos = rand() % dxdys.size();
        Constants::dxdy dxdy = dxdys.at(pos);
        dxdys.erase(dxdys.begin() + pos);
        try{
            this->spawner.createAlly(to.x + dxdy.x, to.y + dxdy.y);
            allyCount--;
        }catch (UniversalStringException &e){
            continue;
        }
    }
    if (allyCount != 0){
        Logger::info("Не удалось заспавнить всех союхников в выбранных координатах, не хватает места!");
    }
}

std::unique_ptr<Spell> SummoningSpell::clone() const {
    return std::make_unique<SummoningSpell>(*this);
}
