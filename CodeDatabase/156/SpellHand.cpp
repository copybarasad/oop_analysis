//
// Created by Artem on 24.10.2025.
//

#include "SpellHand.h"
//#include "../../../../../Exceptions/UnexpectedBehaviorException.h"
#include "../SpellOnCoords.h"
#include "../../../../../GameSetup/Utils/JsonParser.h"
#include "../../../../../Factories/SpellFactory.h"
#include "../../../../../Logger/Logger.h"
#include <iostream>

const std::vector<std::unique_ptr<Spell>>& SpellHand::getSpells() const {
    return this->spells;
}

void SpellHand::addSpell(std::unique_ptr<Spell> spell) noexcept {
    this->spells.emplace_back(std::move(spell));
}

void SpellHand::useSpellWithoutAnyCoordsBinding(int position) {
    this->checkSpellPositionAvailability(position);
    switch (this->spells.at(position).get()->getSpellType()) {
        case SpellType::BuffNextUsedSpell:
            this->gradeLevel++;
            this->spells.at(position).release();
            this->spells.erase(this->spells.begin() + position);
            {
                std::stringstream ss;
                ss << "Использовано заклинание улучшения, уровень следующего заклинания: " << (this->gradeLevel + 1) << std::endl;
                Logger::info(ss.str());
            }
        case SpellType::DirectDamageSpell:
        case SpellType::AreaDamageSpell:
        case SpellType::SummoningSpell:
        case SpellType::CreateTrapSpell:
            throw UnexpectedBehaviorException("Выбран не правильный метод для использования заклинания этого типа");
    }
}

void SpellHand::useSpellWithAIMBinding(int position, Field& field, Constants::XYPair from, Constants::XYPair to) {
    this->checkSpellPositionAvailability(position);
    switch (this->spells.at(position).get()->getSpellType()) {
        case SpellType::DirectDamageSpell:
        case SpellType::AreaDamageSpell:
        case SpellType::SummoningSpell:
        case SpellType::CreateTrapSpell:
        {
            SpellOnCoords* spell = dynamic_cast<SpellOnCoords*>(this->spells.at(position).get());
            spell->castSpell(this->gradeLevel, field, from, to);
            this->gradeLevel = 1;
            this->spells.at(position).release();
            this->spells.erase(this->spells.begin() + position);
        }
            break;
        case SpellType::BuffNextUsedSpell:
            throw UnexpectedBehaviorException("Выбран не правильный метод для использования заклинания этого типа");
    }

}

bool SpellHand::checkSpellPositionAvailability(int position) const {
    if (position < this->spells.size() && position >= 0){
        return true;
    }else{
        throw UnexpectedBehaviorException("Позиция заклинания выходит за рамки руки?? чтобы это не значило, я уже говорить разучился абоба!");
    }
}

const std::map<SpellType, std::string> enumTypeToStrType = {
        {SpellType::DirectDamageSpell, "DirectDamageSpell"},
        {SpellType::BuffNextUsedSpell, "BuffNextUsedSpell"},
        {SpellType::AreaDamageSpell, "AreaDamageSpell"},
        {SpellType::SummoningSpell, "SummoningSpell"},
        {SpellType::CreateTrapSpell, "CreateTrapSpell"}
};
std::string SpellHand::serialize() noexcept {
    std::string res = "{";
    res += "gradeLevel:";
    res+= std::to_string(this->gradeLevel) + ",";
    res+="spells:[";
    for (int i = 0; i < this->spells.size(); ++i) {
        res += enumTypeToStrType.at(this->spells.at(i)->getSpellType());
        if (i != this->spells.size() - 1)
            res += ",";
    }
    res+= "]}";
    return res;
}

const std::map<std::string, SpellType> strTypeToEnumType = {
        {"DirectDamageSpell", SpellType::DirectDamageSpell},
        {"BuffNextUsedSpell", SpellType::BuffNextUsedSpell},
        {"AreaDamageSpell", SpellType::AreaDamageSpell},
        {"SummoningSpell", SpellType::SummoningSpell},
        {"CreateTrapSpell", SpellType::CreateTrapSpell}
};
SpellHand SpellHand::deserialize(std::map<std::string, std::string> json, SpellFactory& spellFactory) noexcept {
    int gradeLevel = std::stoi(json.at("gradeLevel"));
    std::vector<std::string> vec = JsonParser::parseJsonArray(json.at("spells"));
    std::vector<std::unique_ptr<Spell>> spells;
    for(const std::string& sp : vec){
        spells.push_back(spellFactory.createSpell(strTypeToEnumType.at(sp)));
    }
    return SpellHand(gradeLevel, std::move(spells));
}
