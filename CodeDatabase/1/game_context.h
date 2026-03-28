// game_context.h - ПРОСТАЯ ВЕРСИЯ БЕЗ ПРЕОБРАЗОВАНИЙ
#pragma once

class Field;
class VampireHunter;
class Dracula;

// ТОЛЬКО структуры, без операторов преобразования
struct GameContext {
    Field& field;
    VampireHunter& hunter;
    Dracula& dracula;
    
    GameContext(Field& f, VampireHunter& h, Dracula& d) 
        : field(f), hunter(h), dracula(d) {}
};

struct ConstGameContext {
    const Field& field;
    const VampireHunter& hunter;
    const Dracula& dracula;
    
    ConstGameContext(const Field& f, const VampireHunter& h, const Dracula& d) 
        : field(f), hunter(h), dracula(d) {}
};