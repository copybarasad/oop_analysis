#include "swamp.hpp"
#include "viscous.hpp"

Swamp::Swamp(): 
    Landscape({ Viscous() }, '~')
{}