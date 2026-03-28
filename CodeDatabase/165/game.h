#ifndef GAMEH
#define GAMEH
#include "../map.h"
#include "../abstractNinterfaces/decision.h"
class game : public decision{
    map* currentMap;
    bool activeFlag = true;
public:
    game();
    void newGame();
    void continueGame();
    void close();
    bool isActive();
    void setCurrentMap(map*);
    void menu();
    void save();
    void writeASave(const char*);
    void listSaves();
    void load();
    map* loadMap(std::ifstream&);
    hero* loadHero(std::ifstream&);
    std::vector<enemy*> loadEnemies(std::ifstream&);
    std::vector<castle*> loadCastles(std::ifstream&);
    size_t loadTurn(std::ifstream&);
    size_t loadDifficultyLvl(std::ifstream&);
    void play();
    void nextLevel();
    std::vector<size_t>* determineScenario();
    void restoreScenarios();
    ~game();
};
#endif