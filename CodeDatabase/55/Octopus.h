#pragma once
#include "Diver.h"


class Octopus: public Diver {
private:
    int range;
public:
    Octopus(GameConfig &cfg);
    int getRange();
    void setRange(int amount);
    void saveTo(SaveFileRAII &out) const;
    void loadFrom(SaveFileRAII &in);
};