#ifndef INCLUDE_UI_GAME_VIEW
#define INCLUDE_UI_GAME_VIEW

#include <string>
#include <vector>

#include "../core/game_board.h"
#include "position.h"

class IGameView {
public:
    virtual ~IGameView() = default;

    virtual void Render(const GameBoard& board) = 0;

    virtual void ShowError(const std::string& message) = 0;
    virtual void ShowWarning(const std::string& message) = 0;

    virtual void ToggleInventory() = 0;
    virtual void SetInventoryView(bool show) = 0;
    virtual bool IsInventoryShown() const = 0;

    virtual void EnableTargetSelection(const std::vector<int>& targets) = 0;
    virtual void DisableTargetSelection() = 0;

    virtual void EnableAreaSelection(const std::vector<Position>& positions) = 0;
    virtual void DisableAreaSelection() = 0;

    virtual void SetLevel(int level) = 0;
};

#endif
