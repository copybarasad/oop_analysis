#include "console_renderer.h"
#include "prints.h"

void ConsoleRenderer::render(const Game& game) {
    const GameField& field = game.getField();
    const Player& player = game.getPlayer();
    const Enemy& enemy = game.getEnemy();
    const Enemy& enemy2 = game.getEnemy2();
    const EnemyTower& enemy_tower = game.getEnemyTower();

    Prints::print_game_status(player.isAlive() ? player.getHp() : 0, player.getXp(), \
                                    (enemy.isAlive() ? enemy.getHp() : 0),
                                    (enemy2.isAlive() ? enemy2.getHp() : 0), \
                                    (enemy_tower.isAlive() ? enemy_tower.getHp() : 0));
    game.getPlayer().hand.display();
    field.showField(); 
}
