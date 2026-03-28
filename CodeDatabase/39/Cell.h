#ifndef CELL_H
#define CELL_H

#include <cstddef>

class Cell {
public:
    enum class Terrain {
        kEmpty,
        kBlocked,
        kSlow
    };

    enum class Occupant {
        kNone,
        kPlayer,
        kEnemy,
        kEnemyBase,
        kEnemyTower,
        kAlly
    };

    Cell();

    Terrain GetTerrain() const;

    void SetTerrain(Terrain terrain);

    Occupant GetOccupant() const;

    void SetOccupant(Occupant occupant);

    int GetOccupantIndex() const;

    void SetOccupantIndex(int index);

    bool HasTrap() const;

    int GetTrapDamage() const;

    void SetTrap(int damage);
    void ClearTrap();

private:
    Terrain terrain_;
    Occupant occupant_;
    int occupant_index_;

    bool has_trap_;
    int trap_damage_;
};

#endif
