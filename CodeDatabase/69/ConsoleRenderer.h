#ifndef CONSOLE_RENDERER_H
#define CONSOLE_RENDERER_H

#include <string>

class Game;

class ConsoleRenderer {
public:
    void Render(const Game& game) const;
    
    void ShowGameOverPrompt() const;
    void ShowLevelClearMessage(int level) const;
    void ShowVictoryMessage() const;
    void ShowUpgradeMenu() const;
    void ShowExitMessage() const;
    void ShowMainMenu() const;
    void ShowEnterFilenamePrompt() const;
    void ShowLoadSuccessMessage() const;
    void ShowLoadErrorMessage(const std::string& error) const;
    void ShowInvalidChoiceMessage() const;
    void ShowActionMessage(const std::string& msg) const;
};

#endif
