#include <iostream>
#include <unistd.h>


#include "console/ConsoleRenderer.h"
#include "console/KeyboardInputHandler.h"
#include "utils /Menu.h"
#include "utils /Saver.h"

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {

    srand(time(NULL));

    try {
        TemplateMenu<KeyboardInputHandler, ConsoleRenderer> standardMenu("saves");
        standardMenu.showMain();

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}