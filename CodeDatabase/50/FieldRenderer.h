#pragma once
#include <iostream>
#include "GameField.h"

class FieldRenderer {
public:
    static void print(const Field& field, int hp, int turns, bool isAttRange, int mana);
};
