
#ifndef MAP_CELL_H
#define MAP_CELL_H
#include "entity.h"
class MapCell {  
    
public:
    enum class Type { EMPTY, WALL, LOW, TRAP};
    MapCell(Type cell_type = Type::EMPTY, bool use = false);
    ~MapCell()  = default;
    Type getType() const;
    void setType(Type new_type);

    Entity* getEntity();
    const Entity* getEntity() const;
    void setEntity(Entity* entity);

    bool isUsed() const;
    void setUsed(bool used); 
    
    int getTrapDamage(){
        return trapDamage;
    }

    void setTrapDamage(int damage){
        trapDamage = damage;
    }
private:
    Entity* entity = nullptr;
    Type type;
    bool use; 
    int trapDamage;
};

#endif