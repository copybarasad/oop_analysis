#pragma once
#include <iostream>
#include "Game.h"

using namespace std;

class ConsoleRenderStrategy {
public:
    void render(const Game& game) {
        cout << "=== GAME STATE ===" << endl;
        
        if (game.GetPlayer().IsAlive_player()) {
            game.GetPlayer().Statistics_player();
        } else {
            cout << "PLAYER: DEFEATED" << endl;
        }
        
        cout << endl;
        
        if (game.GetEnemy().IsAlive_enemy()) {
            game.GetEnemy().Statistics_enemy();
        } else {
            cout << "ENEMY: DEFEATED" << endl;
        }
        
        cout << endl;
        cout << "Spell hand: " << game.GetSpellHand().GetSpellCount() << "/5 spells" << endl;
        cout << "Current level: " << game.GetCurrentLevel() << endl;
        cout << "==========================" << endl;
        
        cout << "=== GAME FIELD ===" << endl;
        if (game.GetField()) {
            game.GetField()->DisplayField(game.GetPlayer(), game.GetEnemy());
        }
        
        cout << endl;
        cout << "Controls: WASD-move, E-exit, I-instructions, Z-stats" << endl;
        cout << "B-change range, C-cast spell, H-show spells, P-buy spells" << endl;
    }
};