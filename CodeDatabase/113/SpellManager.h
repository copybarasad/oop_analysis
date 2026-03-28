#ifndef SPELLMANAGER_H
#define SPELLMANAGER_H

#include "Player.h"
#include "SpellFactory.h"
#include <memory>

class SpellManager {
private:
    int m_generationCounter;
    int m_generationInterval;

public:
    SpellManager(int generationInterval = 5);

    void update(Player& player);
    void resetCounter() { m_generationCounter = 0; }
    void setGenerationInterval(int interval) { m_generationInterval = interval; }

private:
    void generateNewSpell(Player& player);
    bool shouldGenerateSpell() const;
};

#endif // SPELLMANAGER_H