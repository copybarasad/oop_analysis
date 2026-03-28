#pragma once

#include "Command.hpp"
#include <memory>
#include <iostream>

class InputHandler {
public:
    virtual ~InputHandler() = default;
    virtual std::unique_ptr<Command> getCommand() const = 0;
};

class ConsoleInputHandler : public InputHandler {
public:
    std::unique_ptr<Command> getCommand() const override {
        std::cout << "\nChoose action:\n0 - Move\n1 - Change Attack Type\n2 - Attack\n3 - Cast Spell\n";

        int choice;
        while(true) {
            std::cin >> choice;

            if(std::cin.fail() || choice < 0 || choice > 3) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Invalid input! Please enter 0-3: ";
            } else {
                break;
            }
        }

        switch(choice) {
            case 0: return handleMoveCommand();
            case 1: return handleChangeAttackTypeCommand();
            case 2: return std::make_unique<AttackCommand>();
            case 3: return std::make_unique<GiveASpellCommand>();
            default: return std::make_unique<InvalidCommand>();
        }
    }

private:
    std::unique_ptr<Command> handleMoveCommand() const {
        std::cout << "Enter move direction (dx dy): ";
        int dx, dy;
        while(true) {
            std::cin >> dx >> dy;
            if(std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Invalid input! Enter two integers: ";
            } else {
                break;
            }
        }
        return std::make_unique<MoveCommand>(dx, dy);
    }

    std::unique_ptr<Command> handleChangeAttackTypeCommand() const {
        std::cout << "Enter new attack type (0-2): ";
        int type;
        while(true) {
            std::cin >> type;
            if(std::cin.fail() || type < 0 || type > 2) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Invalid input! Enter 0-2: ";
            } else {
                break;
            }
        }
        return std::make_unique<ChangeAttackTypeCommand>(type);
    }
};
