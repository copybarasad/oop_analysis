#include "spell.h"
#include <iostream>

std::string Spell::GetInfo() {
    return info;
}

void Spell::SetInfo(std::string info) {
    this->info = info;
}