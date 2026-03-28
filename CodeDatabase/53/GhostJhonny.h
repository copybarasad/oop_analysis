#ifndef GHOSTJHONNY_H
#define GHOSTJHONNY_H

#include "SpellBase.h"
#include "IUpgradableSpell.h"
#include "ISummonSpell.h"

class GhostJhonny : public SpellBase, public IUpgradableSpell, public ISummonSpell {
private:
    int maxCntTeamates;
    int curCntTeamates;
    std::string teamateName;
    int teamateHealth;
    int teamateDamage;
    int rangeSpawn;
    
public:
    GhostJhonny(int level);
    
    bool use(int x, int y, GameField& field, Player& player) override;
    
    void upgrade() override;
    bool canUpgrade() override;
    void resetLevel() override;
    
    bool canSpawn(int x, int y, const Player& player, GameField& field) override;
    
    // Геттеры
    int getMaxSpawns() const override;
    int getCurrentSpawns() const override;

    // Сеттеры
    void setCurrentSpawns(int curSpawn) override;
};

#endif