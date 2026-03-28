#include "LevelManager.h"
#include "SpellFactory.h"
#include "GameExceptions.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <limits>
#include <ctime>

LevelManager::LevelManager()
    : currentLevelNumber(1)
    , currentLevel(nullptr) {
}

void LevelManager::StartLevel(int levelNumber, GameField& field) {
    if (levelNumber < 1) {
        throw InvalidLevelException(levelNumber);
    }
    
    currentLevelNumber = levelNumber;
    currentLevel = std::make_unique<Level>(levelNumber, field.GetWidth(), field.GetHeight());
    
    currentLevel->InitializeField(field);
    field.SetPlayerPosition(0, 0);
    
    std::cout << "\n╔══════════════════════════════════════════╗" << std::endl;
    std::cout << "║       НАЧАЛО УРОВНЯ " << levelNumber << "                  ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════╝" << std::endl;
    std::cout << "Размер поля: " << field.GetWidth() << "x" << field.GetHeight() << std::endl;
    std::cout << "Врагов: " << currentLevel->GetEnemyCount() << std::endl;
    std::cout << "Зданий: " << currentLevel->GetBuildingCount() << std::endl;
    std::cout << "Башен: " << currentLevel->GetTowerCount() << std::endl;
    std::cout << "═══════════════════════════════════════════\n" << std::endl;
}

void LevelManager::CompleteLevel(Player& player) {
    std::cout << "\n╔══════════════════════════════════════════╗" << std::endl;
    std::cout << "║       УРОВЕНЬ " << currentLevelNumber << " ПРОЙДЕН!             ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════╝" << std::endl;
    std::cout << "Очки: " << player.GetScore() << std::endl;
    
    int bonusScore = 50 * currentLevelNumber;
    player.AddScore(bonusScore);
    std::cout << "Бонус за прохождение: +" << bonusScore << " очков" << std::endl;
}

void LevelManager::ApplyLevelTransition(Player& player) {
    std::cout << "\n═══════════════════════════════════════════" << std::endl;
    std::cout << "ПЕРЕХОД НА СЛЕДУЮЩИЙ УРОВЕНЬ" << std::endl;
    std::cout << "═══════════════════════════════════════════" << std::endl;
    
    restorePlayerHealth(player);
    removeHalfSpells(player);
    
    std::cout << "Здоровье восстановлено до максимума." << std::endl;
    std::cout << "Половина заклинаний удалена.\n" << std::endl;
}

void LevelManager::removeHalfSpells(Player& player) {
	size_t spellCount = player.GetSpellCount();
	if (spellCount == 0) return;
	
	size_t spellsToRemove = spellCount / 2;
	if (spellsToRemove == 0 && spellCount > 0) {
		spellsToRemove = 1;
	}
	
	std::vector<size_t> indices;
	for (size_t i = 0; i < spellCount; ++i) {
		indices.push_back(i);
	}
	
	static std::mt19937 randomGenerator(static_cast<unsigned int>(std::time(nullptr)));
	std::shuffle(indices.begin(), indices.end(), randomGenerator);
	
	std::sort(indices.begin(), indices.begin() + spellsToRemove, std::greater<size_t>());
	
	for (size_t i = 0; i < spellsToRemove; ++i) {
		std::cout << "  Удалено: " << player.GetSpellNameAt(indices[i]) << std::endl;
		player.RemoveSpellAt(indices[i]);
	}
}

void LevelManager::restorePlayerHealth(Player& player) {
	int maxHealth = player.GetMaxHealth();
	int currentHealth = player.GetHealth();
	if (currentHealth < maxHealth) {
		player.Heal(maxHealth - currentHealth);
	}
}

bool LevelManager::ShowUpgradeMenu(Player& player) {
    std::cout << "\n╔══════════════════════════════════════════╗" << std::endl;
    std::cout << "║         ПРОКАЧКА ПЕРСОНАЖА           ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════╝" << std::endl;
    std::cout << "Выберите улучшение:" << std::endl;
    std::cout << "1. Увеличить максимальное здоровье (+20 HP)" << std::endl;
    std::cout << "2. Увеличить урон в ближнем бою (+5 DMG)" << std::endl;
    std::cout << "3. Увеличить урон в дальнем бою (+5 DMG)" << std::endl;
    std::cout << "4. Улучшить случайное заклинание" << std::endl;
    std::cout << "5. Получить новое заклинание" << std::endl;
    std::cout << "═══════════════════════════════════════════" << std::endl;
    
    int choice = 0;
    while (choice < 1 || choice > 5) {
        std::cout << "Ваш выбор (1-5): ";
        std::cin >> choice;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            choice = 0;
            std::cout << "Некорректный ввод!" << std::endl;
            continue;
        }
        
        if (choice < 1 || choice > 5) {
            std::cout << "Выберите число от 1 до 5!" << std::endl;
        }
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
	switch (choice) {
		case 1:
			player.UpgradeMaxHealth(20);
			std::cout << "✓ Максимальное здоровье увеличено на 20!" << std::endl;
			break;
		case 2:
			player.UpgradeMeleeDamage(5);
			std::cout << "✓ Урон в ближнем бою увеличен на 5!" << std::endl;
			break;
		case 3:
			player.UpgradeRangedDamage(5);
			std::cout << "✓ Урон в дальнем бою увеличен на 5!" << std::endl;
			break;
        case 4:
            if (player.GetSpellCount() > 0) {
                static std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
                std::uniform_int_distribution<size_t> dist(0, player.GetSpellCount() - 1);
                size_t spellIndex = dist(rng);
                
                Spell* spell = player.GetSpellAt(spellIndex);
                if (spell && !spell->IsEnhancement()) {
                    spell->Enhance();
                    std::cout << "✓ Заклинание '" << spell->GetName() << "' улучшено!" << std::endl;
                } else {
                    std::cout << "✗ Не удалось улучшить заклинание" << std::endl;
                }
            } else {
                std::cout << "✗ Нет заклинаний для улучшения" << std::endl;
            }
            break;
        case 5: {
            auto newSpell = SpellFactory::CreateRandomSpell(15, 15);
            player.AddSpell(std::move(newSpell));
            std::cout << "✓ Получено новое заклинание!" << std::endl;
            break;
        }
    }
    
    return true;
}

void LevelManager::SetCurrentLevel(int levelNumber) {
    currentLevelNumber = levelNumber;
}

