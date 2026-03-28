#pragma once
#include <vector>
#include <iostream>
#include "ISpell.h"
#include "FireballSpell.h"
#include "TrapSpell.h"
#include "ExplosionSpell.h"
#include <cstdlib>
#include <ctime>
#include "GameExceptions.h"



class PlayerHand {
private:
    int handCapacity;
    std::vector<ISpell*> spells;  // активные заклинания
    
    std::unique_ptr<FireballSpell> fireballProt;
    std::unique_ptr<TrapSpell> trapProt;
    std::unique_ptr<ExplosionSpell> explosionProt;
    

public:
    PlayerHand(int capacity = 3);

    PlayerHand(PlayerHand&& other) noexcept;
    PlayerHand& operator=(PlayerHand&& other) noexcept;


    void addPrototype(std::unique_ptr<ISpell> spell);
    bool addSpell(ISpell* spell);

    int getCapacity() const { return handCapacity; }
    int getCount() const { return (int)spells.size(); }
    bool isFull() const { return (int)spells.size() >= handCapacity; }

    void showSpellList() const;
    ISpell* getSpell(int index) const;
    bool removeSpell(int index);
    void addRandomSpell();
    void removeRandomHalf();

    FireballSpell& getFireballPrototype() { return *fireballProt; }
    TrapSpell& getTrapPrototype() { return *trapProt; }
    ExplosionSpell& getExplosionPrototype() { return *explosionProt; }

    void incFireballSpellDmg(double dmg) {fireballProt->setDamage(fireballProt->getDamage() + dmg);}
    void incExplosionSpellDmg(double dmg) {explosionProt->setDamage(explosionProt->getDamage() + dmg);}
    void incTrapSpellDmg(double dmg) {trapProt->setDamage(trapProt->getDamage() + dmg);}


    //save and load
    void save(std::ostream& out) const;
    void load(std::istream& in);



    ~PlayerHand();
};
