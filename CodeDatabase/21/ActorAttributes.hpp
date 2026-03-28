#pragma once
#include "../core/SaveData.hpp"

class Attributes {
private:
    int intelligence;
    int dexterity;
    int strength;
public:
    AttributesSaveData getAttributesSavedata();
    void setAttributesSavedata(AttributesSaveData data);
    int getIntelligence() const;
    int getDexterity() const;
    int getStrength() const;
    void setIntelligence(int newInt);
    void setDexterity(int newDex);
    void setStrength(int newStr);
    Attributes(int intelligence, int dexterity, int strength);
    Attributes();
    ~Attributes();
};