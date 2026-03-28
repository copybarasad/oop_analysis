#ifndef GAME_VIEW_H
#define GAME_VIEW_H

template <typename T_Renderer>
class GameView {
public:
    GameView() : renderer_() {}
    
    explicit GameView(T_Renderer renderer) : renderer_(renderer) {}

    template <typename GameType>
    void Update(const GameType& game) {
        renderer_.Render(game);
    }

    void ShowGameOverPrompt() { renderer_.ShowGameOverPrompt(); }
    void ShowLevelClearMessage(int level) { renderer_.ShowLevelClearMessage(level); }
    void ShowVictoryMessage() { renderer_.ShowVictoryMessage(); }
    void ShowUpgradeMenu() { renderer_.ShowUpgradeMenu(); }
    void ShowExitMessage() { renderer_.ShowExitMessage(); }
    void ShowMainMenu() { renderer_.ShowMainMenu(); }
    void ShowEnterFilenamePrompt() { renderer_.ShowEnterFilenamePrompt(); }
    void ShowLoadSuccessMessage() { renderer_.ShowLoadSuccessMessage(); }
    void ShowLoadErrorMessage(const std::string& error) { renderer_.ShowLoadErrorMessage(error); }
    void ShowInvalidChoiceMessage() { renderer_.ShowInvalidChoiceMessage(); }
    void ShowActionMessage(const std::string& msg) { renderer_.ShowActionMessage(msg); }

private:
    T_Renderer renderer_;
};

#endif
