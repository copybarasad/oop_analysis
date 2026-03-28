#include "NCURSESRender.hpp"


NCURSESRender::NCURSESRender() {
    initCurses();
}


NCURSESRender::~NCURSESRender() {
    deinitCurses();
}


void NCURSESRender::initCurses() {
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


void NCURSESRender::deinitCurses() {
    if (!isendwin()) {
        endwin();
    }
}


void NCURSESRender::drawBoxTitle(int x, int y, int w, const std::string& title) {
    mvhline(y, x, ' ', w);
    mvprintw(y, x + (w - (int)title.size())/2, "%s", title.c_str());
}


void NCURSESRender::setColor(char out) {
    if (out == 'E' || out == 'T' || out == 'B' || out == '&') {
        attron(COLOR_PAIR(4));
    }
    if (out == 'P') {
        attron(COLOR_PAIR(3));
    }
}


void NCURSESRender::unsetColor(char out) {
    if (out == 'E' || out == 'T' || out == 'B' || out == '&') {
        attroff(COLOR_PAIR(4));
    }
    if (out == 'P') {
        attroff(COLOR_PAIR(3));
    }
}


void NCURSESRender::updateTermSize() {
    getmaxyx(stdscr, term_h, term_w);
}


void NCURSESRender::drawWideAsciiArt(int x, int y, const std::vector<std::wstring>& art) {
    for (size_t i = 0; i < art.size(); ++i) {
        mvwaddwstr(stdscr, y + i, x, art[i].c_str());
    }
}


void NCURSESRender::drawPatternSelectedMenu(
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


void NCURSESRender::drawGameOverMenu() {
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
    drawPatternSelectedMenu(gameOverArt, gameOverOptions, 4, GlobalMenuSelector::gameOverSelected, true);
}


void NCURSESRender::drawMainMenu() {
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
    drawPatternSelectedMenu(mainMenuArt, mainMenuOptions, 5, GlobalMenuSelector::mainMenuSelected, true);
}


void NCURSESRender::drawLevelUpMenu(std::shared_ptr<PlayerData> &playerData, std::vector<EnemyData> &enemyData) {
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
    drawPatternSelectedMenu(levelUpArt, levelUpOptions, 5, GlobalMenuSelector::levelUpSelected, true);
    int left_w = term_w / 4;
    int right_w = term_w / 4;
    int center_w = term_w - left_w - right_w - 2;
    int top = 1;
    int height = term_h - 2;
    for (int y = top; y <= top + height; ++y) {
        mvaddch(y, left_w, '|');
        mvaddch(y, left_w + 1 + center_w, '|');
    }
    drawLeftPanel(playerData, enemyData, 0, top, left_w, height);
    drawRightPanel(playerData, left_w + 1 + center_w + 1, top, right_w);
}


void NCURSESRender::drawInGame(
    std::vector<wchar_t> &fieldChars,
    std::shared_ptr<PlayerData> &playerData,
    std::vector<EnemyData> &enemyData) {
    clear();
    int left_w = term_w / 4;
    int right_w = term_w / 4;
    int center_w = term_w - left_w - right_w - 2;
    int top = 1;
    int height = term_h - 2;
    for (int y = top; y <= top + height; ++y) {
        mvaddch(y, left_w, '|');
        mvaddch(y, left_w + 1 + center_w, '|');
    }
    drawLeftPanel(playerData, enemyData, 0, top, left_w, height);
    drawFieldPanel(fieldChars, left_w + 1, top, center_w, height);
    drawRightPanel(playerData, left_w + 1 + center_w + 1, top, right_w);
    std::string hint = "WASD/arrows - move | Q - swap weapon | E - use item | Esc - menu | 1-4 - swap spell";
    mvprintw(term_h - 1, (term_w - (int)hint.size()) / 2, "%s", hint.c_str());
    refresh();
}


void NCURSESRender::drawLeftPanel(std::shared_ptr<PlayerData>& playerData, std::vector<EnemyData>& enemyData, int x, int y, int w, int h) {
    int half = h / 2;
    int cur_y = y;
    drawBoxTitle(x, cur_y, w, " PLAYER INFO ");
    cur_y++;
    float health = (playerData->playerMaxHealth > 0)
        ? (float)playerData->playerHealth / (float)playerData->playerMaxHealth : 0.0f;
    float currentExp = playerData->playerCurrentExperience - playerData->playerPrevLevelUpExperience;
    float expToLvlUp = playerData->playerLevelUpExperience - playerData->playerPrevLevelUpExperience;
    float exp = currentExp/expToLvlUp;
    int bar_w = w - 4;
    int health_filled = (int)(bar_w * health);
    int exp_filled = (int)(bar_w * exp);
    mvprintw(cur_y++, x + 1, "Health: %d/%d", playerData->playerHealth, playerData->playerMaxHealth);
    mvprintw(cur_y, x + 1, "[");
    attron(COLOR_PAIR(4));
    for (int i = 0; i < bar_w; ++i) {
        if (i < health_filled) addch('=');
        else addch(' ');
    }
    attroff(COLOR_PAIR(4));
    addch(']');
    cur_y += 2;
    mvprintw(cur_y++, x + 1, "Level: %d", playerData->playerLevel);
    mvprintw(cur_y++, x + 1, "Exp: %lld/%lld", playerData->playerCurrentExperience, playerData->playerLevelUpExperience);
    mvprintw(cur_y, x + 1, "[");
    attron(COLOR_PAIR(5));
    for (int i = 0; i < bar_w; ++i) {
        if (i < exp_filled) addch('=');
        else addch(' ');
    }
    attroff(COLOR_PAIR(5));
    addch(']');
    cur_y += 2;
    mvprintw(cur_y++, x + 1, "Attack: %d", playerData->playerAttack);
    mvprintw(cur_y++, x + 1, "Weapon: %s", playerData->playerWeapon.c_str());
    mvprintw(cur_y++, x + 1, "Attributes");
    mvprintw(cur_y++, x + 1, "int: %s str: %s dex: %s",
        std::to_string(playerData->playerIntelligence).c_str(),
        std::to_string(playerData->playerStrength).c_str(),
        std::to_string(playerData->playerDexterity).c_str()
    );

    cur_y = y + half;
    drawBoxTitle(x, cur_y, w, " ENEMIES ");
    cur_y++;
    for (size_t i = 0; i < enemyData.size() && cur_y < y + h - 1; ++i) {
        mvprintw(cur_y++, x + 1, "%s %d/%d atk:%d",
                    enemyData[i].name.c_str(),
                    enemyData[i].enemyHealth,
                    enemyData[i].enemyMaxHealth,
                    enemyData[i].enemyAttack);
    }
}


void NCURSESRender::drawFieldPanel(std::vector<wchar_t>& fieldChars, int x, int y, int w, int h) {
    int fw = GlobalGameConfig::fieldWidth;
    int fh = GlobalGameConfig::fieldHeight;

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


void NCURSESRender::drawRightPanel(std::shared_ptr<PlayerData>& playerData, int x, int y, int w) {
    drawBoxTitle(x, y, w, " HAND ");
    int cur_y = y + 1;
    for (int i = 0; i < 4; ++i) {
        auto item = playerData->playerHandItem[i];
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



void NCURSESRender::drawPauseMenu() {
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
    drawPatternSelectedMenu(pauseArt, pauseOptions, 5, GlobalMenuSelector::pauseMenuSelected, true);
}


void NCURSESRender::drawAutorsMenu() {
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


void NCURSESRender::drawLoadMenu(std::vector<std::string> loadMenuOptions) {
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
    drawPatternSelectedMenu(loadMenuArt, loadMenuOptions, 5, GlobalMenuSelector::loadMenuSelected, true, GlobalMenuSelector::errMessage);
}