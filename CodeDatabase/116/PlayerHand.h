#include "../spells/ISpell.h"
#include <memory>
#include <vector>

class PlayerHand{
private:
    const int maxCapacity;
    std::vector<std::vector<std::unique_ptr<ISpell>>> spells;
public:
    PlayerHand(int maxSpells = 5);
    ~PlayerHand();
    ISpell* getSpell(int index);
    bool isFull();
    void addSpell(std::unique_ptr<ISpell> spell, int index);
    void removeSpell(int index);
    int getCurrentSize();
    int getMaxSize();
    int getSpellNum(int index);

    const std::vector<std::vector<std::unique_ptr<ISpell>>>& getSpells() const { return spells; }
    int getMaxCapacity() const { return maxCapacity; }

    void clearAndResize();
};