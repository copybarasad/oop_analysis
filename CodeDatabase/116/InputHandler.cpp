#include "InputHandler.h"
#include "GameControl.h"
#include "StatsRenderer.h"
#include "../spells/SpellManager.h" 

char InputHandler::getCommand(const std::string& prompt) {
    char input;
    std::cout << prompt;
    std::cin >> input;
    clearInputBuffer();
    return input;
}

void InputHandler::waitForEnter(const std::string& message) {
    std::cout << message;
    clearInputBuffer();
}

void InputHandler::clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void InputHandler::processInput(GameControl& controller, StatsRenderer& statsRenderer, bool& gameRunning) {
    char input = getCommand();
    Player& player = controller.getWorld().getPlayer();
    tookTurn = false;
    
    switch(input) {
        case 'w': case 'W':
            controller.movePlayer(0, -1, tookTurn);
            break;
        case 's': case 'S':
            controller.movePlayer(0, 1, tookTurn);
            break;
        case 'a': case 'A':
            controller.movePlayer(-1, 0, tookTurn);
            break;
        case 'd': case 'D':
            controller.movePlayer(1, 0, tookTurn);
            break;
        case 'f': case 'F':{
            if (player.getCombatMode() == Player::CombatMode::RANGED){
                std::cout << "Выберите направление атаки (w/a/s/d): ";
                char attackDirection = getCommand("");
                switch (attackDirection)
                {
                case 'w': case 'W':
                    controller.longRangeAttack(0, -1);
                    tookTurn = true;
                    break;
                case 's': case 'S':
                    controller.longRangeAttack(0, 1);
                    tookTurn = true;
                    break;
                case 'a': case 'A':
                    controller.longRangeAttack(-1, 0);
                    tookTurn = true;
                    break;
                case 'd': case 'D':
                    controller.longRangeAttack(1, 0);
                    tookTurn = true;
                    break;
                default:
                    std::cout << "Неизвестное направление! Используйте w/a/s/d." << std::endl;
                    return;
                }
                break;
            }
            else{
                std::cout << "Вы не переключились на дальний бой!" << std::endl;
                waitForEnter();
                return;
            }
        }
        case 'b': case 'B':
            Market(controller, statsRenderer);
            break;
        case 'z': case 'Z':{
            if(player.getPlayerHand().getCurrentSize() == 0){
                std::cout << "У вас нет доступных заклинаний!" << std::endl;
            }
            else{
                statsRenderer.PrintPlayersSpells(controller);
                char spellNum = getCommand("Введите номер закинания: ");
                int index = spellNum - '0';
                if(index <= 0 || index > player.getPlayerHand().getMaxSize() || player.getPlayerHand().getSpellNum(index - 1) == 0){
                    std::cout << "Неверный номер заклинания!" << std::endl;
                }
                else{
                    switch(index){
                        case 1:
                        case 2:
                        case 3:
                        case 4:{
                            std::cout << "Введите координаты в формате x y: ";
                            int targX, targY;
                            std::cin >> targX >> targY;
                            waitForEnter("");
                            if(targX < 0 || targX > controller.getWorld().getField().getWidth() || targY < 0 || targY > controller.getWorld().getField().getHeight()){
                                std::cout << "Неверные координаты!" << std::endl;
                                tookTurn = false;
                                break;
                            }
                            controller.castSpell(index - 1, tookTurn, {targX, targY});
                            break;
                        }
                        case 5: {
                            std::cout << "Введите номер улучшаемого заклинания: ";
                            int numToUpgrade;
                            std::cin >> numToUpgrade;
                            clearInputBuffer();
                            
                            controller.castUpgradeSpell(index - 1, tookTurn, numToUpgrade - 1);
                            break;
                        }
                    }
                }
            }
            break;
        }
        case 'm': case 'M': {
            controller.getWorld().getPlayer().switchCombatMode();
            tookTurn = true;
            std::cout << "Режим боя изменен на: " 
                      << (player.getCombatMode() == Player::CombatMode::MELEE ? "БЛИЖНИЙ" : "ДАЛЬНИЙ")
                      << std::endl;
            break;
        }
        case 'q': case 'Q':
            gameRunning = false;
            std::cout << "Выход из игры..." << std::endl;
            break;
        case 'i': case 'I': {
            statsRenderer.printBattleInformation(controller);
            break;
        }
        default:
            std::cout << "Неизвестная команда!" << std::endl;
            return;
    }
}

void InputHandler::Market(GameControl& controller, StatsRenderer& statsRenderer){
    auto& player = controller.getWorld().getPlayer();
    std::cout << "==== Торговая лавка ====" << std::endl;
    std::cout << "Текущий баланс: " << player.getCoins() << " монет" << std::endl;
    statsRenderer.PrintPlayersSpells(controller);
    std::cout << "Доступные заклинания:" << std::endl;
    std::cout << "1) Заклинание прямого боя - 12 монет:" << std::endl;
    std::cout << "2) Заклинание урона по площади  - 15 монет:" << std::endl;
    std::cout << "3) Заклинание ловушки  - 17 монет:" << std::endl;
    std::cout << "4) Заклинание призыва  - 25 монет:" << std::endl;
    std::cout << "5) Заклинание улучшения  - 30 монет:" << std::endl;
    std::cout << "q - выйти из магазина" << std::endl;
    if (player.getPlayerHand().getCurrentSize() == player.getPlayerHand().getMaxSize()){
        std::cout << "Инвентарь заклинаний заполнен!" << std::endl;
    }
    else{
    char input = getCommand("Выберите заклинание: ");
    SpellManager spellManager;
    switch(input){
        case '1':{
            if(player.getCoins() >= 12){
                player.updateCoins(-12);
                spellManager.addSpecificSpell(controller.getWorld().getPlayer(),0,0);
                break;
            }
            else{
                std::cout << "Милорд, ваша казна пуста!" << std::endl;
            }
            break;
        }
        case '2':{
            if(player.getCoins() >= 15){
                player.updateCoins(-15);
                spellManager.addSpecificSpell(controller.getWorld().getPlayer(),1,0);
                break;
            }
            else{
                std::cout << "Милорд, ваша казна пуста!" << std::endl;
            }
            break;
        }
        case '3':{
            if(player.getCoins() >= 17){
                player.updateCoins(-17);
                spellManager.addSpecificSpell(controller.getWorld().getPlayer(),2,0);
                break;
            }
            else{
                std::cout << "Милорд, ваша казна пуста!" << std::endl;
            }
            break;
        }
        case '4':{
            if(player.getCoins() >= 25){
                player.updateCoins(-25);
                spellManager.addSpecificSpell(controller.getWorld().getPlayer(),3,0);
                break;
            }
            else{
                std::cout << "Милорд, ваша казна пуста!" << std::endl;
            }
            break;
        }
        case '5':{
            if(player.getCoins() >= 30){
                player.updateCoins(30);
                spellManager.addSpecificSpell(controller.getWorld().getPlayer(),4,0);
                break;
            }
            else{
                std::cout << "Милорд, ваша казна пуста!" << std::endl;
            }
            break;
        }
        case 'q': case 'Q':
            return;
        default:
            std::cout << "Неизвестная команда!" << std::endl;
            return;
        }
    }
}