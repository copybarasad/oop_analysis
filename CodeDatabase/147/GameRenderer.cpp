#include "GameRenderer.h"
#include "Game.h"

void GameRenderer::render(const Game& game, const GameController& controller) const {
    const GameField* field = controller.getField();
    if (field) {
        field->display();
    }
    const Player& player = game.getPlayer();
    std::cout << "Игрок: HP=" << player.get_health()
              << " DMG=" << player.get_damage()
              << " Очки=" << player.get_score() << "\n";
    std::cout << "Уровень: " << game.getCurrentLevel()
              << ", Нужно очков: " << game.getRequiredPoints() << "\n";
}
