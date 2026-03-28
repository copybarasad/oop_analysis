#include "ActorAttributes.hpp"


Attributes::Attributes(int intelligence, int dexterity, int strength) {
    this->intelligence = intelligence;
    this->dexterity = dexterity;
    this->strength = strength;
}


Attributes::Attributes(): Attributes(10,10,10) {};


Attributes::~Attributes() {
}


int Attributes::getIntelligence() const {
    return intelligence;
}


int Attributes::getDexterity() const {
    return dexterity;
}


int Attributes::getStrength() const {
    return strength;
}


void Attributes::setIntelligence(int newInt) {
    intelligence = newInt;
}


void Attributes::setDexterity(int newDex) {
    dexterity = newDex;
}


void Attributes::setStrength(int newStr) {
    strength = newStr;
}


AttributesSaveData Attributes::getAttributesSavedata() {
    AttributesSaveData data;
    data.dexterity = this->dexterity;
    data.intelligence = this->intelligence;
    data.strength = this->strength;
    return data;
}


void Attributes::setAttributesSavedata(AttributesSaveData data) {
    this->dexterity = data.dexterity;
    this->intelligence = data.intelligence;
    this->strength = data.strength;
}