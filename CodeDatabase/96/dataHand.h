#pragma once
#include <vector>
#include <memory>
#include "dataSpell.h"


struct dataHand {
    int maxSpell;
    int currentSpell;
    std::vector<std::shared_ptr<dataSpell>> spells;
};