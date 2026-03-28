#ifndef PLAYER_SAVE
#define PLAYER_SAVE

#include "ISave.h"
#include "../GameSystem/Player.h"

class PlayerSave : public ISave
{
private:
    enum class SpellType
    {
        DirectDamage,
        SquareDamage
    };

    shared_ptr<Player> *ent;

public:
    PlayerSave(shared_ptr<Player> *buff)
    {
        ent = buff;
    }

    void write(fstream &file) override
    {
        file.write(reinterpret_cast<char *>(&*(ent->get())), sizeof(Player) - sizeof(Hand *));
        file.write(reinterpret_cast<char *>(&*(ent->get()->getHand())), sizeof(Hand) - sizeof(vector<Spell *>));
        int spellSize = ent->get()->getHand()->getSize();
        file.write(reinterpret_cast<char *>(&spellSize), sizeof(int));
        for (int i = 0; i < spellSize; i++)
        {
            SpellType type;
            unsigned short size;
            if (typeid(*ent->get()->getHand()->getSpell(i)) == typeid(DirectDamageSpell))
            {
                type = SpellType::DirectDamage;
                size = ent->get()->getHand()->getSpell(i)->getLevel();
            }
            else if (typeid(*ent->get()->getHand()->getSpell(i)) == typeid(SquareDamageSpell))
            {
                type = SpellType::SquareDamage;
                size = ent->get()->getHand()->getSpell(i)->getLevel();
            }
            file.write(reinterpret_cast<char *>(&type), sizeof(SpellType));
            file.write(reinterpret_cast<char *>(&size), sizeof(unsigned short));
        }
    }

    void read(fstream &file) override
    {
        *ent = make_shared<Player>();
        file.read(reinterpret_cast<char *>(&*(ent->get())), sizeof(Player) - sizeof(Hand *));
        file.read(reinterpret_cast<char *>(&*(ent->get())->getHand()), sizeof(Hand) - sizeof(vector<Spell *>));
        int spellSize = 0;
        file.read(reinterpret_cast<char *>(&spellSize), sizeof(int));
        for (int i = 0; i < spellSize; i++)
        {
            SpellType type;
            file.read(reinterpret_cast<char *>(&type), sizeof(SpellType));
            unsigned short lvl;
            file.read(reinterpret_cast<char *>(&lvl), sizeof(unsigned short));
            switch (type)
            {
            case SpellType::DirectDamage:
                ent->get()->getHand()->addSpecificSpell(new DirectDamageSpell(lvl));
                break;
            case SpellType::SquareDamage:
                ent->get()->getHand()->addSpecificSpell(new SquareDamageSpell(lvl));
                break;
            }
        }
    }
};

#endif