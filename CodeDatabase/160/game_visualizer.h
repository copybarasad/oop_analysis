#ifndef INCLUDE_UI_GAME_VISUALIZER
#define INCLUDE_UI_GAME_VISUALIZER

#include "game_view.h"

template <class RendererT>
class GameVisualizer final : public IGameView {
private:
    RendererT renderer_;

public:
    GameVisualizer() = default;

    RendererT& renderer() { return renderer_; }
    const RendererT& renderer() const { return renderer_; }

    void Render(const GameBoard& board) override { renderer_.RenderBoard(board); }

    void ShowError(const std::string& message) override { renderer_.ShowError(message); }
    void ShowWarning(const std::string& message) override { renderer_.ShowWarning(message); }

    void ToggleInventory() override { renderer_.ToggleInventory(); }
    void SetInventoryView(bool show) override { renderer_.SetInventoryView(show); }
    bool IsInventoryShown() const override { return renderer_.is_inventory_shown(); }

    void EnableTargetSelection(const std::vector<int>& targets) override { renderer_.EnableTargetSelection(targets); }
    void DisableTargetSelection() override { renderer_.DisableTargetSelection(); }

    void EnableAreaSelection(const std::vector<Position>& positions) override { renderer_.EnableAreaSelection(positions); }
    void DisableAreaSelection() override { renderer_.DisableAreaSelection(); }

    void SetLevel(int level) override { renderer_.SetLevel(level); }
};

#endif
