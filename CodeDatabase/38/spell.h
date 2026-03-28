#pragma once
class Spell {

public:
  typedef enum { DIRECT = 0, AREA, TRAP } SpellType;

  Spell() {}
  virtual SpellType getType() const { return type; }
  virtual ~Spell() = default;

protected:
  unsigned int cost;
  unsigned int damage;
  SpellType type;
};
