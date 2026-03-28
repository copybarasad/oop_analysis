#pragma once
#include <vector>
#include <string>
#include <memory>
#include "../utils/Values.h"
#include "../../saving/Saving.h"
#include "DDSpell.h"
#include "ARSpell.h"
#include "TrapSpell.h"
#include "../../command_manager/Control.h"

class Hand{
private:
    std::vector<std::string> acceptable; //допустимые значения заклинания
    std::vector<std::string> spells;
    int last_spell; //чтобы добавлять новые заклинания друг за другом
    int max_count; //размер руки
public:
    Hand();
    Hand(int max_count); 
    Hand(std::vector<std::string> spells); 
    ~Hand();

    void add_spell();
    std::unique_ptr<Spell> get_DDSpell();
    std::unique_ptr<Spell> get_ARSpell();
    std::unique_ptr<Spell> get_TrapSpell();

    std::unique_ptr<Spell> get_spell(char c);
    void check_spell(std::unique_ptr<Spell> spell, bool was);
    void add_such_spell(std::unique_ptr<Spell> spell);

    bool can_use_spell(char c);
    std::vector<std::string> get_my_spells();
    int get_max_count();
    std::vector<std::string> get_reset_spells();

    Saving::HandData serialize() const;
    void deserialize(const Saving::HandData& data);

};
