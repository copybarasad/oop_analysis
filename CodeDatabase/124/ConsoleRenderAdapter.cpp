#include "ConsoleRenderAdapter.h"
#include "spellcard.h"
#include <iostream>
#include <iomanip>

void ConsoleRenderAdapter::renderGame(const GameField& field, const Player& player, const Enemy& enemy,
                                     const std::vector<EnemyTower>& towers, const EnemyManager& enemyManager) {
    renderField(field);
    renderUI(field, player, enemy, towers, enemyManager);
}

void ConsoleRenderAdapter::renderField(const GameField& field) const {
    std::cout << "\n=== ИГРОВОЕ ПОЛЕ " << field.getWidth() << "x" << field.getHeight() << " ===\n";
     
    std::cout << "  ";
    for (int x = 0; x < field.getWidth(); x++) {
        std::cout << x % 10 << " ";
    }
    std::cout << "\n";
     
    for (int y = 0; y < field.getHeight(); y++) {
        std::cout << std::setw(2) << y << " ";
        
        for (int x = 0; x < field.getWidth(); x++) {
            std::cout << getCellSymbol(field, x, y) << " ";
        }
        std::cout << "\n";
    }
    
    std::cout << "Легенда: P-игрок, E-враг, T-башня, L-ловушка, #-препятствие\n";
}

std::string ConsoleRenderAdapter::getCellSymbol(const GameField& field, int x, int y) const {
    const Cell& cell = field.getCell(x, y);
    
    if (cell.getHasPlayer()) return "P";
    if (cell.getHasEnemy()) return "E";
    if (field.hasTowerAt(x, y)) return "T";
    if (!cell.getIsPassable()) return "#";
    if (field.hasTrapAt(x, y)) return "L";
    
    return ".";
}

void ConsoleRenderAdapter::renderUI(const GameField& field, const Player& player, const Enemy& enemy,
                                   const std::vector<EnemyTower>& towers, const EnemyManager& enemyManager) {
    renderPlayerStatus(player);
    renderEnemyInfo(enemy);
    renderTowersInfo(towers);
    renderProgressInfo(enemyManager);
}

void ConsoleRenderAdapter::renderPlayerStatus(const Player& player) const {
    std::cout << "\n=== СТАТУС ИГРОКА ===\n";
    std::cout << "Здоровье: " << player.getHealth() << "/" << player.getMaxHealth() << "\n";
    std::cout << "Мана: " << player.getEconomy().getMana() << "/" << player.getEconomy().getMaxMana() << "\n";
    std::cout << "Тип атаки: " << player.getCombatSystem().getAttackTypeString() << "\n";
    std::cout << "Урон ближний: " << player.getCombatSystem().getMeleeDamage() << "\n";
    std::cout << "Урон дальний: " << player.getCombatSystem().getRangedDamage() << "\n";
    std::cout << "Текущий урон: " << player.getCombatSystem().getDamage() << "\n";
    std::cout << "Очки: " << player.getEconomy().getScore() << "\n";
    
    const auto& spells = player.getSpellHand().getSpells();
    if (!spells.empty()) {
        std::cout << "--- ЗАКЛИНАНИЯ (" << spells.size() << "/" << player.getSpellHand().getMaxSize() << ") ---\n";
        for (size_t i = 0; i < spells.size(); ++i) {
            auto spell = spells[i];
            std::cout << (i == player.getSpellHand().getSelectedIndex() ? "> " : "  ");
            std::cout << spell->getName() << "\n";
            std::cout << "    " << spell->getDescription() << "\n";
            std::cout << "    Мана: " << spell->getManaCost();
            if (!spell->isReady()) {
                std::cout << ", Перезарядка: " << spell->getCurrentCooldown();
            }
            std::cout << "\n";
        }
    } else {
        std::cout << "--- НЕТ ЗАКЛИНАНИЙ ---\n";
    }
    std::cout << "=====================\n";
}

void ConsoleRenderAdapter::renderEnemyInfo(const Enemy& enemy) const {
    if (enemy.isAlive()) {
        std::cout << "Враг (Ур. " << enemy.getLevel() << "): " 
                  << enemy.getHealth() << " HP, Урон: " << enemy.getDamage() << std::endl;
    }
}

void ConsoleRenderAdapter::renderTowersInfo(const std::vector<EnemyTower>& towers) const {
    if (!towers.empty()) {
        std::cout << "--- БАШНИ ---\n";
        for (const auto& tower : towers) {
            if (tower.getIsAlive()) {
                int x, y;
                tower.getPosition(x, y);
                std::cout << "T(" << x << "," << y << "): " << tower.getHealth() << " HP\n";
            }
        }
    }
}

void ConsoleRenderAdapter::renderProgressInfo(const EnemyManager& enemyManager) const {
    std::cout << "Прогресс: " << enemyManager.getEnemiesDefeated() << "/" << 2 << " врагов побеждено\n";
}

void ConsoleRenderAdapter::renderHelp() {
    std::cout << "Игра началась! Управление:\n";
    std::cout << "wasd-движение, x-атака, t-смена атаки\n";
    std::cout << "q/e-выбор заклинания, c-применить заклинание\n";
    std::cout << "b-купить заклинание (100 очков)\n";
    std::cout << "m-открыть магазин заклинаний\n";
    std::cout << "l-сохранить игру, o-загрузить игру\n";
    std::cout << "h-справка по командам, r-перезагрузить настройки\n";
}