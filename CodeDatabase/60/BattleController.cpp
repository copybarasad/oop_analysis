#include <BattleController.hpp>
#include <Constants.hpp>
#include <BattleUI.hpp>
#include <Floor.hpp>
#include <vector>

BattleController::BattleController(const ScreenSize* const screen, std::vector<std::vector<Ceil>>& ceils, 
    std::shared_ptr<Player> player, std::shared_ptr<Enemy> enemy, BattleTurn turn, int field_height, int field_width)
    : screen(screen),
      ceils(ceils),
      player(player),
      enemy(enemy),
      turn(turn),
      selected_option(0),
      animator(screen, player, enemy),
      ui(std::make_unique<BattleUI>(screen, player, enemy, selected_option, this->turn)),
      battleIsOver(false),
      battleResult(0),
      field_height(field_height),
      field_width(field_width) 
{}

BattleController::~BattleController() = default;

BattleController::BattleTurn BattleController::handlePlayerTurn() {
    flushinp();
    int ch = getch();
    const auto& options = ui->getOptions();
    switch (ch) {
        case KEY_UP:
            if (selected_option > 0) selected_option--;
            break;
        case KEY_DOWN:
            if (selected_option < options.size() - 1) selected_option++;
            break;
        case ENTER:
            if (options[selected_option] == "Attack") {
                napms(TIME_BETWEEN_ANIMATIONS);
                animator.playPlayerAttackAnimation();

                int damage = player->attack();
                enemy->takeDamage(damage);

                if (enemy->getHealth() <= 0) {
                    battleIsOver = true;
                    battleResult = 1; 
                }
                return BattleTurn::ENEMY; 
            } else if (options[selected_option] == "Change weapon") {
                player->switchMode();
                return BattleTurn::PLAYER; 
            } else if (options[selected_option] == "Skip turn") {
                return BattleTurn::ENEMY;
            }
            break;
    }
    return BattleController::BattleTurn::PLAYER; 
}

BattleController::BattleTurn BattleController::handleEnemyTurn() {
    refresh();

    bool blocked = animator.playEnemyAttackAnimation();

    if (!blocked) {

        player->takeDamage(enemy->attack());

        if (player->getHealth() <= 0) {
            ceils[player->getY() - screen->yMax/2 + this->field_height/2][player->getX() - screen->xMax/2 + this->field_width/2].setEntity(std::make_shared<Floor>(player->getX(), player->getY()));
            battleIsOver = true;
            battleResult = 0; 
        }
    } else {

    }

    return BattleController::BattleTurn::PLAYER; 
}


bool BattleController::start() {

    ui->drawBattleScreen();
    while (!battleIsOver) {
        if (turn == BattleTurn::PLAYER) {
            turn = handlePlayerTurn();
        } else {
            turn = handleEnemyTurn();
        }
        ui->drawBattleScreen();
    }
    
    napms(1500);

    return battleResult;
}