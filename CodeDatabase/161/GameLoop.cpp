#include <filesystem>

#include "GameLoop.h"
#include "../logic/GameSaver.h"


GameLoop::GameLoop() {
    std::shared_ptr<Player> player = std::make_shared<Player>(100, 100, 10);
    Field field(10, 10);
    World world;

    getLevelMod(world, field, player);

    std::cout << "\n=== CONTROLS ===\n";
    std::cout << "w/a/s/d - Move | r - Change attack mode | b - Buy spell\n";
    std::cout << "e X Y - Attack | q - Cast spell | S - Save game | D - Delete save\n\n";

    while (!player->IsDead()) {
        std::cout << "\n========== LEVEL " << currentLevel << " ==========\n";
        Visualizer::Draw(field, world, player);



        world.Update(field, player);
        world.DeleteDeadEntites(field);

        if (CheckLevelComplete(field)) {
            std::cout << "\n*** LEVEL " << currentLevel << " COMPLETE! ***\n";
            StartLevel(world, field, player);
        }
    }

    std::cout << "\n=== GAME OVER ===\n";
    std::cout << "Final Score: " << player->GetScore() << "\n";
    std::cout << "Levels Completed: " << currentLevel - 1 << "\n\n";

    std::cout << "Play again? (y/n): ";
    std::string choice;
    std::getline(std::cin, choice);
    if (choice == "y" || choice == "Y") {
        *this = GameLoop(); // Перезапуск
    }
}

void GameLoop::getLevelMod(World& world, Field& field, std::shared_ptr<Player>& player) {
    while (true) {
        std::cout << "\n=== GAME MENU ===\n";
        std::cout << "1. New Game\n";
        std::cout << "2. Load Game\n";
        std::cout << "3. Delete Save\n";
        std::cout << "4. Exit\n";
        std::cout << "Choice: ";

        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") {
            world.Init(field, player, 1, 1, 1, 1, 0);
            currentLevel = 1;
            break;
        }
        else if (choice == "2") {
            if (LoadGameMenu(world, field, player)) {
                break;
            }
        }
        else if (choice == "3") {
            DeleteSaveMenu();
        }
        else if (choice == "4") {
            std::cout << "Goodbye!\n";
            std::exit(0);
        }
        else {
            std::cout << "Invalid choice!\n";
        }
    }
}

bool GameLoop::LoadGameMenu(World& world, Field& field, std::shared_ptr<Player>& player) {
    auto saves = GameSaver::listSaves();
    std::vector<std::string> valid;

    for (const auto& f : saves) {
        try {
            GameSaver g(f);
            g.load();
            valid.push_back(f);
        } catch (const FileNotFoundError& e) {
            std::cerr << "Skip missing file: " << e.what() << "\n";
        } catch (const CorruptedSaveError& e) {
            std::cerr << "Skip corrupted save: " << e.what() << "\n";
        } catch (const InvalidDataError& e) {
            std::cerr << "Skip invalid save: " << e.what() << "\n";
        } catch (const std::exception& e) {
            std::cerr << "Skip unreadable save: " << e.what() << "\n";
        }
    }

    if (valid.empty()) {
        std::cout << "No valid saves found.\n";
        return false;
    }

    std::cout << "\nAvailable saves:\n";
    for (size_t i = 0; i < valid.size(); ++i) {
        std::cout << i+1 << ". " << valid[i] << "\n";
    }
    std::cout << "0. Cancel\n";
    std::cout << "Choose: ";

    std::string sel;
    std::getline(std::cin, sel);

    int idx = -1;
    try {
        idx = std::stoi(sel);
    } catch (const std::exception& e) {
        std::cerr << "Invalid input: " << e.what() << "\n";
        return false;
    }

    if (idx <= 0 || idx > static_cast<int>(valid.size())) {
        std::cout << "Cancelled.\n";
        return false;
    }

    try {
        GameSaver saver(valid[idx-1]);
        TokenGameState st = saver.load();
        currentLevel = st.level;
        world.LoadState(field, player, st);
        std::cout << "✓ Loaded: " << valid[idx-1] << "\n";
        return true;
    } catch (const FileNotFoundError& e) {
        std::cerr << "Error: " << e.what() << "\n";
    } catch (const CorruptedSaveError& e) {
        std::cerr << "Error: " << e.what() << "\n";
    } catch (const InvalidDataError& e) {
        std::cerr << "Error: " << e.what() << "\n";
    } catch (const LoadException& e) {
        std::cerr << "Load failed: " << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << "\n";
    }

    return false;
}

void GameLoop::DeleteSaveMenu() {
    auto saves = GameSaver::listSaves();

    if (saves.empty()) {
        std::cout << "No saves to delete.\n";
        return;
    }

    std::cout << "\nSaves:\n";
    for (size_t i = 0; i < saves.size(); ++i) {
        std::cout << i+1 << ". " << saves[i] << "\n";
    }
    std::cout << "0. Cancel\n";
    std::cout << "Delete save #: ";

    std::string sel;
    std::getline(std::cin, sel);

    int idx = -1;
    try {
        idx = std::stoi(sel);
    } catch (const std::exception& e) {
        std::cerr << "Invalid input: " << e.what() << "\n";
        return;
    }

    if (idx <= 0 || idx > static_cast<int>(saves.size())) {
        std::cout << "Cancelled.\n";
        return;
    }

    try {
        std::filesystem::remove(saves[idx-1]);
        std::cout << "✓ Deleted: " << saves[idx-1] << "\n";

        saves.erase(saves.begin() + idx - 1);
        GameSaver::rebuildIndex(saves);
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Delete failed: " << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << "\n";
    }
}


void GameLoop::StartLevel(World& world, Field& field, std::shared_ptr<Player> player) {
    std::cout << "\n╔════════════════════════════════════════╗\n";
    std::cout << "║     LEVEL " << currentLevel << " COMPLETE!                  ║\n";
    std::cout << "╚════════════════════════════════════════╝\n\n";

	player->GetHand()->removeRandomHalf();

    LevelUpMenu(player);
	player->setHp(player->GetMaxHp());
    currentLevel++;

    std::cout << "\n>>> Starting Level " << currentLevel << " <<<\n\n";

    int new_size = std::min(10 + currentLevel, 25);
    field = Field(new_size, new_size);

    int enemies_c, build_c, tower_c;
    GenerateLevel(enemies_c, build_c, tower_c);

    world.Init(field, player, currentLevel, enemies_c, build_c, tower_c, 0);

    std::cout << "Enemies: " << enemies_c << " | Buildings: " << build_c
              << " | Towers: " << tower_c << "\n";
}

void GameLoop::LevelUpMenu(std::shared_ptr<Player>& player) {
    std::cout << "\n╔════════════════════════════════════════╗\n";
    std::cout << "║          LEVEL UP!                     ║\n";
    std::cout << "╚════════════════════════════════════════╝\n";
    std::cout << "\nChoose upgrade:\n";
    std::cout << "1. +50 Max HP\n";
    std::cout << "2. +20 Weapon Damage\n";
    std::cout << "3. Upgrade Random Spell (x2)\n";
    std::cout << "4. Add Random Spell to Hand\n";
    std::cout << "Choice: ";

    std::string choice;
    std::getline(std::cin, choice);

    if (choice == "1") {
        // Увеличение HP (нужно добавить макс HP в Player)
    	player->addMaxHp(50);
        std::cout << "✓ Max HP increased by 50!\n";
    }
    else if (choice == "2") {
    	auto old_weapon = player->GetWeapon();
    	auto new_weapon = old_weapon->Upgrade();

    	player->UpgradeWeapon(new_weapon);

    	std::cout << "✓ Weapon upgraded! New damage: "
				  << new_weapon->GetDamage() << " (range mode) / "
				  << new_weapon->GetDamage() * 2 << " (melee mode)\n";
    }
    else if (choice == "3") {
        auto hand = player->GetHand();
        if (hand->size() > 0) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, hand->size() - 1);
            size_t idx = dis(gen);

            auto spell = hand->getSpell(idx);
            if (spell) {
                spell->upgrade(2);
                std::cout << "✓ Random spell upgraded (x2 power)!\n";
            }
        } else {
            std::cout << "✗ No spells to upgrade! Getting +50 HP instead.\n";
        	player->addMaxHp(50);
        	player->setHp(player->GetMaxHp());
        }
    }
    else if (choice == "4") {
        player->GetHand()->setRandomSpell();
        std::cout << "✓ Random spell added to hand!\n";
    }
    else {
        std::cout << "Invalid choice! No upgrade applied.\n";
    }
}

bool GameLoop::CheckLevelComplete(Field& field) {
    return field.getNotFriendlyCount() == 0;
}

void GameLoop::GenerateLevel(int& enemy, int& buildings, int& towers) {
	const int MAX_LEVEL = 100;
	int safeLevel = std::min(currentLevel, MAX_LEVEL);

	enemy = std::min(safeLevel + 2, 50); // Максимум 50 врагов
	buildings = std::min(safeLevel / 2 + 1, 50); // Максимум 20 зданий
	towers = std::min(safeLevel / 3 + 1, 50); // Максимум 15 башен
}