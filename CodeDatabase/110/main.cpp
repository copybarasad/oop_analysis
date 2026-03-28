#include "modules/Entities/Entity/Entity.hpp"
#include "modules/Entities/Player/Player.hpp"
#include "modules/Entities/Enemies/Enemy.hpp"
#include "modules/Entities/Inanimate/Building/Building.hpp"
#include "modules/Entities/Inanimate/Trap/Trap.hpp"
#include "modules/Entities/Inanimate/Tower/Tower.hpp"
#include "modules/Items/Item.hpp"
#include "modules/Items/Weapon.hpp"
#include "modules/Items/Armor.hpp"
#include "modules/Field/Field/Field.hpp"
#include "modules/Field/Cell/Cell.hpp"

#include "modules/Managers/EntityManager/EntityManager.hpp"
#include "modules/Spells/SpellHand/SpellHand.hpp"

#include "modules/GameBlock/TurnProcessor/PlayerTurnProcessor/PlayerTurnProcessor.hpp"
#include "modules/GameBlock/TurnProcessor/EnemyTurnProcessor/EnemyTurnProcessor.hpp"
#include "modules/GameBlock/TurnProcessor/AllyTurnProcessor/AllyTurnProcessor.hpp"
#include "modules/GameBlock/TurnProcessor/BuildingTurnProcessor/BuildingTurnProcessor.hpp"
#include "modules/GameBlock/TurnProcessor/TowerTurnProcessor/TowerTurnProcessor.hpp"

#include "modules/Services/GameCycleService/GameCycleService.hpp"

#include "modules/Services/SaveLoadServices/SaveService/TxtSaveService/TxtSaveService.hpp"
#include "modules/Services/SaveLoadServices/LoadService/TxtLoadService/TxtLoadService.hpp"


int main () {
    std::string saveFile = "save1";
    try {
        TxtLoadService loadService("GameSaves");

        if (loadService.saveExists(saveFile)) {
            std::cout << "Найдено сохранение " << saveFile << std::endl;

            GameCycleService::GameState* loadedState = loadService.load(saveFile);
            
            std::cout << "Успешно загружено!" << std::endl;
            std::cout << "Текущий уровень: " << loadedState->currentLevel << std::endl;
            std::cout << "Прогресс: ";
            
            switch (loadedState->progress) {
                case GameProgress::ON_GOING:
                    std::cout << "Игра продолжается" << std::endl;
                    break;
                case GameProgress::LEVEL_COMPLETED:
                    std::cout << "Уровень завершен" << std::endl;
                    break;
                case GameProgress::PLAYER_DEAD:
                    std::cout << "Игрок мертв" << std::endl;
                    break;
            }

            std::cout << "Размер поля: " << loadedState->field.getWidth() << "x" << loadedState->field.getHeight() << std::endl;
            std::cout << "Количество сущностей: " << loadedState->manager.getEntityNumber() << std::endl;
            std::cout << "Врагов на поле: " << loadedState->manager.getEnemiesNumber() << std::endl;
            std::cout << "Заклинаний в руке: " << loadedState->hand.getSpellNumber() << std::endl;
            std::cout << "Заклинаний в магазине: " << loadedState->store.getSpellNumber() << std::endl;
        }
        else {
            std::cout << "Сохранение" << saveFile << " не найдено" << std::endl;
        }
    }
    catch (const ValidationException& e) {
        std::cout << "Ошибка валидации: " << e.what() << std::endl;
        std::cout << "Файл сохранения поврежден!" << std::endl;
        
    } catch (const SaveLoadException& e) {
        std::cout << "Ошибка загрузки: " << e.what() << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "Неизвестная ошибка: " << e.what() << std::endl;
    }

    return 0;
}