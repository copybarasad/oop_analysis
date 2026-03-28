#pragma once

#include "Input/IInputReader.h"
#include "Input/KeyBindings.h"
#include "Input/Direction.h"

class Board;
class Hand;
class Player;
class IGameRenderer;

/**
 * @brief Реализация IInputReader для консольного ввода
 * 
 * @requirement Требование 1: Реализация класса считывающего ввод пользователя
 * 
 * Использует KeyBindings для маппинга клавиш на команды (требование 5).
 * 
 * @see IInputReader
 * @see KeyBindings
 * @see GameAction
 */
class ConsoleInputReader : public IInputReader {
public:
    ConsoleInputReader();

    void setRenderer(IGameRenderer* renderer) override;
    GameCommand mapInputToCommand(const std::string& input) const override;
    std::string readRawInput() override;

    std::unique_ptr<GameAction> parseCommand(
        const std::string& input,
        Board* board,
        Hand* hand,
        Player* player
    ) override;

private:
    std::unique_ptr<GameAction> handleMovementInput(Direction direction);
    std::unique_ptr<GameAction> handleAttackInput(Board* board);
    std::unique_ptr<GameAction> handleSpellInput(Hand* hand, Board* board);
    std::unique_ptr<GameAction> handleOptionsMenu(Board* board, Hand* hand, Player* player);
    std::unique_ptr<GameAction> handleUpgradeMenu(Player* player, Hand* hand);

    char getDirectionFromUser();
    int getSpellChoice(Hand* hand);

    void renderOptionsMenuFallback();
    void renderUpgradeMenuFallback(Player* player, Hand* hand);

    KeyBindings keyBindings;
    IGameRenderer* renderer{nullptr};
};

