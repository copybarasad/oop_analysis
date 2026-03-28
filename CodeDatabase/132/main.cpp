#include "app.hpp"

const std::string DB_NAME = "oopMagic";

int main() {
    App app(DB_NAME);
    app.run();
}