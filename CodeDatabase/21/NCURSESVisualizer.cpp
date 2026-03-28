#include "NCURSESVisualizer.hpp"


NCURSESVisualizer::NCURSESVisualizer() {
    initCurses();
    last_frame_time = std::chrono::steady_clock::now();
}


NCURSESVisualizer::~NCURSESVisualizer() {
    deinitCurses();
}


void NCURSESVisualizer::setInputController(InputController* ic) {
    inputController = ic;
}


void NCURSESVisualizer::setGameController(IGameController* gc) {
    gameController = gc;
}


void NCURSESVisualizer::initCurses() {
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);
    start_color();
    use_default_colors();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_BLACK, COLOR_YELLOW);
    init_pair(3, COLOR_GREEN, -1);
    init_pair(4, COLOR_RED, -1);
    init_pair(5, COLOR_CYAN, -1);
    updateTermSize();
    set_escdelay(25);
    erase();
    refresh();
}


void NCURSESVisualizer::deinitCurses() {
    if (!isendwin()) {
        endwin();
    }
}


void NCURSESVisualizer::drawBoxTitle(int x, int y, int w, const std::string& title) {
    mvhline(y, x, ' ', w);
    mvprintw(y, x + (w - (int)title.size())/2, "%s", title.c_str());
}


void NCURSESVisualizer::setColor(char out) {
    if (out == 'E' || out == 'T' || out == 'B' || out == '&') {
        attron(COLOR_PAIR(4));
    }
    if (out == 'P') {
        attron(COLOR_PAIR(3));
    }
}


void NCURSESVisualizer::unsetColor(char out) {
    if (out == 'E' || out == 'T' || out == 'B' || out == '&') {
        attroff(COLOR_PAIR(4));
    }
    if (out == 'P') {
        attroff(COLOR_PAIR(3));
    }
}


void NCURSESVisualizer::updateTermSize() {
    getmaxyx(stdscr, term_h, term_w);
}


int NCURSESVisualizer::fetchInput() {
    int input = 0;
    if (inputController) {
        input = inputController->getInputChar();
    }
    return input;
}


void NCURSESVisualizer::drawWideAsciiArt(int x, int y, const std::vector<std::wstring>& art) {
    for (size_t i = 0; i < art.size(); ++i) {
        mvwaddwstr(stdscr, y + i, x, art[i].c_str());
    }
}


void NCURSESVisualizer::display() {
    state = GameState::MainMenu;
    while (state != GameState::Exit) {
        updateTermSize();
        switch (state) {
            case GameState::MainMenu:
                loopMainMenu();
                break;
            case GameState::InGame:
                loopInGame();
                break;
            case GameState::LevelUpMenu:
                loopLevelUp();
                break;
            case GameState::GameOver:
                loopGameOverMenu();
                break;
            case GameState::PauseMenu:
                loopPauseMenu();
                break;
            case GameState::AutorsMenu:
                loopAutorsMenu();
                break;
            case GameState::LoadMenu:
                loopLoadMenu();
                break;
            default:
                break;
        }
    }
}


void NCURSESVisualizer::drawPatternSelectedMenu(
    std::vector<std::wstring>& asciiArt,
    std::vector<std::string>& selectedOptions,
    int artColor,
    int selector,
    bool needHints,
    std::string speshializeHint
) {
    erase();
    int art_x = (term_w - (int)asciiArt[0].size()) / 2;
    int art_y = 2;
    attron(COLOR_PAIR(artColor));
    drawWideAsciiArt(art_x, art_y, asciiArt);
    attroff(COLOR_PAIR(artColor));
    int box_w = 40;
    int box_h = (int)selectedOptions.size() + 4;
    int bx = (term_w - box_w) / 2;
    int by = (term_h - box_h) / 3 * 2;
    for (int i = 0; i < box_w; ++i) mvaddch(by, bx + i, '-');
    for (int i = 0; i < box_w; ++i) mvaddch(by + box_h - 1, bx + i, '-');
    for (int i = 0; i < box_h; ++i) {
        mvaddch(by + i, bx, '|');
        mvaddch(by + i, bx + box_w - 1, '|');
    }
    mvaddch(by, bx, '+'); mvaddch(by, bx + box_w - 1, '+');
    mvaddch(by + box_h - 1, bx, '+'); mvaddch(by + box_h - 1, bx + box_w - 1, '+');
    if (needHints) {
        std::string title = "Choose action";
        mvprintw(by + 1, bx + (box_w - (int)title.size())/2, "%s", title.c_str());
        mvprintw(by + box_h, (term_w - (int)speshializeHint.size()) / 2, "%s", speshializeHint.c_str());
    }

    for (size_t i = 0; i < selectedOptions.size(); ++i) {
        int iy = by + 2 + (int)i;
        if ((int)i == selector) {
            attron(COLOR_PAIR(artColor));
            mvprintw(iy, bx + 2, " %s ", selectedOptions[i].c_str());
            attroff(COLOR_PAIR(artColor));
        } else {
            mvprintw(iy, bx + 1, " %s ", selectedOptions[i].c_str());
        }
    }
    refresh();
}


void NCURSESVisualizer::loopGameOverMenu() {
    auto frame_start = std::chrono::steady_clock::now();
    drawGameOverMenu();
    int input = fetchInput();
    if (input) {
        switch (input) {
            case KEY_UP:
            case 'w': case 'W':
                gameOverSelected = (gameOverSelected - 1 + 3) % 3;
                break;
            case KEY_DOWN:
            case 's': case 'S':
                gameOverSelected = (gameOverSelected + 1) % 3;
                break;
            case '\n': case 'e': case KEY_ENTER:
                if (gameController) {
                    if (gameOverSelected == 0) {
                        gameController->startNewGame();
                        state = GameState::InGame;
                    }
                    else if (gameOverSelected == 1) {
                        state = GameState::LoadMenu;
                        prevState = GameState::GameOver;
                    }
                    else if (gameOverSelected == 2) {
                        state = GameState::MainMenu;
                    }
                }
                gameOverSelected = 0;
                break;
            default:
                break;
        }
    }
    auto frame_end = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(frame_end - frame_start).count();
    if (ms < frame_ms) std::this_thread::sleep_for(std::chrono::milliseconds(frame_ms - ms));
}


void NCURSESVisualizer::drawGameOverMenu() {
    std::vector<std::wstring> gameOverArt = {
        L" _____                                                                              _____ ",
        L"( ___ )----------------------------------------------------------------------------( ___ )",
        L" |   |                                                                              |   | ",
        L" |   | ▄██   ▄    ▄██████▄  ███    █▄       ████████▄   ▄█     ▄████████ ████████▄  |   | ",
        L" |   | ███   ██▄ ███    ███ ███    ███      ███   ▀███ ███    ███    ███ ███   ▀███ |   | ",
        L" |   | ███▄▄▄███ ███    ███ ███    ███      ███    ███ ███▌   ███    █▀  ███    ███ |   | ",
        L" |   | ▀▀▀▀▀▀███ ███    ███ ███    ███      ███    ███ ███▌  ▄███▄▄▄     ███    ███ |   | ",
        L" |   | ▄██   ███ ███    ███ ███    ███      ███    ███ ███▌ ▀▀███▀▀▀     ███    ███ |   | ",
        L" |   | ███   ███ ███    ███ ███    ███      ███    ███ ███    ███    █▄  ███    ███ |   | ",
        L" |   | ███   ███ ███    ███ ███    ███      ███   ▄███ ███    ███    ███ ███   ▄███ |   | ",
        L" |   |  ▀█████▀   ▀██████▀  ████████▀       ████████▀  █▀     ██████████ ████████▀  |   | ",
        L" |___|                                                                              |___| ",
        L"(_____)----------------------------------------------------------------------------(_____)"
    };
    std::vector<std::string> gameOverOptions = {
        "New game",
        "Load game",
        "Main menu"
    };
    drawPatternSelectedMenu(gameOverArt, gameOverOptions, 4, gameOverSelected, true);
}


void NCURSESVisualizer::loopMainMenu() {
    auto frame_start = std::chrono::steady_clock::now();
    drawMainMenu();
    int input = fetchInput();
    if (input) {
        switch (input) {
            case KEY_UP: case 'w':
                mainMenuSelected = (mainMenuSelected - 1 + 4) % 4;
                break;
            case KEY_DOWN: case 's':
                mainMenuSelected = (mainMenuSelected + 1) % 4;
                break;
            case '\n': case 'e': case KEY_ENTER:
                if (mainMenuSelected == 0) {
                    if (gameController) gameController->startNewGame();
                    state = GameState::InGame;
                } else if (mainMenuSelected == 1) {
                    if (gameController) gameController->startNewGame();
                    state = GameState::LoadMenu;
                    prevState = GameState::MainMenu;
                } else if (mainMenuSelected == 2) {
                    state = GameState::AutorsMenu;
                } else if (mainMenuSelected == 3) {
                    state = GameState::Exit;
                }
                mainMenuSelected = 0;
                break;
            default:
                break;
        }
    }
    auto frame_end = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(frame_end - frame_start).count();
    if (ms < frame_ms) std::this_thread::sleep_for(std::chrono::milliseconds(frame_ms - ms));
}


void NCURSESVisualizer::drawMainMenu() {
    std::vector<std::wstring> mainMenuArt = {
        L" _____                                                                                                 _____ ",
        L"( ___ )-----------------------------------------------------------------------------------------------( ___ )",
        L" |   |                                                                                                 |   | ",
        L" |   |                   ▄█    █▄       ▄████████    ▄████████  ▄██████▄     ▄████████                 |   | ",
        L" |   |                  ███    ███     ███    ███   ███    ███ ███    ███   ███    ███                 |   | ",
        L" |   |                  ███    ███     ███    █▀    ███    ███ ███    ███   ███    █▀                  |   | ",
        L" |   |                 ▄███▄▄▄▄███▄▄  ▄███▄▄▄      ▄███▄▄▄▄██▀ ███    ███   ███                        |   | ",
        L" |   |                ▀▀███▀▀▀▀███▀  ▀▀███▀▀▀     ▀▀███▀▀▀▀▀   ███    ███ ▀███████████                 |   | ",
        L" |   |                  ███    ███     ███    █▄  ▀███████████ ███    ███          ███                 |   | ",
        L" |   |                  ███    ███     ███    ███   ███    ███ ███    ███    ▄█    ███                 |   | ",
        L" |   |                  ███    █▀      ██████████   ███    ███  ▀██████▀   ▄████████▀                  |   | ",
        L" |   |                                    ███    ███                                                   |   | ",
        L" |   |         ▄████████    ▄████████  ▄█    █▄     ▄████████ ███▄▄▄▄      ▄██████▄     ▄████████      |   | ",
        L" |   |        ███    ███   ███    ███ ███    ███   ███    ███ ███▀▀▀██▄   ███    ███   ███    ███      |   | ",
        L" |   |        ███    ███   ███    █▀  ███    ███   ███    █▀  ███   ███   ███    █▀    ███    █▀       |   | ",
        L" |   |       ▄███▄▄▄▄██▀  ▄███▄▄▄     ███    ███  ▄███▄▄▄     ███   ███  ▄███         ▄███▄▄▄          |   | ",
        L" |   |      ▀▀███▀▀▀▀▀   ▀▀███▀▀▀     ███    ███ ▀▀███▀▀▀     ███   ███ ▀▀███ ████▄  ▀▀███▀▀▀          |   | ",
        L" |   |      ▀███████████   ███    █▄  ███    ███   ███    █▄  ███   ███   ███    ███   ███    █▄       |   | ",
        L" |   |        ███    ███   ███    ███ ███    ███   ███    ███ ███   ███   ███    ███   ███    ███      |   | ",
        L" |   |        ███    ███   ██████████  ▀██████▀    ██████████  ▀█   █▀    ████████▀    ██████████      |   | ",
        L" |   |        ███    ███                                                                               |   | ",
        L" |___|                                                                                                 |___| ",
        L"(_____)-----------------------------------------------------------------------------------------------(_____)",
    };
    std::vector<std::string> mainMenuOptions = { "New Game", "Load Game", "Authors", "Exit" };
    drawPatternSelectedMenu(mainMenuArt, mainMenuOptions, 5, mainMenuSelected, true);
}


void NCURSESVisualizer::loopLevelUp() {
    auto frame_start = std::chrono::steady_clock::now();
    drawLevelUpMenu();
    int input = fetchInput();
    if (input) {
        switch (input) {
            case KEY_UP:
            case 'w': case 'W':
                levelUpSelected = (levelUpSelected - 1 + 3) % 3; break;
            case KEY_DOWN:
            case 's': case 'S':
                levelUpSelected = (levelUpSelected + 1) % 3; break;
            case '\n': case 'e': case KEY_ENTER:
                if (gameController) {
                    char choice = '1' + levelUpSelected;
                    gameController->playerLevelUp(choice);
                }
                state = GameState::InGame;
                levelUpSelected = 0;
                break;
        }
    }
    auto frame_end = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(frame_end - frame_start).count();
    if (ms < frame_ms) std::this_thread::sleep_for(std::chrono::milliseconds(frame_ms - ms));
}


void NCURSESVisualizer::drawLevelUpMenu() {
    std::vector<std::wstring> levelUpArt = {
        L" _____                                                                            _____ ",
        L"( ___ )--------------------------------------------------------------------------( ___ )",
        L" |   |                                                                            |   | ",
        L" |   |  ▄█        ▄███████  ▄█   █▄     ▄███████  ▄█        ███   █▄     ▄██████▄ |   | ",
        L" |   | ███       ███   ███ ███   ███   ███   ███ ███        ███   ███   ███   ███ |   | ",
        L" |   | ███       ███   █▀  ███   ███   ███   █▀  ███        ███   ███   ███   ███ |   | ",
        L" |   | ███      ▄███▄▄▄    ███   ███  ▄███▄▄▄    ███        ███   ███   ███   ███ |   | ",
        L" |   | ███     ▀▀███▀▀▀    ███   ███ ▀▀███▀▀▀    ███        ███   ███ ▀████████▀  |   | ",
        L" |   | ███       ███   █▄  ███   ███   ███   █▄  ███        ███   ███   ███       |   | ",
        L" |   | ███▌   ▄  ███   ███ ███   ███   ███   ███ ███▌   ▄   ███   ███   ███       |   | ",
        L" |   | █████▄██  █████████  ▀█████▀    █████████ █████▄██   ███████▀   ▄████▀     |   | ",
        L" |   | ▀                                             ▀                            |   | ",
        L" |___|                                                                            |___| ",
        L"(_____)--------------------------------------------------------------------------(_____)"
    };
    std::vector<std::string> levelUpOptions = {
        "INT +10 -- spell damage",
        "STR +10 -- sword damage & life",
        "DEX +10 -- bow damage"
    };
    drawPatternSelectedMenu(levelUpArt, levelUpOptions, 5, levelUpSelected, true);
    int left_w = term_w / 4;
    int right_w = term_w / 4;
    int center_w = term_w - left_w - right_w - 2;
    int top = 1;
    int height = term_h - 2;
    for (int y = top; y <= top + height; ++y) {
        mvaddch(y, left_w, '|');
        mvaddch(y, left_w + 1 + center_w, '|');
    }
    drawLeftPanel(0, top, left_w, height);
    drawRightPanel(left_w + 1 + center_w + 1, top, right_w);
}


void NCURSESVisualizer::loopInGame() {
    auto frame_start = std::chrono::steady_clock::now();
    auto pdata = gameController->getPlayerData();
    if (pdata && pdata->levelIncreased) {
        state = GameState::LevelUpMenu;
        return;
    }
    drawInGame();
    int input = fetchInput();
    if (input) {
        if (input == 27) {
            state = GameState::PauseMenu;
            return;
        }
        char c = (char)input;
        if (gameController) {
            bool consumed = false;
            if (input == KEY_UP || c == 'w' || c == 'W') { consumed = !gameController->performAnAction('w'); }
            else if (input == KEY_DOWN || c == 's' || c == 'S') { consumed = !gameController->performAnAction('s'); }
            else if (input == KEY_LEFT || c == 'a' || c == 'A') { consumed = !gameController->performAnAction('a'); }
            else if (input == KEY_RIGHT || c == 'd' || c == 'D') { consumed = !gameController->performAnAction('d'); }
            else if (c == 'e' || c == 'E') { consumed = !gameController->performAnAction('e'); }
            else if (c == 'q' || c == 'Q') { gameController->performAnAction('q'); }
            else if (c >= '1' && c <= '4') {
                gameController->performAnAction((char)('0' + (c - '1')));
            }
            if (consumed) {
                state = GameState::GameOver;
                return;
            }
        }
    }
    auto frame_end = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(frame_end - frame_start).count();
    if (ms < frame_ms) std::this_thread::sleep_for(std::chrono::milliseconds(frame_ms - ms));
}


void NCURSESVisualizer::drawInGame() {
    erase();
    int left_w = term_w / 4;
    int right_w = term_w / 4;
    int center_w = term_w - left_w - right_w - 2;
    int top = 1;
    int height = term_h - 2;
    for (int y = top; y <= top + height; ++y) {
        mvaddch(y, left_w, '|');
        mvaddch(y, left_w + 1 + center_w, '|');
    }
    drawLeftPanel(0, top, left_w, height);
    drawFieldPanel(left_w + 1, top, center_w, height);
    drawRightPanel(left_w + 1 + center_w + 1, top, right_w);
    std::string hint = "WASD/arrows - move | Q - swap weapon | E - use item | Esc - menu | 1-4 - swap spell";
    mvprintw(term_h - 1, (term_w - (int)hint.size()) / 2, "%s", hint.c_str());
    refresh();
}




void NCURSESVisualizer::drawLeftPanel(int x, int y, int w, int h) {
    int half = h / 2;
    int cur_y = y;
    drawBoxTitle(x, cur_y, w, " PLAYER INFO ");
    cur_y++;
    if (gameController) {
        auto data = gameController->getPlayerData();
        if (data) {
            float health = (data->playerMaxHealth > 0)
                ? (float)data->playerHealth / (float)data->playerMaxHealth : 0.0f;
            float currentExp = data->playerCurrentExperience - data->playerPrevLevelUpExperience;
            float expToLvlUp = data->playerLevelUpExperience - data->playerPrevLevelUpExperience;
            float exp = currentExp/expToLvlUp;
            int bar_w = w - 4;
            int health_filled = (int)(bar_w * health);
            int exp_filled = (int)(bar_w * exp);
            mvprintw(cur_y++, x + 1, "Health: %d/%d", data->playerHealth, data->playerMaxHealth);
            mvprintw(cur_y, x + 1, "[");
            attron(COLOR_PAIR(4));
            for (int i = 0; i < bar_w; ++i) {
                if (i < health_filled) addch('=');
                else addch(' ');
            }
            attroff(COLOR_PAIR(4));
            addch(']');
            cur_y += 2;
            mvprintw(cur_y++, x + 1, "Level: %d", data->playerLevel);
            mvprintw(cur_y++, x + 1, "Exp: %lld/%lld", data->playerCurrentExperience, data->playerLevelUpExperience);
            mvprintw(cur_y, x + 1, "[");
            attron(COLOR_PAIR(5));
            for (int i = 0; i < bar_w; ++i) {
                if (i < exp_filled) addch('=');
                else addch(' ');
            }
            attroff(COLOR_PAIR(5));
            addch(']');
            cur_y += 2;
            mvprintw(cur_y++, x + 1, "Attack: %d", data->playerAttack);
            mvprintw(cur_y++, x + 1, "Weapon: %s", data->playerWeapon.c_str());
            mvprintw(cur_y++, x + 1, "Attributes");
            mvprintw(cur_y++, x + 1, "int: %s str: %s dex: %s",
                std::to_string(data->playerIntelligence).c_str(),
                std::to_string(data->playerStrength).c_str(),
                std::to_string(data->playerDexterity).c_str()
            );
        } else {
            mvprintw(cur_y++, x + 1, "(no player data)");
        }
    } else {
        mvprintw(cur_y++, x + 1, "(no game controller)");
    }
    cur_y = y + half;
    drawBoxTitle(x, cur_y, w, " ENEMIES ");
    cur_y++;
    if (gameController) {
        auto enemies = gameController->getEnemyData();
        if (!enemies.empty()) {
            for (size_t i = 0; i < enemies.size() && cur_y < y + h - 1; ++i) {
                mvprintw(cur_y++, x + 1, "%s %d/%d atk:%d",
                         enemies[i].name.c_str(),
                         enemies[i].enemyHealth,
                         enemies[i].enemyMaxHealth,
                         enemies[i].enemyAttack);
            }
        } else {
            mvprintw(cur_y++, x + 1, "(no enemies)");
        }
    } else {
        mvprintw(cur_y++, x + 1, "(no game controller)");
    }
}


void NCURSESVisualizer::drawFieldPanel(int x, int y, int w, int h) {
    std::vector<wchar_t> fieldChars;
    int fw = GlobalGameConfig::fieldWidth;
    int fh = GlobalGameConfig::fieldHeight;

    if (gameController)
        fieldChars = gameController->getFieldData();

    int render_cell_width = 2;
    int render_field_width = fw * render_cell_width;
    int render_field_height = fh;

    int visible_w = w - 2;
    int visible_h = h - 2;

    int start_x = x + 1;
    int start_y = y + 1;

    int offset_x = 0;
    int offset_y = 0;

    if (render_field_width < visible_w) {
        start_x = x + 1 + (visible_w - render_field_width) / 2;
    } else {
        offset_x = (render_field_width - visible_w) / 2;
    }
    if (render_field_height < visible_h) {
        start_y = y + 1 + (visible_h - render_field_height) / 2;
    } else {
        offset_y = (render_field_height - visible_h) / 2;
    }
    for (int ry = 0; ry < std::min(visible_h, render_field_height); ++ry) {
        int actual_ry = ry + offset_y;
        if (actual_ry >= fh) break;

        int base_idx = actual_ry * fw;

        for (int rx = 0; rx < std::min(visible_w / 2, fw); ++rx) {
            int actual_rx = rx + offset_x;
            if (actual_rx >= fw) break;

            int idx = base_idx + actual_rx;
            if (idx >= (int)fieldChars.size()) break;

            wchar_t ch = fieldChars[idx];
            char out = (ch > 0 && ch < 128) ? (char)ch : '.';

            int screen_x = start_x + rx * 2;
            int screen_y = start_y + ry;

            if (screen_x < x + w && screen_y < y + h) {
                setColor(out);
                mvaddch(screen_y, screen_x, out);
                unsetColor(out);
            }
        }
    }
}


void NCURSESVisualizer::drawRightPanel(int x, int y, int w) {
    drawBoxTitle(x, y, w, " HAND ");
    int cur_y = y + 1;

    if (gameController) {
        auto pdata = gameController->getPlayerData();
        if (pdata) {
            for (int i = 0; i < 4; ++i) {
                auto item = pdata->playerHandItem[i];
                std::string sel = item.first ? "*" : " ";
                std::string spell_name;
                switch(i) {
                    case 0: spell_name = "AOE Spell"; break;
                    case 1: spell_name = "Fireball"; break;
                    case 2: spell_name = "Upgrade"; break;
                    case 3: spell_name = "Trap"; break;
                }
                mvprintw(cur_y++, x + 1, "%d) %s %s x%d", i+1, sel.c_str(), spell_name.c_str(), item.second);
            }
        }
    } else {
        mvprintw(cur_y++, x + 1, "(no controller)");
    }
}


void NCURSESVisualizer::loopPauseMenu() {
    auto frame_start = std::chrono::steady_clock::now();
    drawPauseMenu();
    int input = fetchInput();
    if (input) {
        switch (input) {
            case KEY_UP:
            case 'w': case 'W':
                pauseMenuSelected = (pauseMenuSelected - 1 + 4) % 4;
                break;
            case KEY_DOWN:
            case 's': case 'S':
                pauseMenuSelected = (pauseMenuSelected + 1) % 4;
                break;
            case '\n': case 'e': case KEY_ENTER:
                if (gameController) {
                    if (pauseMenuSelected == 0) {
                        state = GameState::InGame;
                    }
                    else if (pauseMenuSelected == 1) {
                        state = GameState::InGame;
                        gameController->saveGame();
                    }
                    else if (pauseMenuSelected == 2) {
                        state = GameState::LoadMenu;
                        prevState = GameState::PauseMenu;
                    }
                    else if (pauseMenuSelected == 3) {
                        state = GameState::MainMenu;
                    }
                }
                pauseMenuSelected = 0;
                break;
            case '\e':
                state = GameState::InGame;
                pauseMenuSelected = 0;
            default:
                break;
        }
    }
    auto frame_end = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(frame_end - frame_start).count();
    if (ms < frame_ms) std::this_thread::sleep_for(std::chrono::milliseconds(frame_ms - ms));
}


void NCURSESVisualizer::drawPauseMenu() {
    std::vector<std::wstring> pauseArt = {
        L" _____                                                                _____ ",
        L"( ___ )--------------------------------------------------------------( ___ )",
        L" |   |                                                                |   | ",
        L" |   |    ▄███████▄    ▄████████ ███    █▄     ▄████████    ▄████████ |   | ",
        L" |   |   ███    ███   ███    ███ ███    ███   ███    ███   ███    ███ |   | ",
        L" |   |   ███    ███   ███    ███ ███    ███   ███    █▀    ███    █▀  |   | ",
        L" |   |   ███    ███   ███    ███ ███    ███   ███         ▄███▄▄▄     |   | ",
        L" |   | ▀█████████▀  ▀███████████ ███    ███ ▀███████████ ▀▀███▀▀▀     |   | ",
        L" |   |   ███          ███    ███ ███    ███          ███   ███    █▄  |   | ",
        L" |   |   ███          ███    ███ ███    ███    ▄█    ███   ███    ███ |   | ",
        L" |   |  ▄████▀        ███    █▀  ████████▀   ▄████████▀    ██████████ |   | ",
        L" |___|                                                                |___| ",
        L"(_____)--------------------------------------------------------------(_____)"
    };
    std::vector<std::string> pauseOptions = {
        "Continue",
        "Save game",
        "Load game",
        "Main menu"
    };
    drawPatternSelectedMenu(pauseArt, pauseOptions, 5, pauseMenuSelected, true);
}


void NCURSESVisualizer::loopAutorsMenu() {
    auto frame_start = std::chrono::steady_clock::now();
    drawAutorsMenu();
    int input = fetchInput();
    if (input) {
        switch (input) {
            case '\n': case 'e': case KEY_ENTER: case '\e': case 'q':
                state = GameState::MainMenu;
                break;
            default:
                break;
        }
    }
    auto frame_end = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(frame_end - frame_start).count();
    if (ms < frame_ms) std::this_thread::sleep_for(std::chrono::milliseconds(frame_ms - ms));
}


void NCURSESVisualizer::drawAutorsMenu() {
    std::vector<std::wstring> autorsArt = {
        L" _____                                                                          _____ ",
        L"( ___ )------------------------------------------------------------------------( ___ )",
        L" |   |                                                                          |   | ",
        L" |   |    ▄████████ ███    █▄      ███      ▄██████▄     ▄████████    ▄████████ |   | ",
        L" |   |   ███    ███ ███    ███ ▀█████████▄ ███    ███   ███    ███   ███    ███ |   | ",
        L" |   |   ███    ███ ███    ███    ▀███▀▀██ ███    ███   ███    ███   ███    █▀  |   | ",
        L" |   |   ███    ███ ███    ███     ███   ▀ ███    ███  ▄███▄▄▄▄██▀   ███        |   | ",
        L" |   | ▀███████████ ███    ███     ███     ███    ███ ▀▀███▀▀▀▀▀   ▀███████████ |   | ",
        L" |   |   ███    ███ ███    ███     ███     ███    ███ ▀███████████          ███ |   | ",
        L" |   |   ███    ███ ███    ███     ███     ███    ███   ███    ███    ▄█    ███ |   | ",
        L" |   |   ███    █▀  ████████▀     ▄████▀    ▀██████▀    ███    ███  ▄████████▀  |   | ",
        L" |   |                                                  ███    ███              |   | ",
        L" |___|                                                                          |___| ",
        L"(_____)------------------------------------------------------------------------(_____)"
    };
    std::vector<std::string> autors = {
        "Game developer : 4342 Larionov V.",
        "   Composer    : 4344 Kozyrev M. "
    };
    drawPatternSelectedMenu(autorsArt, autors, 5, (int)autors.size() + 1);
}


void NCURSESVisualizer::loopLoadMenu() {
    auto frame_start = std::chrono::steady_clock::now();
    static const int PAGE_SIZE = 10;
    static int startList = 0;
    static int endList = PAGE_SIZE;
    static std::vector<std::string> loadMenuOptions;
    static bool firstCall = true;
    if (firstCall) {
        firstCall = false;
    }
    loadMenuOptions = gameController->getSavesList(startList, endList);
    for (auto& str : loadMenuOptions) {
        str = str.substr(std::min(9, (int)str.length()));
    }
    drawLoadMenu(loadMenuOptions);
    int input = fetchInput();

    if (input) {
        switch (input) {
            case KEY_UP:
            case 'w': case 'W':
                if (loadMenuSelected == 0 && startList > 0) {
                    startList = std::max(0, startList - PAGE_SIZE);
                    endList = startList + PAGE_SIZE;
                    loadMenuOptions = gameController->getSavesList(startList, endList);
                    loadMenuSelected = (int)loadMenuOptions.size() - 1;
                } else {
                    loadMenuSelected = std::max(0, loadMenuSelected - 1);
                }
                break;

            case KEY_DOWN:
            case 's': case 'S':
                if (loadMenuSelected == (int)loadMenuOptions.size() - 1) {
                    int totalSaves = (int)gameController->getSavesList(0,-1).size();
                    if (endList < totalSaves) {
                        startList = endList;
                        endList = std::min(startList + PAGE_SIZE, totalSaves);
                        loadMenuOptions = gameController->getSavesList(startList, endList);
                        loadMenuSelected = 0;
                    }
                } else {
                    loadMenuSelected = std::min((int)loadMenuOptions.size() - 1, loadMenuSelected + 1);
                }
                break;

            case '\n': case 'e': case KEY_ENTER:
                if (gameController && !loadMenuOptions.empty()) {
                    std::string selectedSave = loadMenuOptions[loadMenuSelected];
                    try {
                        if (gameController->loadGame(selectedSave)) {
                            startList = 0;
                            endList = PAGE_SIZE;
                            loadMenuSelected = 0;
                            firstCall = true;
                            state = GameState::InGame;
                            errMessage = "";
                        }
                        else {
                            errMessage = "failed to load save";
                        }
                    }
                    catch(const std::exception& e) {
                        errMessage = e.what();
                    }
                }
                break;

            case '\e':
                startList = 0;
                endList = PAGE_SIZE;
                loadMenuSelected = 0;
                firstCall = true;
                state = prevState;
                break;

            default:
                break;
        }
    }

    auto frame_end = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(frame_end - frame_start).count();
    if (ms < frame_ms) std::this_thread::sleep_for(std::chrono::milliseconds(frame_ms - ms));
}


void NCURSESVisualizer::drawLoadMenu(std::vector<std::string> loadMenuOptions) {
    std::vector<std::wstring> loadMenuArt = {
        L" _____                                              _____ ",
        L"( ___ )--------------------------------------------( ___ )",
        L" |   |                                              |   | ",
        L" |   |  ▄█        ▄██████▄     ▄████████ ████████▄  |   | ",
        L" |   | ███       ███    ███   ███    ███ ███   ▀███ |   | ",
        L" |   | ███       ███    ███   ███    ███ ███    ███ |   | ",
        L" |   | ███       ███    ███   ███    ███ ███    ███ |   | ",
        L" |   | ███       ███    ███ ▀███████████ ███    ███ |   | ",
        L" |   | ███       ███    ███   ███    ███ ███    ███ |   | ",
        L" |   | ███▌    ▄ ███    ███   ███    ███ ███   ▄███ |   | ",
        L" |   | █████▄▄██  ▀██████▀    ███    █▀  ████████▀  |   | ",
        L" |   | ▀                                            |   | ",
        L" |___|                                              |___| ",
        L"(_____)--------------------------------------------(_____)",
    };
    drawPatternSelectedMenu(loadMenuArt, loadMenuOptions, 5, loadMenuSelected, true, errMessage);
}