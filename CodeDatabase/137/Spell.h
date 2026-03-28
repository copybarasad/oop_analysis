#ifndef SPELL_H
#define SPELL_H

#include <string>
#include <vector>
#include <memory>

class GameField;
class Player;
class Enemy;
class EnemyBuilding;
class EnemyTower;
class Ally;

enum class SpellType {
    DIRECT_DAMAGE,
    AREA_DAMAGE,
    TRAP,
    SUMMON,
    ENHANCEMENT
};

class Spell {
public:
    Spell(const std::string& name, int mana_cost, int range, SpellType type);
    virtual ~Spell();

    std::string GetName() const;
    int GetManaCost() const;
    int GetRange() const;
    SpellType GetType() const;
    int GetLevel() const;

    virtual bool CanCast(int player_x, int player_y, int target_x, int target_y, 
                         GameField& field) const = 0;
    virtual void Cast(int player_x, int player_y, int target_x, int target_y,
                      GameField& field, Player& player,
                      std::vector<std::unique_ptr<Enemy>>& enemies,
                      std::vector<std::unique_ptr<EnemyBuilding>>& enemy_buildings,
                      std::vector<std::unique_ptr<EnemyTower>>& enemy_towers,
                      std::vector<std::unique_ptr<Ally>>& allies) = 0;
    virtual void Enhance() = 0;
    virtual Spell* Clone() const = 0;

protected:
    std::string name_;
    int mana_cost_;
    int range_;
    SpellType type_;
    int level_;
};

#endif // SPELL_H
