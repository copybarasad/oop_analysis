#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#include "GameTypes.h"
#include <vector>
#include <string>

template <typename Renderer>
class GameView {
private:
    Renderer renderer;
    
public:
    GameView() = default;

    template<typename GameT>
    void DrawGame(GameT& game) {
        renderer.RenderField(game.GetField(), game.GetPlayerX(), 
                           game.GetPlayerY(), game.GetEnemyPositions());
    }

    template<typename GameT>
    void DrawMenu(GameT& game) {
        renderer.RenderMenu(game.GetPlayer(), game.GetHand(), 
                          game.GetPlayerX(), game.GetPlayerY());
    }

    template<typename GameT>
    void DrawStats(GameT& game) {
        renderer.RenderPlayerStats(game.GetPlayer(), game.GetHand());
    }
    
    void DrawAttackResult(const AttackResult& result) {
        renderer.RenderAttackResult(result);
    }
    
    void DrawSpellResult(const SpellResult& result) {
        renderer.RenderSpellResult(result);
    }
    
    template<typename GameT>
    void DrawSearchResults(GameT& game, const std::vector<EnemyInfo>& enemies) {
        renderer.RenderSearchResults(enemies, game.GetPlayerX(), game.GetPlayerY());
    }
    
    void DrawEnemyAttacks(const std::vector<std::string>& attacks) {
        renderer.RenderEnemyAttacks(attacks);
    }
    
    void DrawMessage(const std::string& message) {
        if (!message.empty()) {
            renderer.RenderGameMessage(message);
        }
    }
    
    void DrawSaveLoadMessage(const std::string& message) {
        renderer.RenderSaveLoadMessage(message);
    }
    
    void DrawGameOver(bool playerWon) {
        renderer.RenderGameOver(playerWon);
    }
    
    void DrawUpgradeMenu() {
        renderer.RenderUpgradeMenu();
    }
};

#endif