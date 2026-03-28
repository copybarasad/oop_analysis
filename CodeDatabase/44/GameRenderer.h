#pragma once
#include <string>
#include <vector>
#include "KeyBindings.h"

class Field;
class Player;
class Enemy;
class SpellHand;

class GameRenderer {
public:
    GameRenderer();

    void SetKeyBindings(const KeyBindings& bindings);

    void RenderField(const Field& field) const;

    void RenderPlayerStatus(const Player& player, int spell_count) const;

    void RenderCommands() const;

    void RenderGameState(const Field& field, const Player& player, int spell_count) const;

    void RenderMainMenu() const;

    void RenderLevelUpMenu() const;

    void RenderSavesList(const std::vector<std::string>& saves) const;

    void ShowMessage(const std::string& message) const;
    void ShowError(const std::string& error) const;
    void ShowVictory() const;
    void ShowDefeat() const;
    void ShowLevelTransition(int level) const;
    void ShowGameComplete() const;

    void ShowPrompt(const std::string& prompt) const;
    void ShowCommandPrompt() const;

private:
    static constexpr const char* kReset = "\033[0m";
    static constexpr const char* kRed = "\033[31m";
    static constexpr const char* kGreen = "\033[32m";
    static constexpr const char* kYellow = "\033[33m";
    static constexpr const char* kMagenta = "\033[35m";
    static constexpr const char* kCyan = "\033[36m";
    static constexpr const char* kGray = "\033[90m";

    char GetCellSymbol(int cell_type) const;
    const char* GetCellColor(int cell_type) const;

    KeyBindings key_bindings_;
};
