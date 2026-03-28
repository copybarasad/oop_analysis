#pragma once

#include "trap_image.hpp"
#include "landscape.hpp"

class Trap : public Landscape {
private:
    int lvl;
public:
    Trap();
    Trap(int lvl);

    Trap(const TrapImage& img);
    TrapImage get_data_image();
};