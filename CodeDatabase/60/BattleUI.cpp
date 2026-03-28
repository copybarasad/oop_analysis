#include <BattleUI.hpp>
#include <Constants.hpp>
#include <Utils.hpp>

BattleUI::BattleUI(const ScreenSize* const screen, std::shared_ptr<Player> player, std::shared_ptr<Enemy> enemy, int& selected_option, BattleController::BattleTurn& turn) : screen(screen), player(player), enemy(enemy), selected_option(selected_option), turn(turn) {
    pl_bar = newwin(screen->yMax / 5, screen->xMax / 6, 0, 0);
    en_bar = newwin(screen->yMax / 5, screen->xMax / 6, 0, screen->xMax - screen->xMax / 6);
    act_bar = newwin(screen->yMax / 5, screen->xMax / 6, screen->yMax - screen->yMax / 5, 0);

    options = {"Attack", "Change weapon", "Skip turn"};
}

BattleUI::~BattleUI() {
    delwin(pl_bar);
    delwin(en_bar);
    delwin(act_bar);
    clear();
}

const std::vector<std::string>& BattleUI::getOptions() const {
    return options;
}

void BattleUI::drawPlayerBar() {
    wborder(pl_bar, '|', '|', '=', '=', '#', '#', '#', '#');
    mvwhline(pl_bar, 2, 1, ACS_HLINE, screen->xMax / 6 - 2);
    mvwaddch(pl_bar, 2, 0, '+');
    mvwaddch(pl_bar, 2, screen->xMax / 6 - 1, '+');

    wborder(en_bar, '|', '|', '=', '=', '#', '#', '#', '#');
    mvwhline(en_bar, 2, 1, ACS_HLINE, screen->xMax / 6 - 2);
    mvwaddch(en_bar, 2, 0, '+');
    mvwaddch(en_bar, 2, screen->xMax / 6 - 1, '+');

    wborder(act_bar, '|', '|', '-', '-', '+', '+', '+', '+');
}

void BattleUI::updateBattleWindows() {
    werase(pl_bar);
    werase(en_bar);
    werase(act_bar);
    
    int width = screen->xMax;

    drawPlayerBar();

    int playerHP = player->getHealth() > 0 ? player->getHealth() : 0;
    int enemyHP = enemy->getHealth() > 0 ? enemy->getHealth() : 0;

    mvwprintw(pl_bar, 1, width/12 - 3, "KNIGHT");
    mvwprintw(pl_bar, 3, 5, "Health: %d", playerHP);
    if (player->getMode() == MELEE_MODE) {
        mvwprintw(pl_bar, 4, 5, "MODE: MELEE");
    } else {
        mvwprintw(pl_bar, 4, 5, "MODE: RANGE");
    }
    mvwprintw(en_bar, 1, width/12 - 2, "ENEMY");
    mvwprintw(en_bar, 3, 5, "Health: %d", enemyHP);

    for (int i = 0; i < options.size(); i++) {
        if (selected_option == i) {
            wattron(act_bar, A_REVERSE);
            mvwprintw(act_bar, 1 + i, width / 6 / 4, "%s", options[i].c_str());
            wattroff(act_bar, A_REVERSE);
        } else {
            mvwprintw(act_bar, 1 + i, width / 6 / 4, "%s", options[i].c_str());
        }
    }

    wrefresh(act_bar);
    wrefresh(pl_bar);
    wrefresh(en_bar);
}

void BattleUI::drawBattleScreen() {
    clear();

    int centerY = screen->yMax / 2;
    int centerX = screen->xMax / 2;

    if (player->getHealth() > 0) {
        if (player->getMode() == MELEE_MODE) {
            Utils::printMultiline(centerY + BATTLE_Y_OFFSET, centerX + PLAYER_X_OFFSET, Utils::readFileToString(KNIGHT_MELEE_BATTLE));
        } else {
            Utils::printMultiline(centerY + BATTLE_Y_OFFSET, centerX + PLAYER_X_OFFSET, Utils::readFileToString(KNIGHT_RANGED_BATTLE));
        }
    } else {
        Utils::printMultiline(centerY + BATTLE_Y_OFFSET, centerX + PLAYER_X_OFFSET, Utils::readFileToString(GRAVE));
    }

    if (enemy->getHealth() > 0) {
        Utils::printMultiline(centerY + BATTLE_Y_OFFSET, centerX + ENEMY_X_OFFSET, Utils::readFileToString(GOBLIN_BATTLE));
    } else {
        Utils::printMultiline(centerY + BATTLE_Y_OFFSET, centerX + ENEMY_X_OFFSET, Utils::readFileToString(GRAVE));
    }
    if (turn == BattleController::BattleTurn::PLAYER) {
        Utils::printMultiline(0, centerX - 40 + PLAYER_TURN_X_OFFSET, Utils::readFileToString(PLAYER_TURN_TXT));
    } else {
        Utils::printMultiline(0, centerX - 40 + ENEMY_TURN_X_OFFSET, Utils::readFileToString(ENEMY_TURN_TXT));
    }

    refresh();
    updateBattleWindows();
}