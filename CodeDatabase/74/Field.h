#ifndef FIELD_H
#define FIELD_H

#include "Player.h"
#include "Enemy.h"
#include "Tower.h"
#include "Types.h"
#include <vector>
#include <string>
#include "Cell.h"

class Field {
private:
    int width;
    int height;
    std::vector<std::vector<Cell>> cells;
    std::vector<Trap> traps;
    std::vector<Tower> towers;

public:
    Field(int w, int h, int numOfUnwalkableCells);

    bool isCellWalkable(int x, int y) const;

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    Field(const Field& other);
    Field(Field&& other) noexcept;
    Field& operator=(const Field& other);
    Field& operator=(Field&& other) noexcept;

    // --- Traps ---
    void spawnTrap(const Coords& pos, int damage);
    void removeTrapAt(const Coords& pos);
    int checkTrapDamage(const Coords& pos) const;
    bool hasTrapAt(int x, int y) const;
    const std::vector<Trap>& getTraps() const { return traps; }

    // === Towers ===
    void addTower(const Tower& tower);
    std::vector<Tower>& getTowers();
    bool hasTowerAt(int x, int y) const;

    void log() const;

    json serialize() const;
    void deserialize(const json& j);
};

#endif // FIELD_H
