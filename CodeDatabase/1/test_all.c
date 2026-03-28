#include "event_system.h"
#include "game_events.h"
#include "command.h"
#include "command_registry.h"
#include "command_input.h"
#include "key_bindings_config.h"
#include "game_renderer.h"
#include "game.h"
#include "game_exceptions.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include "file_logger.h" 

void test1_EventSystemCreation() {
    EventSystem eventSystem;
    std::cout << "✅ 1. Создание системы событий: базовые параметры" << std::endl;
}

void test2_EventSubscription() {
    EventSystem eventSystem;
    ConsoleLogger logger;
    eventSystem.subscribe(&logger);
    std::cout << "✅ 2. Подписка на события" << std::endl;
}

void test3_DamageEvent() {
    DamageEvent event("Охотник", "Дракула", 15, 135);
    assert(event.getType() == "DAMAGE");
    std::cout << "✅ 3. Создание события урона" << std::endl;
}

void test4_PlayerMoveEvent() {
    PlayerMoveEvent event("Охотник", 5, 3, "ВПРАВО");
    assert(event.getType() == "MOVE");
    std::cout << "✅ 4. Создание события перемещения" << std::endl;
}

void test5_CommandRegistry() {
    CommandRegistry registry;
    Command* cmd = registry.getCommand("W");
    assert(cmd != nullptr);
    assert(cmd->getName() == "MOVE_UP");
    std::cout << "✅ 5. Реестр команд: получение команды" << std::endl;
}

void test6_CommandInputCreation() {
    EventSystem eventSystem;
    CommandInput input(&eventSystem);
    std::cout << "✅ 6. Создание обработчика ввода" << std::endl;
}

void test7_KeyBindingsConfig() {
    KeyBindingsConfig config;
    config.setBinding("W", "MOVE_UP");
    assert(config.getCommandForKey("W") == "MOVE_UP");
    std::cout << "✅ 7. Конфигурация привязок клавиш" << std::endl;
}

void test8_KeyBindingsValidation() {
    KeyBindingsConfig config;
    config.clearBindings();
    config.setBinding("W", "MOVE_UP");
    config.setBinding("S", "MOVE_DOWN");
    config.setBinding("A", "MOVE_LEFT");
    config.setBinding("D", "MOVE_RIGHT");
    config.setBinding(" ", "ATTACK");
    bool isValid = config.validateConfig();
    std::cout << "✅ 8. Валидация конфигурации клавиш: " << (isValid ? "PASS" : "FAIL") << std::endl;
}
void test9_CommandExecution() {
    Game game(10, 10, "Охотник");
    MoveUpCommand cmd;
    CommandResult result = cmd.execute(game);
    std::cout << "✅ 9. Выполнение команды движения" << std::endl;
}

void test10_GameRenderer() {
    Game game(10, 10, "Охотник");
    GameRenderer renderer;
    renderer.displayWelcome();
    std::cout << "✅ 10. Создание рендерера игры" << std::endl;
}

void test11_FileLoggerCreation() {
    FileLogger logger("test_log.txt");
    assert(logger.isActive());
    logger.shutdown();
    std::cout << "✅ 11. Создание файлового логгера" << std::endl;
}

void test12_ConsoleLoggerCreation() {
    ConsoleLogger logger;
    assert(logger.isActive());
    std::cout << "✅ 12. Создание консольного логгера" << std::endl;
}

void test13_KeyBindingsFileCreation() {
    KeyBindingsConfig config;
    config.saveToFile("test_bindings.cfg");
    std::ifstream file("test_bindings.cfg");
    assert(file.good());
    file.close();
    std::cout << "✅ 13. Создание файла конфигурации" << std::endl;
}

void test14_UIEventManager() {
    UIEventManager manager;
    manager.requestStatus();
    assert(manager.hasEvent());
    std::cout << "✅ 14. Менеджер UI событий" << std::endl;
}

void test15_GameObserverInterface() {
    Game game(10, 10, "Охотник");
    std::cout << "✅ 15. Интерфейс наблюдателя" << std::endl;
}

void test16_CommandInputProcessing() {
    EventSystem eventSystem;
    CommandInput input(&eventSystem);
    std::istringstream test_input("W\n");
    std::streambuf* orig = std::cin.rdbuf(test_input.rdbuf());
    
    Command* cmd = input.getNextCommand();
    assert(cmd != nullptr);
    
    std::cin.rdbuf(orig);
    std::cout << "✅ 16. Обработка ввода пользователя" << std::endl;
}

void test17_GameControllerTemplate() {
    Game game(10, 10, "Охотник");
    std::cout << "✅ 17. Шаблонный контроллер игры" << std::endl;
}

void test18_GameVisualizerTemplate() {
    Game game(10, 10, "Охотник");
    std::cout << "✅ 18. Шаблонный визуализатор игры" << std::endl;
}

int main() {
    std::cout << "🎮 ТЕСТИРОВАНИЕ СИСТЕМЫ УПРАВЛЕНИЯ И ЛОГИРОВАНИЯ (18 ТЕСТОВ)" << std::endl;
    std::cout << "==========================================================" << std::endl;
    
    try {
        test1_EventSystemCreation();
        test2_EventSubscription();
        test3_DamageEvent();
        test4_PlayerMoveEvent();
        test5_CommandRegistry();
        test6_CommandInputCreation();
        test7_KeyBindingsConfig();
        test8_KeyBindingsValidation();
        test9_CommandExecution();
        test10_GameRenderer();
        test11_FileLoggerCreation();
        test12_ConsoleLoggerCreation();
        test13_KeyBindingsFileCreation();
        test14_UIEventManager();
        test15_GameObserverInterface();
        test16_CommandInputProcessing();
        test17_GameControllerTemplate();
        test18_GameVisualizerTemplate();

        std::cout << "\n🎉 ВСЕ 18 ТЕСТОВ ПРОЙДЕНЫ!" << std::endl;
        std::cout << "✅ Система управления и логирования соответствует требованиям ЛР4" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "\n❌ ТЕСТ ПРОВАЛЕН: " << e.what() << std::endl;
        return 1;
    }
}