#include "KeyboardLayout.hpp"
#include "commands/Command.hpp"
#include "commands/ControlCommand.hpp"
#include "commands/PlayerActionCommand.hpp"
#include <memory>


template<typename InputController>
class CommandFactory {
private:
    InputController& controller;
    KeyboardLayout& keyboard;
public:
    explicit CommandFactory(InputController& inputController, KeyboardLayout& keyboard) : controller(inputController), keyboard(keyboard) {}
    std::unique_ptr<Command> getCommand() {
        char input = this->controller.getInputChar();
        char standartInput = 0;
        if (input == keyboard.KEYESC) {
            standartInput = StandardKeyboardLayout::KEYESC;
        }
        else if (input == keyboard.KEYENTER) {
            standartInput = StandardKeyboardLayout::KEYENTER;
        }
        if (standartInput) {
            return std::make_unique<ControlCommand>(standartInput);
        }
        if (input == keyboard.KEYUP) {
            standartInput = StandardKeyboardLayout::KEYUP;
        }
        else if (input == keyboard.KEYDOWN) {
            standartInput = StandardKeyboardLayout::KEYDOWN;
        }
        else if (input == keyboard.KEYLEFT) {
            standartInput = StandardKeyboardLayout::KEYLEFT;
        }
        else if (input == keyboard.KEYRIGHT) {
            standartInput = StandardKeyboardLayout::KEYRIGHT;
        }
        else if (input == keyboard.KEYE) {
            standartInput = StandardKeyboardLayout::KEYE;
        }
        else if (input == keyboard.KEYQ) {
            standartInput = StandardKeyboardLayout::KEYQ;
        }
        else if (input == keyboard.KEY1) {
            standartInput = StandardKeyboardLayout::KEY1;
        }
        else if (input == keyboard.KEY2) {
            standartInput = StandardKeyboardLayout::KEY2;
        }
        else if (input == keyboard.KEY3) {
            standartInput = StandardKeyboardLayout::KEY3;
        }
        else if (input == keyboard.KEY4) {
            standartInput = StandardKeyboardLayout::KEY4;
        }
        if (standartInput) {
            return std::make_unique<PlayerActionCommand>(standartInput);
        }
        return nullptr;
    }
};