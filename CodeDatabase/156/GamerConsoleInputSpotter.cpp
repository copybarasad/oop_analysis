//
// Created by Artem on 17.12.2025.
//
#include "GamerConsoleInputSpotter.h"
#include "Actions/PlayerAction.h"
#include "Actions/PlayerActionWithCoords.h"
#include "Actions/PlayerActionUseSpell.h"
#include "Actions/PlayerUpgradeAction.h"
#include "Actions/UpgradesType.h"
#include "../../../Exceptions/Notifications/CantAttackOnCoordsNotification.h"
#include "../../../Exceptions/Notifications/CantCastSpellOnCellNotification.h"

void GamerConsoleInputSpotter::playerMove(GameMasterBase &master) {
    std::string c;
    c = Constants::getInputWithoutN("Если хотите выйти и сохранить игру нажмиту \"" + std::string(1, this->keysModel.saveGameKey) + "\"");
    if(c[0] == this->keysModel.saveGameKey) {
        try{
            this->controller->doMove(master,  new PlayerAction(ActionType::SaveGame));
        }catch(...){

        }
        return;
    }

    bool flag = true;
    while(flag) {
        char yn;
        yn = Constants::getInputWithoutN("Вы хотите переместиться? (" + std::string(1, this->keysModel.confirmActionKey) + "\\" + std::string(1, this->keysModel.dismissActionKey) +"): ");
        if(yn == this->keysModel.confirmActionKey) {
            std::pair<int, int> yx = Constants::readTwoInts(
                    "Введите координаты на которые хотите перейти (сначала x потом y): ");
            yx.first--;
            yx.second--;
            try{
                this->controller->doMove(master, new PlayerActionWithCoords(ActionType::Move, {yx.second, yx.first}));
                flag = false;
                continue;
            }catch(...){
                throw;
            }
        }else if(yn == this->keysModel.dismissActionKey) {
            flag = false;
            continue;
        }
        std::cout << "Вы ввели не правильное значение!" << std::endl;
    }

    flag = true;
    while(flag){
        char acs = Constants::getInputWithoutN("Вы хотите атаковать, применить заклинание или пропустить ход? (" + std::string(1, this->keysModel.attackKey) + "\\" + std::string(1, this->keysModel.spellKey) + "\\" + std::string(1, this->keysModel.skipKey) + "): ");
        if (acs == this->keysModel.attackKey) {
            flag = false;
            bool isChanged = false;
            changeAttackType(master, &isChanged);
            if (!isChanged)
                attackOnCoords(master);
        }else if (acs == this->keysModel.spellKey) {
            useSpell(master);
            flag = false;
        }else if (acs == this->keysModel.skipKey) {
            flag = false;
        }else {
            std::cout << "Вы ввели не правильное значение!" << std::endl;
        }
    }
}

void GamerConsoleInputSpotter::changeAttackType(GameMasterBase &master, bool* isChanged) {
    bool flag = true;
    while(flag) {
        char ny = Constants::getInputWithoutN("Вы хотите сменить тип атаки? (" + std::string(1, this->keysModel.confirmActionKey) + "\\" + std::string(1, this->keysModel.dismissActionKey) +"): ");
        if (ny == this->keysModel.confirmActionKey) {
            try{
                this->controller->doMove(master, new PlayerAction(ActionType::ChangeAttackType));
                flag = false;
                *isChanged = true;
                continue;
            }catch(...){
                throw;
            }
        } else if (ny == this->keysModel.dismissActionKey) {
            flag = false;
            continue;
        }
        std::cout << "Вы ввели не правильное значение!" << std::endl;
    }
}

void GamerConsoleInputSpotter::attackOnCoords(GameMasterBase &master) {
    bool flag = true;

    while(flag) {
        std::pair<int, int> yx = Constants::readTwoInts("Введите координаты которые хотите атаковать (сначала x потом y): ");
        yx.first--;
        yx.second--;
        if(yx.first == -2 && yx.second == -2){
            flag = false;
            std::cout << "Атака отменена!" << std::endl;
            continue;
        }
        try{
            this->controller->doMove(master, new PlayerActionWithCoords(ActionType::Attack, {yx.second, yx.first}));
            flag = false;
            continue;
        }catch(const CantAttackOnCoordsNotification& e){
            std::cout << e.what() << ". Если хотите отменить атаку ввидите -1 -1 в координаты" << std::endl;
        }catch (const CoordinateException& e){
            std::cout << e.what() << ". Если хотите отменить атаку ввидите -1 -1 в координаты" << std::endl;
        }
    }

}

const std::map<SpellType, std::string> enumTypeToStrType = {
        {SpellType::DirectDamageSpell, "DirectDamageSpell"},
        {SpellType::BuffNextUsedSpell, "BuffNextUsedSpell"},
        {SpellType::AreaDamageSpell, "AreaDamageSpell"},
        {SpellType::SummoningSpell, "SummoningSpell"},
        {SpellType::CreateTrapSpell, "CreateTrapSpell"}
};

void GamerConsoleInputSpotter::useSpell(GameMasterBase &master) {
    std::cout << "В вашей руке: ";
    if(this->player.getSpellHand().getSpells().size() == 0) {
        std::cout << "пусто, применять нечего" << std::endl;
        return;
    }
    for (const auto & i : this->player.getSpellHand().getSpells()) {
        std::cout << enumTypeToStrType.at(i->getSpellType()) << ", ";
    }
    std::cout << std::endl;

    bool flag = true;
    int spellPos;
    while(flag) {
        int pos = Constants::getInput<int>("Выберете номер заклинания для применения (от 1 до n): ");
        if (pos <= 0 || pos > this->player.getSpellHand().getSpells().size()){
            std::cout << "не корректное значение!" << std::endl;
            continue;
        }
        pos--;
        spellPos = pos;
        flag = false;
    }

    flag = true;
    while(flag){
        switch(this->player.getSpellHand().getSpells()[spellPos]->getSpellType()) {
            case SpellType::DirectDamageSpell:
            case SpellType::AreaDamageSpell:
            case SpellType::SummoningSpell:
            case SpellType::CreateTrapSpell: {
                //перепутано специально
                std::pair<int, int> yx = Constants::readTwoInts(
                        "Введите координаты которые хотите атаковать (сначала x потом y): ");
                yx.first--;
                yx.second--;
                try{
                    this->controller->doMove(master, new PlayerActionUseSpell(ActionType::UseSpell, {yx.second, yx.first}, spellPos));
                    flag = false;
                    continue;
                }catch (const CantCastSpellOnCellNotification& e){
                    std::cout << e.what() << ". Если хотите отменить применение ввидите -1 -1 в координаты" << std::endl;
                }
            }
                break;
            case SpellType::BuffNextUsedSpell: {
                try {
                    this->controller->doMove(master, new PlayerActionUseSpell(ActionType::UseSpell, {1, -1}, spellPos));
                    flag = false;
                    continue;
                } catch (...) {

                }
            }
                break;
        }
    }


}

void GamerConsoleInputSpotter::waitingForContinueCommand(GameMasterBase &master) {
    bool flag = true;
    while(flag) {
        char cont = Constants::getInputWithoutN("Нажмите " + std::string(1, this->keysModel.continueKey) + " для продолжения: ");
        if (cont == this->keysModel.continueKey) {
            flag = false;
            try{
                this->controller->doMove(master, new PlayerAction(ActionType::Skip));
            }catch(...){}
        } else {
            std::cout << "Вы ввели неправильный символ!" << std::endl;
        }
    }
}

void GamerConsoleInputSpotter::upgradePlayer(GameMasterBase &master) {
    bool flag = true;
    while(flag) {
        std::cout << "Есть возможность улучшить игрока" << std::endl;
        int num = Constants::getInput<int>("Что вы хотите улучшить (требуется 1 очко), у вас: " + std::to_string(this->player.getScore()) + ":\n1.Урон\n2.Дальность хода\n3.Здоровье\n(введите цифру от 1 до 3х или 0 чтобы отказаться от выбора)");
        switch(num){
            case 0:
                flag = false;
                break;
            case 1:
                this->controller->doMove(master, new PlayerUpgradeAction(ActionType::UpgradePlayer, UpgradesType::UpdateDamage));
                flag = false;
                break;
            case 2:
                this->controller->doMove(master, new PlayerUpgradeAction(ActionType::UpgradePlayer, UpgradesType::UpdateMovementDistance));
                flag = false;
                break;
            case 3:
                this->controller->doMove(master, new PlayerUpgradeAction(ActionType::UpgradePlayer, UpgradesType::UpdateHealth));
                flag = false;
                break;
            default:
                std::cout << "Введённая цифра не в диапазоне от одного до трёх!" << std::endl;
                continue;
        }
    }
}