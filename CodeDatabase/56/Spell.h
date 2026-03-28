#ifndef SPELL_H
#define SPELL_H

#include <string>
#include <nlohmann/json.hpp>

class Spell{
protected:
    int type;
    int mana_cost;
    std::string name;
    int damage;
public:
    Spell(int spell_type, std::string spell_name, int cost, int dmg);

    std::string getName();
    int getManaCost();
    int getType();
    virtual nlohmann::json toJson() const;
    virtual void fromJson(const nlohmann::json& j);
    virtual std::unique_ptr<Spell> clone() const = 0;
    virtual int getDamage() const { return damage; }
    virtual void setDamage(int dmg) { damage = dmg; }

};

#endif