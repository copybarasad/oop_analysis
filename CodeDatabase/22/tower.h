#include <memory>               
#include "characters/character.h"          
#include "spells/directSpell.h" 
struct Position;

class Tower : public Character {
    private:
        std::unique_ptr<DirectSpell> spell_;

    public:
        Tower(int health, Position position);

        DirectSpell* attack();
};