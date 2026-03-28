#include "LevelController.h"
#include "Game.h"
#include <iostream>
#include <limits>

void LevelController::initLevel(Game& game) {
    // считаем величину поля
    int width  = 10 + (game.getCurrentLevel() - 1) * 2;
    int height = 10 + (game.getCurrentLevel() - 1) * 2;

    game.getField() = Field(width, height);
    game.getEntityManager() = EntityManager();

    Player& player = game.getPlayer();

    // ставим игрока в центр
    int startX = width / 2 - 1;
    int startY = height / 2 - 1;

    player.setPosition(startX, startY);
    game.getField().setCellEntity(&player, startX, startY);

    int enemiesCount = 3 + (game.getCurrentLevel() - 1) * 2;
    int towersCount  = 2 + (game.getCurrentLevel() - 1);

    game.getEntityManager().initTowers(game.getField(), towersCount);
    game.getEntityManager().initEnemies(game.getField(), enemiesCount, game.getCurrentLevel());

    std::cout << "Запуск уровня " << game.getCurrentLevel()
              << ". Размер поля: " << width << "x" << height
              << ", врагов: " << enemiesCount
              << ", башен: " << towersCount << "\n";
}

void LevelController::levelUp(Game& game) {
    Player& player = game.getPlayer();
    PlayerHand& hand = game.getHand();

    std::cout << "\n----- ПОВЫШЕНИЕ УРОВНЯ -----\n";
    std::cout << "Выберите улучшение:\n"
                 "1) +5 к максимальному здоровью\n"
                 "2) +2 к ближнему урону\n"
                 "3) +3 к дальнему урону\n"
                 "4) +5 к мане\n"
                 "5) +4 к урону Fireball\n"
                 "6) +4 к урону Explosion\n"
                 "7) +4 к урону Trap\n";

    int choice = 0;
    int choice_flag = 1;
    while (choice_flag) {
        std::cout << "Ваш выбор: ";
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Введите число 1–7.\n";
            continue;
        }
        if (choice >= 1 && choice <= 7){
            choice_flag = 0;
        } else{
            std::cout << "Некорректный выбор.\n";
        }
        
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    switch (choice) {
        case 1: player.increaseMaxHealth(5);    break;
        case 2: player.increaseMeleeDamage(2);  break;
        case 3: player.increaseRangeDamage(3);  break;
        case 4: player.increaseMaxMana(5);      break;
        case 5: hand.incFireballSpellDmg(4);    break;
        case 6: hand.incExplosionSpellDmg(4);   break;
        case 7: hand.incTrapSpellDmg(4);        break;
    }

    player.healToFull();
    player.manaToFull();

    std::cout << "Здоровье восстановлено: " << player.getHp() << "\n";

    hand.removeRandomHalf();

    std::cout << "Половина заклинаний удалена.\n";
}
