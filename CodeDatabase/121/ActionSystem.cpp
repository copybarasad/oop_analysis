#include "ActionSystem.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "SpellFactory.h"
#include <iostream>
#include <memory>


//  Обработка действий игрока
//  Содержит логику обработки команд игрока - движение, атака, заклинания.
//  Использует TextSystem для вывода сообщений и CombatSystem для боевых действий.

ActionSystem::ActionSystem(std::shared_ptr<TextSystem> textSystem) 
    : textSystem(textSystem), combatSystem(textSystem) {}

// Обрабатывает команду игрока и выполняет соответствующее действие
bool ActionSystem::processCommand(Command command, LevelController& level) {
    switch (command) {
        case Command::MOVE_UP:    return movePlayer(level, 0, -1);
        case Command::MOVE_DOWN:  return movePlayer(level, 0, 1);
        case Command::MOVE_LEFT:  return movePlayer(level, -1, 0);
        case Command::MOVE_RIGHT: return movePlayer(level, 1, 0);
        case Command::ATTACK:     return playerAttack(level);
        case Command::SWITCH_WEAPON:
            level.getPlayer().switchBattleMode();
            std::cout << textSystem->getBattleModeMessage(
                level.getPlayer().getBattleMode() == BattleMode::CLOSE) << std::endl;
            return true;
        case Command::USE_SPELL_1: return useSpell(level, 0);
        case Command::USE_SPELL_2: return useSpell(level, 1);
        case Command::USE_SPELL_3: return useSpell(level, 2);
        case Command::SHOW_SPELLS:
            showSpells(level);
            return false;
        case Command::SHOP:
            return false;
        case Command::SAVE_GAME:
        case Command::LOAD_GAME:
            return false;
        default:
            return false;
    }
}

// Перемещает игрока на указанное смещение с проверкой клеток и столкновений
bool ActionSystem::movePlayer(LevelController& level, int deltaX, int deltaY) {
    auto& player = level.getPlayer();
    auto& field = level.getField();
    
    Position currentPos = player.getPosition();
    Position newPos = {currentPos.first + deltaX, currentPos.second + deltaY};
    
    // Проверка выхода за границы поля
    if (!field.isPositionValid(newPos)) {
        std::cout << textSystem->getOutOfBoundsMessage() << std::endl;
        return false;
    }
    
    // Проверка возможности перемещения на клетку
    if (!field.canMoveToPosition(newPos)) {
        std::cout << textSystem->getBlockedCellMessage() << std::endl;
        return false;
    }
    
    // Проверка столкновений с врагами
    for (const auto& enemy : level.getEnemies()) {
        if (enemy.isAlive() && enemy.getPosition() == newPos) {
            player.takeDamage(1);
            std::cout << textSystem->getPlayerDamageMessage(1, player.getHealth()) << std::endl;
            return true;
        }
    }
    
    // Проверка типа клетки перед перемещением
    // Если клетка замедляющая - устанавливаем флаг slowed
    const Cell& targetCell = field.getCell(newPos);
    if (targetCell.getType() == CellType::SLOW) {
        player.setSlowed(true);
        std::cout << textSystem->getPlayerSlowedMessage() << std::endl;
    }
    
    // Выполнение перемещения
    field.removeObjectFromPosition(currentPos);
    player.move(deltaX, deltaY);
    field.placeObjectAtPosition(newPos);
    
    std::cout << textSystem->getMoveMessage(
        PositionUtils::toString(currentPos),
        PositionUtils::toString(newPos)
    ) << std::endl;
    
    return true;
}

// Выполняет атаку игрока с использованием CombatSystem
bool ActionSystem::playerAttack(LevelController& level) {
    auto& player = level.getPlayer();
    bool attacked = combatSystem.playerAttack(player, level.getField(), 
                                            level.getEnemies(), level.getBuildings(), level.getTowers());
    
    if (attacked) {
        std::cout << textSystem->getAttackSuccessMessage() << std::endl;
    } else {
        std::cout << textSystem->getNoTargetsMessage() << std::endl;
    }
    
    return attacked;
}

// Использует заклинание из указанного слота
bool ActionSystem::useSpell(LevelController& level, int spellIndex) {
    auto& player = level.getPlayer();
    auto& hand = player.getHand();
    
    // Проверка на пустую руку
    if (hand.getSpellCount() == 0) {
        std::cout << textSystem->getNoSpellsMessage() << std::endl;
        return false;
    }
    
    // Проверка валидности индекса заклинания
    if (spellIndex < 0 || spellIndex >= static_cast<int>(hand.getSpellCount())) {
        std::cout << textSystem->getInvalidChoice() << std::endl;
        std::cout << textSystem->getAvailableSpellsRange(hand.getSpellCount()) << std::endl;
        return false;
    }
    
    // Получаем имя заклинания ДО каста для сообщения
    std::string spellName = "";
    if (const Spell* spell = hand.getSpell(spellIndex)) {
        spellName = spell->getName();
    }
    
    // Определяем целевую позицию (рядом с игроком)
    Position targetPos = player.getPosition();
    targetPos.first += 1;
    
    // Попытка кастовать заклинание
    bool success = hand.castSpell(spellIndex, player.getPosition(), targetPos, level.getField());
    
    if (success) {
        std::cout << textSystem->getSpellCastMessage(spellName, true) << std::endl;
        
        // Шанс получить случайное заклинание после успешного каста
        if (hand.hasSpace() && (std::rand() % 100) < 30) {
            giveRandomSpell(hand);
        }
    } else {
        std::cout << textSystem->getSpellCastMessage(spellName, false) << std::endl;
    }
    
    return success;
}

// Показывает все заклинания в руке игрока
void ActionSystem::showSpells(LevelController& level) {
    auto& hand = level.getPlayer().getHand();
    std::cout << textSystem->getSpellHandTitle() << std::endl;
    
    if (hand.getSpellCount() == 0) {
        std::cout << textSystem->getEmptySpellHand() << std::endl;
    } else {
        for (size_t i = 0; i < hand.getSpellCount(); i++) {
            if (const Spell* spell = hand.getSpell(i)) {
                std::cout << i + 1 << ". " << spell->getName() << " - " << spell->getDescription() << std::endl;
            }
        }
    }
    std::cout << textSystem->getSpellSlotInfo(hand.getMaxSize() - hand.getSpellCount(), hand.getMaxSize()) << std::endl;
}

// Дает случайное заклинание в руку игрока
void ActionSystem::giveRandomSpell(SpellHand& hand) {
    // Создаем случайное заклинание через фабрику
    auto newSpell = SpellFactory::createRandomSpell();
    
    // Если создание успешно и есть место в руке - добавляем
    if (newSpell && hand.hasSpace()) {
        hand.addSpell(newSpell);
        std::cout << textSystem->getSpellObtainedMessage(newSpell->getName()) << std::endl;
    }
}