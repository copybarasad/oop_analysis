#include "cell.h"

Cell::Cell() : type(CellType::EMPTY), is_available(true), is_slow_trap(false), is_trap_spell(false) {}
    
CellType Cell::get_type() const { return type; }
bool Cell::get_availability() const { return is_available; }
bool Cell::get_slow_trap() const { return is_slow_trap; }
bool Cell::get_trap_spell() const { return is_trap_spell; }

void Cell::set_type(CellType new_type)
{
    type = new_type;
}
void Cell::set_availability(bool availability)
{
    is_available = availability;
}
void Cell::set_slow_trap(bool slow_trap)
{
    is_slow_trap = slow_trap;
}
void Cell::set_trap_spell(bool trap_spell)
{
    is_trap_spell = trap_spell;
}

void Cell::save_binary(std::ostream& os) const
{
    int32_t type_ = static_cast<int32_t>(type);
    os.write(reinterpret_cast<const char*>(&type_), sizeof(type_));
    char slow = static_cast<char>(is_slow_trap);
    char trap = static_cast<char>(is_trap_spell);
    char avail = static_cast<char>(is_available);
    os.write(&slow, sizeof(slow));
    os.write(&trap, sizeof(trap));
    os.write(&avail, sizeof(avail));
}

void Cell::load_binary(std::istream& is)
{
    int32_t type_ = 0;
    is.read(reinterpret_cast<char*>(&type_), sizeof(type_));
    type = static_cast<CellType>(type_);
    char slow=0, trap=0, avail=0;
    is.read(&slow, sizeof(slow));
    is.read(&trap, sizeof(trap));
    is.read(&avail, sizeof(avail));
    is_slow_trap = static_cast<bool>(slow);
    is_trap_spell = static_cast<bool>(trap);
    is_available = static_cast<bool>(avail);
}