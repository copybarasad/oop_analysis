#include "GameRunner.h"
#include <windows.h>
#include <iostream>

int main() {
    SetConsoleOutputCP(65001);
    GameRunner runner;
    return runner.run();
}