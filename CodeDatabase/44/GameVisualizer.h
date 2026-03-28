#pragma once
#include <string>
#include <vector>

class Field;
class Player;

template<typename TRenderer>
class GameVisualizer {
public:
    GameVisualizer() : renderer_() {}

    void RenderGameState(const Field& field, const Player& player, int spell_count) {
        renderer_.RenderGameState(field, player, spell_count);
    }

    void RenderField(const Field& field) {
        renderer_.RenderField(field);
    }

    void RenderPlayerStatus(const Player& player, int spell_count) {
        renderer_.RenderPlayerStatus(player, spell_count);
    }

    void RenderMainMenu() {
        renderer_.RenderMainMenu();
    }

    void RenderLevelUpMenu() {
        renderer_.RenderLevelUpMenu();
    }

    void RenderSavesList(const std::vector<std::string>& saves) {
        renderer_.RenderSavesList(saves);
    }

    void ShowMessage(const std::string& message) {
        renderer_.ShowMessage(message);
    }

    void ShowError(const std::string& error) {
        renderer_.ShowError(error);
    }

    void ShowVictory() {
        renderer_.ShowVictory();
    }

    void ShowDefeat() {
        renderer_.ShowDefeat();
    }

    void ShowLevelTransition(int level) {
        renderer_.ShowLevelTransition(level);
    }

    void ShowGameComplete() {
        renderer_.ShowGameComplete();
    }

    void ShowPrompt(const std::string& prompt) {
        renderer_.ShowPrompt(prompt);
    }

    void ShowCommandPrompt() {
        renderer_.ShowCommandPrompt();
    }

    TRenderer& GetRenderer() { return renderer_; }
    const TRenderer& GetRenderer() const { return renderer_; }

private:
    TRenderer renderer_;
};
