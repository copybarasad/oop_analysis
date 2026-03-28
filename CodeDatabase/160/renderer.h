#ifndef INCLUDE_RENDERER
#define INCLUDE_RENDERER

#include "../core/game_board.h"
#include <vector>

class Renderer {
private:
    std::string current_message_;
    bool show_inventory_;
    bool show_target_selection_;
    std::vector<int> target_indices_;
    bool show_area_selection_;
    std::vector<class Position> area_positions_;
    int current_level_;

    std::string GetCellSymbol(CellType CellType) const;
    void RenderLegend(int row) const;
    void RenderInventory(const GameBoard& board, int row) const;

public:
    Renderer();

    void RenderBoard(const GameBoard& board);
    void ShowError(const std::string& message);
    void ShowWarning(const std::string& message);
    void ClearMessage();
    void ToggleInventory();
    void SetInventoryView(bool show);
    bool is_inventory_shown() const { return show_inventory_; }
    
    void EnableTargetSelection(const std::vector<int>& targets);
    void DisableTargetSelection();
    bool is_target_selection_active() const { return show_target_selection_; }
    const std::vector<int>& target_indices() const { return target_indices_; }

    void EnableAreaSelection(const std::vector<Position>& positions);
    void DisableAreaSelection();
    bool is_area_selection_active() const { return show_area_selection_; }

    void SetLevel(int level) { current_level_ = level; }
    int GetLevel() const { return current_level_; }
};

#endif