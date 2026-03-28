#pragma once
#include <memory>
#include "MenuController.h"

class Application {
public:
    Application();
    void Run();

private:
    std::unique_ptr<MenuController> menu_;
};
