#include "CommandFactory.hpp"

template<typename InputController>
std::unique_ptr<Command> CommandFactory<InputController>::getCommand() {
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