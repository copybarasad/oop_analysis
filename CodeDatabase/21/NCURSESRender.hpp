#pragma once
#include <vector>
#include <string>
#include <ncurses.h>
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <wchar.h>
#include <locale.h>
#include "../Config.hpp"

class NCURSESRender {
private:
    void deinitCurses();
    void initCurses();
    void drawLeftPanel(std::shared_ptr<PlayerData>& playerData, std::vector<EnemyData>& enemyData, int x, int y, int w, int h);
    void drawFieldPanel(std::vector<wchar_t>& fieldChars, int x, int y, int w, int h);
    void drawRightPanel(std::shared_ptr<PlayerData>& playerData, int x, int y, int w);
    void drawWideAsciiArt(int x, int y, const std::vector<std::wstring>& art);
    void drawBoxTitle(int x, int y, int w, const std::string& title);
    void setColor(char out);
    void unsetColor(char out);
    void drawPatternSelectedMenu(
        std::vector<std::wstring>& asciiArt,
        std::vector<std::string>& selectedOptions,
        int artColor,
        int selector,
        bool needHints = false,
        std::string speshializeHint = "Use 'W'&'S' + Enter/E to choose"
    );
    int term_w = 0;
    int term_h = 0;
public:
    void updateTermSize();
    void drawGameOverMenu();
    void drawMainMenu();
    void drawInGame(std::vector<wchar_t> &fieldChars, std::shared_ptr<PlayerData> &playerData, std::vector<EnemyData> &enemyData);
    void drawLevelUpMenu(std::shared_ptr<PlayerData> &playerData, std::vector<EnemyData> &enemyData);
    void drawPauseMenu();
    void drawAutorsMenu();
    void drawLoadMenu(std::vector<std::string> loadMenuOptions);
    NCURSESRender();
    ~NCURSESRender();
};
