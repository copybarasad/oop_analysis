#include "SpellManager.h"
#include "../game_spells/DirectDamageSpell.h"
#include "../game_spells/AreaDamageSpell.h"
#include "../interface/ConsoleRenderer.h"

// Если есть враги рядом -> показать и вернуть true
bool SpellManager::showTargetsInRange(const Player& player, const GameState& state, int range) {
    std::cout << Colors::SLOW << "\n=== Possible Targets in Range " << range
              << " around (" << player.getX() << ", " << player.getY() << ") ===\n" << Colors::RESET;

    bool any = false;

    // Персонажи (враги)
    for (const auto& enemy : state.getAllCharacters()) {
        if (enemy == &player || !enemy->isAlive()) continue;
        int dx = std::abs(enemy->getX() - player.getX());
        int dy = std::abs(enemy->getY() - player.getY());
        if (dx <= range && dy <= range) {
            std::cout << Colors::CYAN << " - Enemy (" << enemy->getX() << ", " << enemy->getY() << ")\n" << Colors::RESET;
            any = true;
        }
    }

    // Вражеские здания
    for (const auto& building : state.getAllBuildings()) {
        if (!building->isAlive()) continue;
        int dx = std::abs(building->getX() - player.getX());
        int dy = std::abs(building->getY() - player.getY());
        if (dx <= range && dy <= range) {
            std::cout << Colors::CYAN << " - Enemy building (" << building->getX() << ", " << building->getY() << ")\n" << Colors::RESET;
            any = true;
        }
    }

    // Вражеские башни
    for (const auto& tower : state.getAllTowers()) {
        if (!tower->isAlive()) continue;
        int dx = std::abs(tower->getX() - player.getX());
        int dy = std::abs(tower->getY() - player.getY());
        if (dx <= range && dy <= range) {
            std::cout << Colors::CYAN << " - Enemy Tower (" << tower->getX() << ", " << tower->getY() << ")\n" << Colors::RESET;
            any = true;
        }
    }

    return any;
}

// Процесс обработки заклинания
void SpellManager::spellProcessing(Player& player, const PlayerHand& hand, GameState& state) {
    // Рука не может быть пустой
    if (hand.isEmpty()) {
        state.addMessage("No possible spells in hand :(");
        return;
    }

    std::ostringstream ss;

    size_t spellIndex;
    std::cout << "Enter number of the spell: ";

    // Некорректный ввод
    if (!(std::cin >> spellIndex)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        ss << Colors::DARK_RED << "Invalid input for index of the spell." << Colors::RESET;
        state.addMessage(ss.str());
        return;
    }

    // Неправильный введенный номер заклинания
    spellIndex--;
    if (spellIndex < 0 || spellIndex >= static_cast<int>(hand.getCurrentSize())) {
        ss << Colors::DARK_RED << "Wrong index of the spell" << Colors::RESET;
        state.addMessage(ss.str());
        return;
    }

    // Заклинание не нашлось
    auto spell = hand.getSpell(spellIndex);
    if (!spell) {
        ss << Colors::DARK_RED << "Error! Spell is not found!" << Colors::RESET;
        state.addMessage(ss.str());
        return;
    }

    int range = spell->getRange();
    bool hasTargets;

    // Если TrapSpell - проверяем просто есть ли враги рядом, если SpawnAllySpell - нам не нужно условие на радиус, поэтому hasTargets = true;
    if (spell->getSymbol() == 'a' || spell->getSymbol() == 'U') {
        hasTargets = true;
    } else {
        hasTargets = showTargetsInRange(player, state, range); // Для удобства пользователя показывать коориднаты доступных сущностей
    }

    // Если нет никого в радиусе игрока -> выходим
    if (!hasTargets) {
        ss << Colors::DARK_RED << "No enemies around of the player. Can't use the spell." << Colors::RESET;
        state.addMessage(ss.str());
        return;
    }

    // Если заклинание - SpawnAlly то используем его отедльно (т.к. не требует ввода координат цели)
    if (spell->getSymbol() == 'a' || spell->getSymbol() == 'U') {
        // Передаем любые координаты (они будут проигнорированы в методе use)
        if (state.useSpellFromHand(spellIndex, player.getX(), player.getY())) {
            ss << Colors::GREEN << spell->getName() << " was successfully used!" << Colors::RESET;
            state.addMessage(ss.str());
        }
        return;
    }

    int targetX, targetY;
    std::cout << "Enter target (x, y): ";

    // Некорректный ввод
    if (!(std::cin >> targetX >> targetY)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        ss << Colors::DARK_RED << "Invalid input for coordinates." << Colors::RESET;
        state.addMessage(ss.str());
        return;
    }

    // Неправильные координаты
    if (targetX > state.getField().getWidth() || targetY > state.getField().getWidth() ||
                targetX < 0 || targetY < 0 ||
                std::abs(player.getX() - targetX) > range || std::abs(player.getY() - targetY) > range) {
        ss << Colors::DARK_RED << "Invalid input for coordinates." << Colors::RESET;
        state.addMessage(ss.str());
        return;
    }
    
    // Если все хорошо -> Используем заклинание!
    if (state.useSpellFromHand(spellIndex, targetX, targetY)) {
        ss << Colors::GREEN << spell->getName() << " succesfully used!" << Colors::RESET;
        state.addMessage(ss.str());
    } 
}