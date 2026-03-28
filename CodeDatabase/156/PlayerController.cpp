//
// Created by Artem on 09.10.2025.
//

#include <iostream>
#include "PlayerController.h"
#include "../GameMasterBase.h"
#include "ExternalComandController/Actions/PlayerAction.h"
#include "../../Exceptions/Notifications/CantAttackOnCoordsNotification.h"
#include "../../Logger/Logger.h"

#include <cmath>
#include <queue>

void PlayerController::doMove(GameMasterBase& gameMaster, PlayerAction* action) {
    switch (action->type) {
        case ActionType::SaveGame: {
            gameMaster.saveGame();
        }
        break;
        case ActionType::Move: {
            PlayerActionWithCoords* playerAction = static_cast<PlayerActionWithCoords*>(action);

            std::vector<std::vector<int>> distances(this->field.getHeight(), std::vector<int>(this->field.getWidth(), -1));

            if (this->field.canMoveToOrSpawnOnNoExcept(playerAction->xy.x, playerAction->xy.y) && canMoveOn(distances, {this->playerView.getXCoordinate(), this->playerView.getYCoordinate()}, playerAction->xy)){
                moveTo(distances, {this->playerView.getXCoordinate(), this->playerView.getYCoordinate()}, playerAction->xy, gameMaster);
            }else{
                throw CoordinateException("Вы не можете перейти на клетку x/y:" + std::to_string(this->playerView.getXCoordinate()) + "/" + std::to_string(this->playerView.getYCoordinate()) + " клетка или за гарницами поля или недоступна!");
            }
        }
        break;
        case ActionType::Attack: {
            PlayerActionWithCoords* playerAction = static_cast<PlayerActionWithCoords*>(action);


            this->field.isCoordsAvailable(playerAction->xy.x, playerAction->xy.y); //швыряет ошибку CoordinateException (ненавижу плюсы!!!!!!!!!!)

            if(!this->field.getFieldCells()[playerAction->xy.x][playerAction->xy.y].hasEntityInCell())
                throw CantAttackOnCoordsNotification("Невозможно проатаковать по введённым координатам, там некого нету!");

            if (this->playerView.isCloseRangeAttackSelected()){
                Constants::dxdy dxdyEcs = {this->playerView.getXCoordinate() - playerAction->xy.x, this->playerView.getYCoordinate() - playerAction->xy.y};
                bool flag = false;
                for(Constants::dxdy pair : Constants::dxdys){
                    if (dxdyEcs == pair){
                        flag = true;
                        break;
                    }
                }

                if (!flag)
                    throw CantAttackOnCoordsNotification("Вы не можете атаковать ближней атакой сущности находящиеся дальше чем одна клетка от вас или клетку на которой вы стоите!");

                this->manager.attack(playerAction->xy, AttackType::CloseRange);
                Logger::info("Игрок проатаковал сущность по координатам x/y: " + std::to_string(playerAction->xy.y+1) + "/" + std::to_string(playerAction->xy.x+1) + " на " + std::to_string(playerView.getCloseRangeAttack().getDamage()) + "hp");
            } else {
                int distance = std::floor(std::sqrt(std::pow(this->playerView.getXCoordinate() - playerAction->xy.x, 2) + std::pow(this->playerView.getYCoordinate() - playerAction->xy.y, 2)));
                if(distance > this->playerView.getLongRangeAttack().getRange())
                    throw CantAttackOnCoordsNotification("Вы не можете атаковать дальней атакой сущности находящиеся дальше чем дальность дальней атаки!");

                this->manager.attack(playerAction->xy, AttackType::LongRange);
                Logger::info("Игрок проатаковал сущность по координатам x/y: " + std::to_string(playerAction->xy.y+1) + "/" + std::to_string(playerAction->xy.x+1) + " на " + std::to_string(playerView.getCloseRangeAttack().getDamage()) + "hp");
            }
            gameMaster.redraw();
        }
        break;
        case ActionType::UseSpell: {
            PlayerActionUseSpell* playerAction = static_cast<PlayerActionUseSpell*>(action);

            switch(this->manager.getSpellHand().getSpells()[playerAction->spellNum].get()->getSpellType()){
                case SpellType::DirectDamageSpell:
                case SpellType::AreaDamageSpell:
                case SpellType::SummoningSpell:
                case SpellType::CreateTrapSpell:
                    this->manager.getSpellHand().useSpellWithAIMBinding(playerAction->spellNum, this->field, this->manager.getEntityCoords(), playerAction->xy);
                    break;
                case SpellType::BuffNextUsedSpell:
                    this->manager.getSpellHand().useSpellWithoutAnyCoordsBinding(playerAction->spellNum);
                    break;
            }
            gameMaster.redraw();
        }
        break;
        case ActionType::Skip:
            Logger::tech("Действие пропущено по желанию пользователя!");
        break;
        case ActionType::ChangeAttackType: {
            bool isCloseRangeSelected = this->playerView.isCloseRangeAttackSelected();
            this->manager.changeAttackType();
            Logger::info(std::string("Тип атаки игрока был изменён с ") + std::string(isCloseRangeSelected ? "ближней" : "дальней") + " на " + std::string(isCloseRangeSelected ? "дальнюю" : "ближнюю"));
            gameMaster.redraw();
        }
        break;
        case ActionType::UpgradePlayer: {
            PlayerUpgradeAction* playerAction = static_cast<PlayerUpgradeAction*>(action);
            this->manager.upgradePlayer(playerAction->upgradeType);
            Logger::tech("Улучшение успешно произведено!");
            gameMaster.redraw();
        }
        break;
        case ActionType::Continue:{
            Logger::tech("Выполнение успешно продолжено (после подтверждения пользователем)");
        }
        break;
    }
}

bool PlayerController::canMoveOn(std::vector<std::vector<int>>& distances, Constants::XYPair moveFrom, Constants::XYPair moveTo) {
    std::queue<Constants::XYPair> queue;
    queue.push(moveTo);
    distances[moveTo.x][moveTo.y] = 0;

    while (!queue.empty()) {
        Constants::XYPair tempCoords = queue.front();
        queue.pop();
        for (Constants::dxdy dxdy: Constants::dxdys) {
            if ((this->field.canMoveToOrSpawnOnNoExcept(tempCoords.x + dxdy.x, tempCoords.y + dxdy.y) || this->manager.getEntityCoords() == Constants::XYPair{tempCoords.x + dxdy.x, tempCoords.y + dxdy.y})
            && distances[tempCoords.x + dxdy.x][tempCoords.y + dxdy.y] == -1) {
                distances[tempCoords.x + dxdy.x][tempCoords.y + dxdy.y] = distances[tempCoords.x][tempCoords.y] + 1;
                queue.push({tempCoords.x + dxdy.x, tempCoords.y + dxdy.y});
            }
        }
    }

    std::cout << "\n\n";
    for (int i = 0; i < distances.size(); ++i) {
        for (int j = 0; j < distances[0].size(); ++j) {
            std::cout << distances[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "\n\n";

    return distances[moveFrom.x][moveFrom.y] <= this->playerView.getStepRange();
}

void
PlayerController::moveTo(std::vector<std::vector<int>> &distances, Constants::XYPair moveFrom, Constants::XYPair moveTo,
                         GameMasterBase &gameMaster) {
    Constants::XYPair coords = moveFrom;
    while(true){
        for(Constants::dxdy dxdy : Constants::dxdys){
            int nX = coords.x + dxdy.x;
            int nY = coords.y + dxdy.y;
            if (nX < 0 || nX >= this->field.getHeight() || nY < 0 || nY >= this->field.getWidth())
                continue;
            if(distances[nX][nY] == (distances[coords.x][coords.y] - 1)){
                coords = {nX, nY};
                this->manager.moveTo(dxdy);
                Logger::info("Игрок переместился в x/y: " + std::to_string(coords.x) + "/" + std::to_string(coords.y));
                gameMaster.redraw();
                break;
            }
        }
        if(coords == moveTo){
            break;
        }
    }


}
