#pragma once
#include <ncurses.h>
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <wchar.h>
#include <locale.h>
#include "IVisualizer.hpp"
#include "../InputController.hpp"
#include "../IGameController.hpp"
#include "../Config.hpp"
#include "../GameState.hpp"

class NCURSESVisualizer : public IVisualizer {
public:
    NCURSESVisualizer();
    ~NCURSESVisualizer();
    void setInputController(InputController* ic);
    void setGameController(IGameController* gc);
    void display();
private:
    GameState state = GameState::MainMenu;
    GameState prevState = GameState::MainMenu;

    int mainMenuSelected = 0;
    int levelUpSelected = 0;
    int pauseMenuSelected = 0;
    int gameOverSelected = 0;
    int loadMenuSelected = 0;

    const int frame_ms = 15;
    std::chrono::steady_clock::time_point last_frame_time;

    InputController* inputController = nullptr;
    IGameController* gameController = nullptr;

    std::string errMessage{""};

    int term_w = 0;
    int term_h = 0;

    void initCurses();
    void deinitCurses();
    void updateTermSize();
    int fetchInput();

    void loopMainMenu();
    void loopInGame();
    void loopLevelUp();
    void loopGameOverMenu();
    void loopPauseMenu();
    void loopAutorsMenu();
    void loopLoadMenu();

    void drawPatternSelectedMenu(
        std::vector<std::wstring>& asciiArt,
        std::vector<std::string>& selectedOptions,
        int artColor,
        int selector,
        bool needHints = false,
        std::string speshializeHint = "Use 'W'&'S' + Enter/E to choose"
    );

    void drawGameOverMenu();
    void drawMainMenu();
    void drawInGame();
    void drawLevelUpMenu();
    void drawPauseMenu();
    void drawAutorsMenu();
    void drawLoadMenu(std::vector<std::string> loadMenuOptions);

    void drawLeftPanel(int x, int y, int w, int h);
    void drawFieldPanel(int x, int y, int w, int h);
    void drawRightPanel(int x, int y, int w);
    void drawWideAsciiArt(int x, int y, const std::vector<std::wstring>& art);
    void drawBoxTitle(int x, int y, int w, const std::string& title);

    void setColor(char out);
    void unsetColor(char out);
};
