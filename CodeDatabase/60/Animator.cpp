#include <Animator.hpp>
#include <Constants.hpp>
#include <Utils.hpp>
#include <chrono>

Animator::Animator(const ScreenSize* const screen, std::shared_ptr<Player> player, std::shared_ptr<Enemy> enemy) : screen(screen), player(player), enemy(enemy) {}

void Animator::playPlayerAttackAnimation() {
    int y = screen->yMax/2 - 15;
    int x = screen->xMax/2 - 50;
    if (player->getMode() == MELEE_MODE) {
        Utils::printMultiline(y, x, Utils::readFileToString(KNIGHT_ATTACK1));
        refresh();
        napms(TIME_BETWEEN_ANIMATIONS);

        Utils::printMultiline(y, x, Utils::readFileToString(KNIGHT_ATTACK2));
        refresh();
        napms(TIME_BETWEEN_ANIMATIONS);

        Utils::printMultiline(y, x, Utils::readFileToString(KNIGHT_ATTACK3));
        refresh();
        napms(TIME_BETWEEN_ANIMATIONS);
    } else {
        Utils::printMultiline(y, x, Utils::readFileToString(KNIGHT_SHOOT1));
        refresh();
        napms(TIME_BETWEEN_ANIMATIONS);

        Utils::printMultiline(y, x, Utils::readFileToString(KNIGHT_SHOOT2));
        refresh();
        napms(TIME_BETWEEN_ANIMATIONS);
    }
}

bool Animator::playEnemyAttackAnimation() {
    bool blocked = false;
    int y = screen->yMax/2 - 15;
    int x = screen->xMax/2 + 13;
    napms(500);
    if (player->getMode() == MELEE_MODE) {
        Utils::printMultiline(y, x, Utils::readFileToString(GOBLIN_ATTACK1));
        refresh();
        nodelay(stdscr, TRUE);
        auto start_time = std::chrono::steady_clock::now();

        while (true) {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count();

            if (elapsed >= BLOCK_WINDOW_MS) break;

            int ch = getch();
            if (ch == 'e' || ch == 'E') {
                blocked = true;

                Utils::printMultiline(screen->yMax / 2 + BATTLE_Y_OFFSET, 
                               screen->xMax / 2 + PLAYER_X_OFFSET, 
                               Utils::readFileToString(KNIGHT_BLOCK));
                refresh();
                napms(TIME_BETWEEN_ANIMATIONS);
                break;
            }
            napms(10); 
        }
    } else {
        Utils::printMultiline(y, x, Utils::readFileToString(GOBLIN_ATTACK1));
        refresh();
        napms(BLOCK_WINDOW_MS);
    }

    int block_y = y - 2;
    int block_x = x + 10;
    if (!blocked) {
        mvprintw(block_y, block_x, "not blocked!");
    } else {
        mvprintw(block_y, block_x, "blocked!");
    }

    Utils::printMultiline(y, x, Utils::readFileToString(GOBLIN_ATTACK2));
    refresh();
    napms(TIME_BETWEEN_ANIMATIONS);

    nodelay(stdscr, FALSE);
    return blocked;
}