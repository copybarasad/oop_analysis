#ifndef RPG_SPELL_H
#define RPG_SPELL_H

#include "ISpell.h"

class Spell : public ISpell {
    unsigned int castDiameter;
    unsigned int cooldown;
    unsigned int reloadTimer;
    unsigned int level;
public:
    Spell(unsigned int castDiameter, unsigned int cooldown);

    explicit Spell(json &data);

    ~Spell() override = default;

    [[nodiscard]] json to_json() const override;

    bool reload() override;

    void fullReload() override;

    [[nodiscard]] bool isReady() const override;

    void used();

    [[nodiscard]] int getCastDiameter() const override;

    [[nodiscard]] int getCooldown() const;

    [[nodiscard]] int getReloadLeft() const override;

    [[nodiscard]] int getLevel() const override;

    void setLevel(int newLevel) override;

    [[nodiscard]] std::pair<int, int> visualData() const override;

    [[nodiscard]] bool allCellsInRange(std::pair<int,int>& target) const override;
};

#endif
