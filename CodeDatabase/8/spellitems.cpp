#include "spellitems.h"

Fireball::Fireball() : DirectSpell(4, 10, 3, "Fireball") {}
IceBolt::IceBolt() : DirectSpell(3, 8, 5, "Ice Bolt") {}
FireWave::FireWave() : AreaSpell(5, 20, 5, "Fire Wave") {}
IceStorm::IceStorm() : AreaSpell(6, 15, 4, "Ice Storm") {}
PoisonTrap::PoisonTrap() : TrapSpell(4, 6, 3, "Poison Trap") {}