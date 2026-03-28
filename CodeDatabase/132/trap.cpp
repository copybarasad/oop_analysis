#include "trap.hpp"
#include "effect.hpp"

const int TRAP_DEFAULT_DAMAGE = 10;

Trap::Trap():
    Landscape({ Effect(TRAP_DEFAULT_DAMAGE, "trap") }, '_', 1)
{}

Trap::Trap(int lvl):
    Landscape(
        { Effect(TRAP_DEFAULT_DAMAGE + TRAP_DEFAULT_DAMAGE * lvl, "trap") },
        '_', 
        1
    )
{}

Trap::Trap(const TrapImage& img):
    Landscape(
        { Effect(TRAP_DEFAULT_DAMAGE + TRAP_DEFAULT_DAMAGE * img.lvl, "trap") },
        '_',
        img.solidity
    )
{}

TrapImage Trap::get_data_image() {
    return TrapImage(
        lvl,
        solidity
    );
}