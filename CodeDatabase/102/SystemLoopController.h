#pragma once
#include "../Containers/CompositionRootN2.h"
#include "../Containers/GameData.h"

class SystemLoopController
{
    friend class SystemLoadController;
    CompositionRootN1* _currentRoot1;
    CompositionRootN2* _currentRoot2;
    Player* _player;
    std::vector<Enemy*> _enemies;
    std::vector<StraightTower*> _towers;
    std::vector<std::string> _spells;

    void Initialize();

    void Initialize(GameData& data);

    void PrintField();

    void PrintVictory();

    void PrintDefeat();

    int ToNextGameIteration();

    int ToCommand(char c);
public:
    SystemLoopController();

    void PrintIntroduction();
};
