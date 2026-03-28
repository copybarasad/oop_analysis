#include <cstddef>
#include <iostream>

enum spell_t{
    FIREBALL,
    SUNSTRIKE,
    TRAP,
    ALLY,
    UPGRADE
};

class Spell{
    int dmg;
    int distance;
    

    spell_t type_spell;

    Spell(size_t temp_id){
        switch (temp_id){
            case 0:
                type_spell = FIREBALL;
                dmg = 100;
                distance = 9;
                break;
            case 1:
                type_spell = SUNSTRIKE;
                dmg = 100;
                distance = 2;
                break;
            case 2:
                type_spell = TRAP;
                distance = 4;
                dmg = 50;
                break;
            case 3:
                type_spell = ALLY;
                distance = 5;
                break;
            case 4:
                type_spell = UPGRADE;
                break;
        }
    }
    std::string get_type_spell() const {
        switch (type_spell) {
            case FIREBALL: return "FIREBALL";
            case SUNSTRIKE:   return "SUNSTRIKE";
            case TRAP: return "TRAP";
            case ALLY: return "ALLY";
            case UPGRADE: return "UPGRADE"; 
        }
    }
    void show(){
        std::cout<< type_spell << " " << get_type_spell()<<std::endl;
        std::cout<< "Damage: " << dmg << std::endl;
        std::cout<< "Distance: " << distance << std::endl;
    }

};