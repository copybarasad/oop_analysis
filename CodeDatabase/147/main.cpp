#include "Game.h"
#include "Logger.h"
#include "ConfigLoader.h"

int main() {
    std::cout << "1) Новая игра\n2) Загрузить игру\n";
    int choice;
    std::cin >> choice;
    std::string filename;

    // Настройка логирования
    std::cout << "Выберите логирование: 0) нет 1) консоль 2) файл: ";
    int logChoice = 0;
    std::cin >> logChoice;
    std::shared_ptr<ILogger> logger = nullptr;
    if (logChoice == 1) {
        logger = std::make_shared<ConsoleLogger>();
    } else if (logChoice == 2) {
        std::cout << "Имя файла лога: ";
        std::string logFile;
        std::cin >> logFile;
        logger = std::make_shared<FileLogger>(logFile);
    }
    EventBus bus(logger);

    // Настройка управления
    std::cout << "Введите путь к файлу настроек управления (или '-' для умолчаний): ";
    std::string bindingsPath;
    std::cin >> bindingsPath;
    ConfigLoader loader;
    auto bindings = (bindingsPath != "-") ? loader.loadBindings(bindingsPath) : std::unordered_map<char, std::string>{};

    Game game;
    game.setEventBus(&bus);
    game.setBindings(bindings);
    if (choice == 2) {
        std::cout << "Введите имя файла для загрузки:(например, save.txt) ";
        std::cin >> filename;
        try {
            game.loadFromFile(filename);
        } catch (const SaveException& e) {
            std::cout << e.what() << "\n";
            std::cout << "Не удалось загрузить сохранение, начинаем новую игру.\n";
        }
    }
    game.start();
    return 0;
}


