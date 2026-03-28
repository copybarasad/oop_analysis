#include "StatsRenderer.h"

void StatsRenderer::PrintPlayersSpells(GameControl& controller){
    auto& player = controller.getWorld().getPlayer();
    std::cout << "Заклинания игрока: " << player.getPlayerHand().getCurrentSize() << "/" << player.getPlayerHand().getMaxSize() << std::endl;
    for(int i = 0; i < 5;i++){
        if(player.getPlayerHand().getSpell(i) != nullptr){
            std::cout << player.getPlayerHand().getSpell(i)->getName() << " x" << player.getPlayerHand().getSpellNum(i) <<std::endl;
        }
    }
}

void StatsRenderer::printGameInstructions() const {
    std::cout << "\n=== УПРАВЛЕНИЕ ===" << std::endl;
    std::cout << "(w/a/s/d) - ходьба" << std::endl;
    std::cout << "m - Сменить оружие" << std::endl;
    std::cout << "f - Дальний бой" << std::endl;
    std::cout << "b - Купить заклинания" << std::endl;
    std::cout << "z - Применить заклинание" << std::endl;
    std::cout << "i - Информация о состоянии боя" << std::endl;
    std::cout << "q - Выход из игры" << std::endl;
}


void StatsRenderer::printPlayerStats(GameControl& controller) {
    const Player& player = controller.getWorld().getPlayer();
    std::cout << "\n=== СТАТИСТИКА ИГРОКА ===" << std::endl;
    std::cout << "Здоровье: " << player.getHealth() << std::endl;
    std::cout << "Очки: " << player.getScore() << std::endl;
    std::cout << "Монеты: " << player.getCoins() << std::endl;
    std::cout << "Режим боя: " 
              << (player.getCombatMode() == Player::CombatMode::MELEE ? "БЛИЖНИЙ БОЙ" : "ДАЛЬНИЙ БОЙ") 
              << std::endl;
    std::cout << "Урон в ближнем бою: " << player.getPlayerDamages().first << std::endl;
    std::cout << "Урон в дальнем бою: " << player.getPlayerDamages().second << std::endl;
    if (player.isSlowed()) {
        std::cout << "СТАТУС: ЗАМЕДЛЕН" << std::endl;
    }
}

void StatsRenderer::printBattleInformation(GameControl& controller){
    std::cout << "=== ИНФОРМАЦИЯ О СОСТОЯНИИ БОЯ ===" << std::endl;
    std::cout << "Позиция игрока: (" << controller.getWorld().getPlayer().getX() << ", " << controller.getWorld().getPlayer().getY() << ")" << std::endl;
    std::cout << "Количество врагов: " << controller.getWorld().getEnemiesCount() << std::endl;
    std::cout << "Количество моюзников: " << controller.getWorld().getAllies().size() << std::endl;
    std::cout << "Здоровье вражеской башни: " << controller.getWorld().getTower().getHealth() << std::endl;
}