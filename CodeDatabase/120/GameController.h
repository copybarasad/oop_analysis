#pragma once
#include "CommandRegistry.h"
#include "ConsoleRenderer.h"
#include "LoadManager.h"
#include "PlayerUpgrade.h"

template<typename InputReader, typename Renderer>
class GameController {
public:
    GameController(GameWorld& world,
                   GameLogic& logic,
                   CommandRegistry& registry,
                   InputReader& reader)
        : world_(world),
          logic_(logic),
          input_reader_(reader),
          renderer_(),
          registry_(registry) {}

    void Run() {    
        MainMenu();
        while (!logic_.IsGameOver()) {
            renderer_.Render(world_, logic_);

            char cmd = input_reader_.GetCommandChar();
            if (cmd == '\0') continue;

            auto command = registry_.FromChar(cmd);
            if (command) {
                GameContext context{world_, logic_, logic_.GetEventSystem(),input_reader_};
                bool playerActed = command->Execute(context);

                if (playerActed) {
                    logic_.UpdateTurn();

                    if (logic_.WasLevelCleared()) {
                        AdvanceToNextLevel();
                    }
                }
            } else {
                logic_.events.Notify(GameMessageEvent{"Unknown command!"});
            }
        }

        renderer_.Render(world_, logic_);
        HandleGameOver();
    }

private:
    GameWorld& world_;
    GameLogic& logic_;
    InputReader input_reader_;
    Renderer renderer_;
    CommandRegistry& registry_;

    void MainMenu() {
        while (true) {
            std::cout << "=== Welcome to the Game ===\n";
            std::cout << "1. New Game\n2. Load Game\n3. Exit\nChoose an option: ";

            int choice = 0;
            std::cin >> choice;

            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input! Please enter 1, 2 or 3.\n";
                continue;
            }

            if (choice == 1) {
                world_.InitEntities();
                SpawnPlayerIfNeeded();
                return;
            }

            if (choice == 2) {
              std::string filename;
              std::cout << "Enter save filename: ";
              std::cin >> filename;

              LoadManager loader;
              ExceptionHandler handler(&logic_);
              bool success = handler.Handle([&]() -> bool {
                loader.LoadGame(filename, world_);
                logic_.events.Notify(GameMessageEvent{"Game loaded successfully."});
                return true;
              });

              if (!success) {
                std::cout << "Failed to load save. Check error above and try again.\n";
                continue;
              }

              return;
            }

            if (choice == 3) exit(0);

            std::cout << "Invalid choice! Please enter 1, 2 or 3.\n";
        }
    }

    void SpawnPlayerIfNeeded() {
        auto& player = world_.GetPlayer();
        if (player.GetPosition().x == -1 && player.GetPosition().y == -1) {
            std::optional<Position> spawnPos;
            do {
                spawnPos = world_.GetMap().FindRandomFreeCell();
            } while (spawnPos && world_.IsOccupied(spawnPos->x, spawnPos->y));

            if (spawnPos) {
                player.SetPosition(*spawnPos);
                logic_.events.Notify(GameMessageEvent{"Player spawned at (" + std::to_string(spawnPos->x) + "," +
                              std::to_string(spawnPos->y) + ")"});
                if (world_.GetMap().GetCellType(spawnPos->x, spawnPos->y) == Cell::CellType::Slow) {
                    player.SetSlowed();
                    logic_.events.Notify(GameMessageEvent{"Player spawned on slow cell! Next turn will be skipped."});
                }
            }
        }
        logic_.events.Notify(GameMessageEvent{"Game initialized successfully."});

    }

    void AdvanceToNextLevel() {
      try {
        world_.SetCurrentLevel(world_.GetCurrentLevel() + 1);

        int curW = world_.GetMap().GetWidth();
        int curH = world_.GetMap().GetHeight();
        int newW = std::min(25, curW + 2);
        int newH = std::min(25, curH + 2);

        int enemyCount = 3 + world_.GetCurrentLevel();
        int towerCount = 1 + (world_.GetCurrentLevel() / 2);
        int spawnerCount = 1 + (world_.GetCurrentLevel() / 3);

        world_.ResetForNextLevel(newW, newH, enemyCount, spawnerCount, towerCount);

        PlayerUpgrade upgrade(world_.GetPlayer(), logic_);
        upgrade.ShowUpgradeMenu();

        SpawnPlayerIfNeeded();
        world_.GetPlayer().RestoreFullHealth();

        for (auto& e : world_.GetEnemies()) {
          if (e) {
            e->SetHealth(e->GetHealth() + world_.GetCurrentLevel() * 2);
            e->SetDamage(e->GetDamage() + world_.GetCurrentLevel());
          }
        }

        world_.GetPlayer().AddScore(20);
        logic_.events.Notify(GameMessageEvent{"Level Up! You earned 20 points."});

        logic_.ClearLevelClearedFlag();
        logic_.events.Notify(GameMessageEvent{"Advanced to level " +
                    std::to_string(world_.GetCurrentLevel()) +
                    ". New field size: " + std::to_string(newW) + "x" +
                    std::to_string(newH) + "."});

      } catch (const std::exception& ex) {
        logic_.events.Notify(GameMessageEvent{std::string("Unexpected error during level transition: ") +
                    ex.what()});
      } catch (...) {
        logic_.events.Notify(GameMessageEvent{"Unknown error during level transition."});
      }
    }

    void HandleGameOver() {
        while (true) {
            std::cout << "\nGame Over! Choose an option:\n1. Restart\n2. Exit\nChoice: ";
            int option = 0;
            std::cin >> option;
            if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); continue; }
            if (option == 1) {
                world_.InitEntities();
                SpawnPlayerIfNeeded();
                logic_.ResetGameOverFlag();
                Run();
                return;
            }
            if (option == 2) exit(0);
        }
    }
};
