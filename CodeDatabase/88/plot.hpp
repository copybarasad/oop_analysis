#ifndef PLOT_HPP
#define PLOT_HPP

#include "../entities/character.hpp"

class Plot {
    char type;
    char originalType;
    Character* obj;
    bool ownsCharacter;
    
public:
    Plot(char type = '*');
    Plot(char type, Character* obj);
    ~Plot();
    
    char getType() const { return type; }
    Character* getCharacter() const { return obj; }
    
    void placeObj(Character* obj, bool takeOwnership = false);
    void removeObj();
    bool isEmpty();
    bool isNotPassable();
    char getOriginalType() const { return originalType; }
    void setTrap();
    bool isTrap();
    void consumeChest();
    bool isChest();

};

#endif