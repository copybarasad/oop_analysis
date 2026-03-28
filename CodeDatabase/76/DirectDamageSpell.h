    #pragma once
    #include "SpellCard.h"
    #include "Enemy.h"

    class Player;

    class DirectDamageSpell : public SpellCard {
    private:
        string name;
        string description;
        int damage;
        int manaCost;
        int range;
        string targetType;

    public:
        DirectDamageSpell(string spellName = "Fireball", int damage = 10, int mana = 5, int spellRange = 2, string target = "enemy");
        string GetName() const override;
        string GetDescription() const override;
        int GetManaCost() const override;
        int GetDamage() const;
        bool CanCast(const Player& caster, Enemy& enemy) const override;
        void Cast(Player& caster, Enemy& enemy) override;
        string GetTargetType() const { return targetType; }
    };