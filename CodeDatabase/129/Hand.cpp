#include "Hand.h"

Hand::Hand() {}
Hand::Hand(int max_count){
    this->max_count = max_count;
    acceptable.push_back("Area");
    acceptable.push_back("DD");
    acceptable.push_back("Trap");

    last_spell = 0;
}   

Hand::Hand(std::vector<std::string> spells){
    this->max_count = max_count;
    acceptable.push_back("Area");
    acceptable.push_back("DD");
    acceptable.push_back("Trap");

    last_spell = 0;
    this->spells = spells;
}
Hand::~Hand() {}


void Hand::add_spell(){
    if(last_spell == acceptable.size()){
        last_spell = 0;
    }

    if(spells.size() == max_count) return;

    spells.push_back(acceptable[last_spell++]);
}


std::unique_ptr<Spell> Hand::get_DDSpell(){
    for(int i = 0; i < spells.size(); i++){
        if(spells[i] == "DD"){
            spells.erase(spells.begin() + i);
            break;
        }
    }

    return std::make_unique<DDSpell>();
}

std::unique_ptr<Spell> Hand::get_ARSpell(){
    for(int i = 0; i < spells.size(); i++){
        if(spells[i] == "Area"){
            spells.erase(spells.begin() + i);
            break;
        }
    }

    return std::make_unique<ARSpell>();
}


std::unique_ptr<Spell> Hand::get_TrapSpell(){
    for(int i = 0; i < spells.size(); i++){
        if(spells[i] == "Trap"){
            spells.erase(spells.begin() + i);
            break;
        }
    }

    return std::make_unique<TrapSpell>();
}

std::unique_ptr<Spell> Hand::get_spell(char c){
    if(c == CONTROL::ACT_DD_SPELL){
       return get_DDSpell();
    }
    
    if(c == CONTROL::ACT_AR_SPELL){
       return get_ARSpell();
    }

    if(c == CONTROL::ACT_TR_SPELL){
        return get_TrapSpell();
    }
}


bool Hand::can_use_spell(char c){
    bool can = false;

    for(std::string spell : spells){
        switch(c){
            case CONTROL::ACT_DD_SPELL:
                if(spell == "DD"){
                    can = true;
                }
                break;
            case CONTROL::ACT_AR_SPELL:
                if(spell == "Area"){
                    can = true;
                }
                break;

            case CONTROL::ACT_TR_SPELL:
                if(spell == "Trap"){
                    can = true;
                }
                break;
        }
    }

    return can;
}


void Hand::add_such_spell(std::unique_ptr<Spell> spell){
    spells.push_back(spell->get_type());
}

void Hand::check_spell(std::unique_ptr<Spell> spell, bool was){
    if(!was && spell->need_to_return()){
        add_such_spell(std::move(spell));
    }
}

std::vector<std::string> Hand::get_my_spells(){
    return spells;
}

int Hand::get_max_count(){
    return max_count;
}

std::vector<std::string> Hand::get_reset_spells(){
    int half = spells.size() / 2;
    for(int i = 0; i < half; i++){
        spells.pop_back();
    }

    return spells;
}


Saving::HandData Hand::serialize() const{
    Saving::HandData result;
    result.acceptable = acceptable;
    result.spells = spells;
    result.last_spell = last_spell;
    result.max_count = max_count;
    return result;
}

void Hand::deserialize(const Saving::HandData& data){
    acceptable = data.acceptable;
    spells = data.spells;
    last_spell = data.last_spell;
    max_count = data.max_count;
}

