#include "game_visualizer.h"
#include "console_renderer.h"

// Явная специализация шаблона GameVisualizer для компиляции
// с типом ConsoleRenderer
template class GameVisualizer<ConsoleRenderer>;

