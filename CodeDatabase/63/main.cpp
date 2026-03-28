#include "Application.h"
#include "ConsoleInputManager.h"
#include "ConsoleRenderer.h"

int main() {
    Application<ConsoleInputManager, ConsoleRenderer> app;
    app.run();
    return 0;
}
