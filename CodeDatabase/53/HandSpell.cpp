#include <HandSpell.h>
#include <iostream>
using namespace std;
using namespace GameConstants;

HandSpell::HandSpell(int sizeHandSpell)
    : cntMegaShotgun(0), cntBulletRain(0), cntCheapWhiskey(0), cntJohnnysGhost(0),
      cntTotalSpells(0), cntUpdateSpell(0), maxSizeHandSpell(sizeHandSpell) {
    int p = rand() % 5;
    if (p == 0) cntMegaShotgun = 1;
    else if (p == 1) cntBulletRain = 1;
    else if (p == 2) cntCheapWhiskey = 1;
    else if (p == 3) cntJohnnysGhost = 1;
    else cntUpdateSpell = 1;
    cntTotalSpells = 1;
    bulletRainSpell = std::make_unique<BulletRain>(BEGIN_LEVEL_BULLETRAIN);
    megaShotgun = std::make_unique<MegaShotGun>(BEGIN_LEVEL_MEGASHOTGUN);
    cheapWhiskey = std::make_unique<CheapWhiskey>(BEGIN_LEVEL_CHEAPWHISKEY);
    ghostJhonnySpell = std::make_unique<GhostJhonny>(BEGIN_LEVEL_GHOSTJOHNNY);
    updateSpell = std::make_unique<UpdateSpell>(BEGIN_SPELL_LEVEL, MAX_SPELL_LEVEL);
}

bool HandSpell::useSpell(SpellID spellID, int x, int y, GameField& field, Player& player) {
    bool spellUsed = false;
    
    switch (spellID) {
        case MEGASHOTGUN:
            if (cntMegaShotgun > 0) {
                if(megaShotgun->use(x, y, field, player)) {
                    cntMegaShotgun--;
                    cntTotalSpells--;
                    spellUsed = true;
                }
            } else {
                cout << "Нет доступных заклинаний MegaShotgun!" << endl;
            }
            break;
            
        case BULLETARAIN:
            if (cntBulletRain > 0) {
                if(bulletRainSpell->use(x, y, field, player)) {
                    cntBulletRain--;
                    cntTotalSpells--;
                    spellUsed = true;
                }
            } else {
                cout << "Нет доступных заклинаний BulletRain!" << endl;
            }
            break;
            
        case CHEAPWHISKEY:
            if (cntCheapWhiskey > 0) {
                if(cheapWhiskey->use(x, y, field, player)) {
                    cntCheapWhiskey--;
                    cntTotalSpells--;
                    spellUsed = true;
                }
            } else {
                cout << "Нет доступных заклинаний CheapWhiskey!" << endl;
            }
            break;
            
        case GHOSTOFJHONNY:
            if (cntJohnnysGhost > 0) {
                if(ghostJhonnySpell->use(x, y, field, player)) {
                    cntJohnnysGhost--;
                    cntTotalSpells--;
                    spellUsed = true;
                }
            } else {
                cout << "Нет доступных заклинаний JohnnysGhost!" << endl;
            }
            break;
            
        case UPDATESPELL: {
            if(cntUpdateSpell > 0) {
                if(updateSpell->use(x, y, field, player)) {
                    cntUpdateSpell--;
                    cntTotalSpells--;
                    spellUsed = true;
                }
            } else {
                cout << "Нет доступных заклинаний UpdateSpell!" << endl;
            }
            break;
        }
        default:
            cout << "Неизвестное заклинание!" << endl;
            break;
    }
    
    if (spellUsed && spellID != UPDATESPELL && updateSpell->getLevel() > BEGIN_SPELL_LEVEL) {
        UpdateSpell* update = getUpdateSpell();
        if (update) {
            update->resetUpdate(*this);
        }
    }
    
    return spellUsed;
}
bool HandSpell::addSpell(SpellID spellID, int count) {
    if (count <= 0) {
        cout << "Неверное количество заклинаний!" << endl;
        return false;
    }
    
    switch (spellID) {
        case MEGASHOTGUN:
            if(cntTotalSpells + count <= maxSizeHandSpell) {
                cntMegaShotgun += count;  
                cntTotalSpells += count;
                cout << "Добавлено " << count << " заклинаний MegaShotgun!" << endl;
                return true;
            } else { 
                cout << "Превышен лимит заклинаний в руке!" << endl;
                return false;
            }
            break;
            
        case BULLETARAIN:
            if(cntTotalSpells + count <= maxSizeHandSpell) {
                cntBulletRain += count;  
                cntTotalSpells += count;
                cout << "Добавлено " << count << " заклинаний BulletRain!" << endl;
                return true;
            } else { 
                cout << "Превышен лимит заклинаний в руке!" << endl;
                return false;
            }
            break;
            
        case CHEAPWHISKEY:
            if(cntTotalSpells + count <= maxSizeHandSpell) {
                cntCheapWhiskey += count;  
                cntTotalSpells += count;
                cout << "Добавлено " << count << " заклинаний CheapWhiskey!" << endl;
                return true;
            } else { 
                cout << "Превышен лимит заклинаний в руке!" << endl;
                return false;
            }
            break;
            
        case GHOSTOFJHONNY:
            if(cntTotalSpells + count <= maxSizeHandSpell) {
                cntJohnnysGhost += count;  
                cntTotalSpells += count;
                cout << "Добавлено " << count << " заклинаний JohnnysGhost!" << endl;
                return true;
            } else { 
                cout << "Превышен лимит заклинаний в руке!" << endl;
                return false;
            }
            break;
        case UPDATESPELL:
            if(cntTotalSpells + count <= maxSizeHandSpell) {
                cntUpdateSpell += count;  
                cntTotalSpells += count;
                cout << "Добавлено " << count << " заклинаний UpdateSpell!" << endl;
                return true;
            } else { 
                cout << "Превышен лимит заклинаний в руке!" << endl;
                return false;
            }
            break;
        default:
            cout << "Неизвестное заклинание!" << endl;
            return false;
            break;
    }
}


void HandSpell::removeRandomSpells(int count) {
    int removed = 0;
    int totalBefore = getCntTotalSpells();
    
    while (removed < count && getCntTotalSpells() > 0) {
        int spellType = rand() % 5;
        bool spellRemoved = false;
        
        switch (spellType) {
            case 0:
                if (cntMegaShotgun > 0) {
                    cntMegaShotgun--;
                    spellRemoved = true;
                }
                break;
            case 1:
                if (cntBulletRain > 0) {
                    cntBulletRain--;
                    spellRemoved = true;
                }
                break;
            case 2: 
                if (cntCheapWhiskey > 0) {
                    cntCheapWhiskey--;
                    spellRemoved = true;
                }
                break;
            case 3: 
                if (cntJohnnysGhost > 0) {
                    cntJohnnysGhost--;
                    spellRemoved = true;
                }
                break;
            case 4: 
                if (cntUpdateSpell > 0) {
                    cntUpdateSpell--;
                    spellRemoved = true;
                }
                break;
        }
        
        if (spellRemoved) {
            removed++;
            cntTotalSpells--;
        }
    }
    
    std::cout << "Удалено " << removed << " заклинаний." << std::endl;
}

MegaShotGun* HandSpell::getMegaShotgun() const { return megaShotgun.get(); }
BulletRain* HandSpell::getBulletRain() const { return bulletRainSpell.get(); }
CheapWhiskey* HandSpell::getCheapWhiskey() const { return cheapWhiskey.get(); }
GhostJhonny* HandSpell::getGhostJhonny() const { return ghostJhonnySpell.get(); }
UpdateSpell* HandSpell::getUpdateSpell() const { return updateSpell.get(); }

int HandSpell::getCntMegaShotgun() const { return cntMegaShotgun; }
int HandSpell::getCntBulletRain() const { return cntBulletRain; }
int HandSpell::getCntCheapWhiskey() const { return cntCheapWhiskey; }
int HandSpell::getCntJohnnysGhost() const { return cntJohnnysGhost; }
int HandSpell::getCntTotalSpells() const { return cntTotalSpells; }
int HandSpell::getCntUpdateSpell() const { return cntUpdateSpell; }
int HandSpell::getMaxSizeHandSpell() const { return maxSizeHandSpell; }

void HandSpell::setCntMegaShotgun(int count) { cntMegaShotgun = count; }
void HandSpell::setCntBulletRain(int count) { cntBulletRain = count; }
void HandSpell::setCntCheapWhiskey(int count) { cntCheapWhiskey = count; }
void HandSpell::setCntJohnnysGhost(int count) { cntJohnnysGhost = count; }
void HandSpell::setCntTotalSpells(int count) { cntTotalSpells = count; }
void HandSpell::setMaxSizeHandSpell(int size) { maxSizeHandSpell = size; }
void HandSpell::setCntUpdateSpell(int size) { cntUpdateSpell = size; }

void HandSpell::setSpellLevel(int level) { 
    bulletRainSpell = std::make_unique<BulletRain>(level);
    megaShotgun = std::make_unique<MegaShotGun>(level);
    cheapWhiskey = std::make_unique<CheapWhiskey>(level);
    ghostJhonnySpell = std::make_unique<GhostJhonny>(level);
    updateSpell = std::make_unique<UpdateSpell>(level, MAX_SPELL_LEVEL);
}