#ifndef INCLUDE_SPELL
#define INCLUDE_SPELL

#include <string>
#include <vector>
#include "../ui/direction.h"

enum class SpellType {
    Heal,
    Damage,
    Splash,
    Trap,
    Summon,
    Enhance
};

class Spell {
protected:
    SpellType type_;
    std::string name_;
    std::string description_;
    int cost_;

public:
    Spell(SpellType type, const std::string& name, const std::string& description, int cost);
    virtual ~Spell() = default;

    SpellType type() const { return type_; }
    std::string name() const { return name_; }
    std::string description() const { return description_; }
    int cost() const { return cost_; }

    virtual void Use(class GameBoard& board) = 0;
    virtual Spell* Clone() const = 0;
};

class HealSpell : public Spell {
private:
    int heal_amount_;

public:
    HealSpell(int heal_amount = 30, int cost = 50);
    void Use(GameBoard& board) override;
    Spell* Clone() const override;
};

class DamageSpell : public Spell {
private:
    int damage_;

public:
    DamageSpell(int damage = 40, int cost = 75);
    void Use(GameBoard& board) override;
    void UseWithTarget(GameBoard& board, int target_index);
    std::vector<int> GetTargetsInRange(const GameBoard& board) const;
    Spell* Clone() const override;
};

class SplashSpell : public Spell {
private:
    int damage_;
    int width_;
    int height_;

public:
    SplashSpell(int damage = 30, int width = 3, int height = 2, int cost = 100);
    void Use(GameBoard& board) override;
    void UseInDirection(GameBoard& board, Direction dir);
    std::vector<class Position> GetAreaPositions(const GameBoard& board, Direction dir) const;
    Spell* Clone() const override;
};

class TrapSpell : public Spell {
private:
    int damage_;

public:
    TrapSpell(int damage = 50, int cost = 60);
    void Use(GameBoard& board) override;
    Spell* Clone() const override;
    int damage() const { return damage_; }
};

class SummonSpell : public Spell {
private:
    int summon_count_;

public:
    SummonSpell(int summon_count = 1, int cost = 80);
    void Use(GameBoard& board) override;
    Spell* Clone() const override;
    int summon_count() const { return summon_count_; }
};

class EnhanceSpell : public Spell {
public:
    EnhanceSpell(int cost = 40);
    void Use(GameBoard& board) override;
    Spell* Clone() const override;
};

#endif
