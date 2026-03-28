#include "GameController.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    try {
        GameController controller;
        
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "--controls" && i + 1 < argc) {
                std::string controlsFile = argv[++i];
                controller.setControlsFile(controlsFile);
            }
            else if (arg == "--log" && i + 1 < argc) {
                // Установить способ логирования
                std::string logMode = argv[++i];
                controller.setLoggingMode(logMode);
            }
        }
        
        controller.start();  // Старт через контроллер
    }
    catch (const std::exception& e) {
        std::cerr << "Критическая ошибка: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
