#pragma once
#include <string>
#include <vector>
#include "nlohmann/json.hpp"
using json = nlohmann::json;

class Game;
class Spell {
public:
    enum class Type { Direct, Area, Trap };
protected:
    int damage;
    int range;
    Type type;
public:
    Spell(int d, int r, Type t) : damage(d), range(r), type(t) {}
    virtual ~Spell() = default;
    int get_damage() { return damage; }
    int get_range() { return range; }
    Type get_type() { return type; }
    virtual std::string get_type_str();
    virtual bool use(Game& game, std::pair<int, int> caster, std::pair<int, int> target);
    float attack() { return damage; }
    bool checkTarget(std::pair<int, int> caster, std::pair<int, int> target);

    virtual json to_json();
    static Spell* from_json(json& j);
};
