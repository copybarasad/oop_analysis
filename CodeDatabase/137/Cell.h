#ifndef CELL_H
#define CELL_H

enum class CellType {
    NORMAL,
    WALL,
    SLOWING
};

class Cell {
public:
    Cell();
    explicit Cell(CellType type);

    CellType GetType() const;
    void SetType(CellType type);

    bool IsPassable() const;

    bool HasPlayer() const;
    bool HasEnemy() const;
    bool HasEnemyBuilding() const;
    bool HasEnemyTower() const;
    bool HasAlly() const;
    bool HasTrap() const;

    int GetTrapDamage() const;

    void SetPlayer(bool value);
    void SetEnemy(bool value);
    void SetEnemyBuilding(bool value);
    void SetEnemyTower(bool value);
    void SetAlly(bool value);
    void SetTrap(bool value, int damage = 0);

private:
    CellType type_;
    bool has_player_;
    bool has_enemy_;
    bool has_enemy_building_;
    bool has_enemy_tower_;
    bool has_ally_;
    bool has_trap_;
    int trap_damage_;
};

#endif // CELL_H
