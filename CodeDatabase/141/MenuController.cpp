#include "MenuController.h"
#include "Game.h"
#include "RenderSystem.h"
#include "InputSystem.h"
#include "ControlMapping.h"
#include "KeyboardCommandReader.h"
#include "GameVisualizer.h"
#include "GameController.h"
#include "SaveSystem.h"
#include "LevelManager.h"
#include <iostream>
#include <limits>

MenuController::MenuController(int argc, char **argv) : argc(argc), argv(argv) {
}

void MenuController::run() {
    bool running = true;
    while (running) {
        std::cout << "\n=== МЕНЮ ===\n1. Новая игра\n2. Загрузить\n3. Выход\n";
        int choice = readInt("Выбор: ");
        if (choice == 3) break;

        bool repeatGame = true;
        while (repeatGame) {
            repeatGame = false;

            std::string logFile;
            bool fileMode = false;
            std::shared_ptr<ILogSink> sink = chooseSink(logFile, fileMode);
            EventBus bus;
            Logger logger(sink);
            logger.attach(bus);

            RenderSystem renderSystem;
            InputSystem inputSystem;
            ControlMapping controls;
            controls.loadFromFile("controls.cfg", [sinkWeak = std::weak_ptr<ILogSink>(sink)](const std::string &msg) {
                if (auto locked = sinkWeak.lock()) locked->write(msg);
            });
            renderSystem.setLogSink(sink.get());
            renderSystem.setConsoleLogEnabled(!fileMode);
            renderSystem.setControls(&controls);
            KeyboardCommandReader reader(controls, inputSystem);
            GameVisualizer<RenderSystem> visualizer(renderSystem);
            Game game(LevelManager::calculateFieldWidth(1), LevelManager::calculateFieldHeight(1), renderSystem, &bus);

            game.setUpgradeSelector([this](const std::vector<UpgradeOption> &options) {
                return selectUpgrade(options);
            });

            GameController<KeyboardCommandReader, GameVisualizer<RenderSystem> > controller(reader, visualizer);

            if (choice == 2) {
                auto slots = SaveSystem::listSaveSlots();
                if (slots.empty()) {
                    std::cout << "Нет сохранений\n";
                } else {
                    std::string slot = selectSlot(slots);
                    if (!slot.empty()) {
                        std::string path = SaveSystem::getSaveFilePath(slot);
                        try {
                            SaveSystem::loadGame(game, path);
                        } catch (const std::exception &e) {
                            std::cout << "Ошибка загрузки: " << e.what() << "\n";
                            continue;
                        }
                    }
                }
            }

            controller.run(game);

            if (controller.isRestartRequested()) {
                choice = 1;
                repeatGame = true;
            }
        }
    }
}

std::shared_ptr<ILogSink> MenuController::chooseSink(std::string &logFile, bool &fileMode) const {
    std::string mode = "console";
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.rfind("--log=", 0) == 0) {
            mode = arg.substr(6);
        } else if (arg.rfind("--log-file=", 0) == 0) {
            logFile = arg.substr(11);
        }
    }
    if (mode == "file") {
        fileMode = true;
        if (logFile.empty()) logFile = "game.log";
        return std::make_shared<FileSink>(logFile);
    }
    fileMode = false;
    return std::make_shared<ConsoleSink>();
}

int MenuController::readInt(const std::string &prompt) const {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Введите число\n";
    }
}

std::string MenuController::selectSlot(const std::vector<std::string> &slots) const {
    std::cout << "\nСлоты сохранений:\n";
    for (size_t i = 0; i < slots.size(); ++i) {
        std::cout << (i + 1) << ". " << slots[i] << "\n";
    }
    std::cout << "0. Отмена\n";
    std::cout << "Введите номер слота или имя нового: ";
    std::string input;
    std::getline(std::cin >> std::ws, input);
    if (input.empty()) return "";
    bool isNumber = !input.empty() && std::all_of(input.begin(), input.end(),
                                                  [](char c) { return std::isdigit(static_cast<unsigned char>(c)); });
    if (isNumber) {
        int choice = std::stoi(input);
        if (choice == 0) return "";
        if (choice >= 1 && choice <= static_cast<int>(slots.size())) {
            return slots[choice - 1];
        }
        return "";
    }
    return input;
}

int MenuController::selectUpgrade(const std::vector<UpgradeOption> &options) const {
    std::cout << "\n=== УЛУЧШЕНИЕ ===\n";
    for (size_t i = 0; i < options.size(); ++i) {
        std::cout << (i + 1) << ". " << options[i].description << "\n";
    }
    int pick = readInt("Выбор: ") - 1;
    if (pick < 0 || pick >= static_cast<int>(options.size())) return 0;
    return pick;
}
