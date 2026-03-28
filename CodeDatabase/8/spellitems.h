#pragma once
#include "directspell.h"
#include "areaspell.h"
#include "trap.h"

class Fireball : public DirectSpell {
public:
    Fireball();
};

class IceBolt : public DirectSpell {
public:
    IceBolt();
};

class FireWave : public AreaSpell {
public:
    FireWave();
};

class IceStorm : public AreaSpell {
public:
    IceStorm();
};

class PoisonTrap : public TrapSpell {
public:
    PoisonTrap();
};