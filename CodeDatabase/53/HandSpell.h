#ifndef HANDSPELL_H
#define HANDSPELL_H

#include "ISpell.h"
#include "MegaShotGun.h"
#include "BulletRain.h"
#include "CheapWhiskey.h"
#include "GhostJhonny.h"
#include "UpdateSpell.h"
#include "Constants.h"
#include <memory>

class HandSpell {
private:
    int cntMegaShotgun;
    int cntBulletRain;
    int cntCheapWhiskey;
    int cntJohnnysGhost;
    int cntUpdateSpell;
    
    int cntTotalSpells;
    int maxSizeHandSpell;
    
    std::unique_ptr<MegaShotGun> megaShotgun;
    std::unique_ptr<BulletRain> bulletRainSpell;
    std::unique_ptr<CheapWhiskey> cheapWhiskey;
    std::unique_ptr<GhostJhonny> ghostJhonnySpell;
    std::unique_ptr<UpdateSpell> updateSpell;

public:
    HandSpell(int sizeHandSpell);
    

    bool useSpell(SpellID spellID, int x, int y, GameField& field, Player& player);
    bool addSpell(SpellID spellID, int count);
    void removeRandomSpells(int count);
    
    
    // Геттеры
    MegaShotGun* getMegaShotgun() const;
    BulletRain* getBulletRain() const;
    CheapWhiskey* getCheapWhiskey() const;
    GhostJhonny* getGhostJhonny() const;
    UpdateSpell* getUpdateSpell() const;
    
    int getCntMegaShotgun() const;
    int getCntBulletRain() const;
    int getCntCheapWhiskey() const;
    int getCntJohnnysGhost() const;
    int getCntTotalSpells() const;
    int getCntUpdateSpell() const;
    int getMaxSizeHandSpell() const;

    // Сеттеры
    void setCntMegaShotgun(int count);
    void setCntBulletRain(int count);
    void setCntCheapWhiskey(int count);
    void setCntJohnnysGhost(int count);
    void setCntTotalSpells(int count);
    void setCntUpdateSpell(int count);
    void setMaxSizeHandSpell(int size);
    void setSpellLevel(int level);
};

#endif