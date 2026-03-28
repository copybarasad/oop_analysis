#include <random>
#include <vector>
#include <iostream>
#include "hand.h"
#include "spells.h"
#include "../player_game/saver.h"


hand::hand(int hs)
    :hand_size(hs)
{
    gen_spell();
}

hand::hand(std::vector<std::string> names, std::vector<int> levels)
{
    Ispell* nspell;
    for (int i = 0; i < names.size(); i++)
    {
        if (names[i] == DAM_SPELL)
        {
            nspell = new damage_spell();
        }
        else if (names[i] == AOE_SPELL)
        {
            std::cout << "ok" << std::endl;
            nspell = new aoe_spell();
        }
        else if (names[i] == TRAP_SPELL)
        {
            nspell = new trap_spell();
        }
        else if (names[i] == SUMMON_SPELL)
        {
            nspell = new summon_spell();
        }
        else if (names[i] == UPGRADE_SPELL)
        {
            nspell = new upgrade_spell();
        }
        else
        {
            throw std::runtime_error("Incorrect spell name in file");
        }
        nspell->upgrade(levels[i] - 1);
        spells_in_hand.push_back(nspell);
    }
}

void hand::use_spell_in_hand(map* map, Ispell* spell, int index)
{
    if (spell->use_spell(map, this))
    {
        del_spell(spell, index);
    }
}

void hand::gen_spell()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> rspell(0, 4);
    int start_spell = rspell(gen);
    Ispell* spell;
    switch (start_spell)
    {
        case 0:
            spell = new damage_spell();
            add_spell(spell);
            break;
        case 1:
            spell = new aoe_spell();
            add_spell(spell);
            break;
        case 2:
            spell = new trap_spell();
            add_spell(spell);
            break;
        case 3:
            spell = new summon_spell();
            add_spell(spell);
            break;
        case 4:
            spell = new upgrade_spell();
            add_spell(spell);
            break;
    }
}

void hand::add_spell(Ispell* spell_ptr)
{
    if (spells_in_hand.size() < hand_size)
    {
        spells_in_hand.push_back(spell_ptr);
        std::cout << "You got " << spell_ptr->get_name() << " spell " << spell_ptr->get_level() << " lvl" << std::endl;
    }
    else
    {
        std::cout << "You can't have more spells, the spell you just received has been delited" << std::endl;
    }
}

void hand::print_hand()
{
    for (int i = 0; i < spells_in_hand.size(); i++)
    {
        std::cout << i << ": " << spells_in_hand[i]->get_name() << " spell " << spells_in_hand[i]->get_level() << " lvl" << std::endl;
    }
}

std::vector<Ispell*> hand::get_spells_in_hand()
{
    return spells_in_hand;
}

void hand::del_spell(Ispell* spell, int index)
{
    spells_in_hand.erase(spells_in_hand.begin() + index);
    delete spell;
}

std::string hand::save_hand()
{
    std::string data;
    data += std::to_string(spells_in_hand.size()) + DATA_SEP2;
    for (Ispell* s : spells_in_hand)
    {
        data += s->get_name() + DATA_SEP + std::to_string(s->get_level()) + DATA_SEP2;
    }
    return data;
}

hand::~hand()
{
    for (Ispell* spell : spells_in_hand)
    {
        delete spell;
    }
}
