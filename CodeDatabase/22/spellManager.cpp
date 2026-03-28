#include "managment/spellManager.h"
#include "managment/enemyManager.h"
#include <iostream>

#define FALL Position {-1, -1}

SpellManager::SpellManager() {};



void SpellManager::cast(AreaSpell &spell, Field &field, EnemyManager &enM, Position target)
{

    std::cout << target.x << ' ' << target.y << ' ' << spell.getRadius() << std::endl;

    std::vector<Position> enP = field.enHInRadius(target, spell.getRadius());

    if (enP.empty()){
        field.clearReady();
        return;
    }

    for (auto pos : enP)
    {
        enM.setDamage(pos, spell.use().first);
    }
    
    field.clearReady();
}

void SpellManager::cast(DirectSpell &spell, Field &field, EnemyManager &enM, Position target)
{

    enM.setDamage(target, spell.use().first);
}

void SpellManager::cast(CallSpell &spell, Field &field, EnemyManager &enM, Position target)
{
    Position newPos = field.getPlayer();
    newPos.y++;

    for (int i = 0; i < spell.use().first; i++){
        if(field.isFree(newPos)){
            allies_.push_back(std::make_unique<Ally>(100, 10, newPos));
            field.addAlly(newPos);
        }
        newPos.x++;
    }
}

void SpellManager::cast(TrapSpell &spell, Field &field, EnemyManager &enM, Position target)
{
    traps_.push_back(std::make_unique<TrapSpell>(10, target));
    field.addTrap(target);
}

void SpellManager::cast(ImproveSpell &spell, Field &field, EnemyManager &enM, Hand *hand, int opt)
{
    const auto& spells = hand->getSpells();

    spell.use(*spells[opt-1]);
}



void SpellManager::allyMove(Field& field, EnemyManager &enM){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(-1, 1);

    if (allies_.empty()){
        return;
    }

    int index = 0;
    for (auto &al : allies_)
    {

        if (!al->isAlive())
        {
            field.deliteCharacter(al->getPosition());
            allies_.erase(allies_.begin() + index);
            continue;
        }

        Position enemyPos = field.enemyInRadius(al->getPosition(), 1);

        if (enemyPos != FALL)
        {
            enM.setDamage(enemyPos, al->getDamage());
        }
        else
        {
            Position newPos = al->getPosition();
            newPos.x += dist(gen);
            newPos.y += dist(gen);

            if (field.isFree(newPos))
            {
                field.deliteCharacter(al->getPosition());
                field.addAlly(newPos);
                al->move(newPos);
            }
        }
        index++;
    }
}

const std::vector<std::unique_ptr<Ally>>& SpellManager::getAlly() const{
    return allies_;
}

void SpellManager::addAlly(std::unique_ptr<Ally> al){
    allies_.push_back(std::move(al));
}

void SpellManager::cleanAlly(){
    allies_.clear();
}

int SpellManager::getTrapDamage(){
    return 10;
}

void SpellManager::deliteTrap(Position pos)
{
    auto it = std::remove_if(traps_.begin(), traps_.end(),
                             [&](const std::unique_ptr<TrapSpell> &t)
                             { return t->position() == pos; });
    traps_.erase(it, traps_.end());
}