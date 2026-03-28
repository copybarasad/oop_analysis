#pragma once
#include "../game_objects/Player.h"
#include "../game_objects/GameField.h"
#include "../game_objects/Character.h"
#include "GameState.h"
#include "../game_objects/PlayerHand.h"
#include <vector>

class Game;

class InputHandler {
public:
    // Считывание с клавиатуры
    char getInput();

    // Передвижение игрока
    void processInput(Game& game);
    //void processInput(Player& player, const PlayerHand& hand, const GameField& field, GameState& state, GameController& controller);

    // Есть ли враги рядом 
    bool showTargetsInRange(const Player& player, const GameState& state, int range);

    // Процесс обработки заклинания 
    void spellProcessing(Player& player, const PlayerHand& hand, GameState& state);
    
};