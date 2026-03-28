#ifndef ENHANCEMENTS_H
#define ENHANCEMENTS_H

#include <iostream>

struct Enhancements {
    int extra_damage = 0;
    int extra_range = 0;
    int extra_area = 0;
    int extra_trap_damage = 0;
    int extra_summons = 0;

    void reset() {
        extra_damage = 0;
        extra_range = 0;
        extra_area = 0;
        extra_trap_damage = 0;
        extra_summons = 0;
    }
    
    void save(std::ostream& os) const {
        os << extra_damage << " " << extra_range << " " << extra_area << " " 
           << extra_trap_damage << " " << extra_summons << " ";
    }
    
    void load(std::istream& is) {
        is >> extra_damage >> extra_range >> extra_area >> extra_trap_damage >> extra_summons;
    }
};

#endif