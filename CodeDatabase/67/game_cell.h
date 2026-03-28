#ifndef GAME_CELL_H
#define GAME_CELL_H

class game_cell {
    bool has_player;
    bool has_enemy;
    bool has_trap;
    bool has_tower;
    int trap_damage;
    
public:
    game_cell();
    bool hasPlayer() const;
    bool hasEnemy() const;
    bool hasTrap() const;
    bool hasTower() const;
    int getTrapDamage() const;
    
    void setPlayer(bool does_has_player);
    void setEnemy(bool does_has_enemy);
    void setTrap(bool does_has_trap, int damage);
    void setTower(bool does_has_tower);
    
    bool isEmpty() const;
};

#endif
