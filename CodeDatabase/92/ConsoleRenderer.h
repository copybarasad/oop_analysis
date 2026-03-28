#pragma once
#include <string>
#include <vector>
#include "Field.h"
#include "Player.h"

class ConsoleRenderer {
public:
    void print_log(const std::string& msg);
    void render_game(Field& field, Player& player);
    void print_spells(const std::vector<std::string>& spells);
    void print_shop(const std::vector<std::string>& items);
};
