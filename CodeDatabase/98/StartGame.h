#ifndef GAMESTARTER_H
#define GAMESTARTER_H

#include "game.h"
#include "GameController.h"
#include "GameView.h"
#include "UserInputConsole.h"
#include "ConsoleRenderer.h"
#include <memory>

class GameStarter {
private:
    bool ValidateFieldSize(int width, int height);
    bool ValidateChance(double chance);
    bool ValidateEnemyCount(int count);
    
public:
    void ShowMainMenu();
    void StartNewGame();
    bool LoadExistingGame();
    void Run();
    
    template<typename InputT, typename RenderT>
    void RunGame(Game& game);
};

#endif