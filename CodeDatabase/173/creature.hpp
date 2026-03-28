#ifndef CREATURE_HPP
#define CREATURE_HPP

#include <string>

#include "combattable.hpp"
#include "map/map.hpp"
#include "positionable.hpp"
#include "pretty/color.hpp"
#include "pretty/printable.hpp"
#include "save/dumpable.hpp"
#include "statusable.hpp"
#include "team.hpp"

class Creature : public Combattable,
                 public Statusable,
                 public Positionable,
                 public Printable,
                 public Dumpable {
  public:
    Creature(int hp, int maxHP);
    Creature(const Creature &creature);

    void setMap(Map *map);
    bool canMove();
    void move(Direction direction);
    void update();

    void setTeam(Team t);
    Team team();
    bool isEnemy();
    void makeRandomMove();

    std::string chr() override;
    Color color() override;
    virtual Color baseColor();

    bytedump dump() override;
    Creature(bytedump bytes);

  protected:
    Map *m_Map;
    Team m_Team;
};

#endif /* CREATURE_HPP */
