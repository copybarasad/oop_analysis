#include "TemplatedGameController.h"
#include "ConfigurateInput.h"
#include "GameVisualizer.h"
#include "ConsoleRenderer.h"

int main() {
    GameState state(14, 14);
    std::string cfg = "controls.cfg";

    TemplatedGameController<
            ConfigurateInput,
            GameVisualizer<ConsoleRenderer>
    > controller(state, cfg);

    controller.run();
}
