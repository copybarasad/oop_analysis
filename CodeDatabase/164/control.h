#ifndef CONTROL_H
#define CONTROL_H

#include <fstream>
#include "json.hpp"

#define CONFIG_FILE "config.json"
using json = nlohmann::json;

class Control{
    public:
    char exit;
    char save;
    char load;
    char start;
    char yes;
    char no;
    char move;
    char change_attack;
    char use_spell;
    char help;

    Control();

    void reset();
};

#endif