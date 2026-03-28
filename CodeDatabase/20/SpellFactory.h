#pragma once
#include <memory>
#include <cstdlib>
#include <ctime>
#include "Lightning_bolt.h"
#include "Firewave.h"
#include "Trap.h"



class SpellFactory {
public:
    static std::shared_ptr<Spell> createRandomSpell() {
        int choice = std::rand() % 3; // 3 типа заклинаний
        switch (choice) {
            case 1: return std::make_shared<Lightning_bolt>();
            case 2: return std::make_shared<TrapSpell>();
            default: return std::make_shared<Firewave>();
        }
    }
};
