#include <windows.h>
#include "GameManager.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    GameManager gameManager;
    gameManager.run_menu();
    return 0;
}
