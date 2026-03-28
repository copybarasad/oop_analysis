#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H


#include "Game.h"
#include "CommandParser.h"
#include "KeyBindingConfig.h"
#include "GameView.h"
#include "ConsoleRenderer.h"
#include "SpellHand.h"
#include <memory>
#include <sstream>
#include <algorithm>
#include <cctype>


/**
 * @file GameController.h
 * @brief Шаблонный контроллер ввода с режимом выбора заклинания
 */


// Режимы обработки ввода
enum class InputMode {
    NORMAL,         // обычный режим (движение, команды)
    SPELL_SELECT,   // ожидаем номер заклинания
    SPELL_TARGET    // ожидаем координаты для спелла
};


template<typename TInputReader>
class GameController {
private:
    Game* game;
    TInputReader inputReader;
    KeyBindingConfig keyConfig;


    InputMode currentMode = InputMode::NORMAL;
    bool shouldRenderAfterInput = true;  ///< Нужна ли перерисовка после ввода
    int selectedSpellIndex = -1;         ///< Индекс выбранного спелла


public:
    explicit GameController(Game* gameInstance)
        : game(gameInstance) {
        keyConfig.loadFromFile("keybindings.txt");
    }


    /**
     * @brief Нужна ли перерисовка после последнего ввода
     * @return true если нужен рендер, false если была ошибка ввода
     */
    bool shouldRender() const {
        return shouldRenderAfterInput;
    }


    bool processInput() {
        shouldRenderAfterInput = true;  // По умолчанию рендерим
        
        std::string input = inputReader.readInput();
        if (input.empty()) {
            shouldRenderAfterInput = false;  // Пустой ввод — не рендерим
            return true;
        }


        std::string upperInput = input;
        for (char& c : upperInput) {
            c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        }


        // EXIT/QUIT/Q работают из любого режима
        if (upperInput == "EXIT" || upperInput == "QUIT" || upperInput == "Q") {
            auto command = CommandParser::parse(input, keyConfig);
            if (command) {
                command->execute(game);
            }
            return false;
        }


        switch (currentMode) {
        case InputMode::NORMAL: {
            // Проверяем команду через конфиг
            const CommandType* cmdType = keyConfig.getCommand(upperInput);
            
            // Клавиша заклинания — показать руку и перейти в режим выбора
            if (cmdType && *cmdType == CommandType::CAST_SPELL) {
                GameView<ConsoleRenderer> view(game);
                view.renderSpellHand();
                std::cout << "Enter spell index, or 'cancel':\n";
                currentMode = InputMode::SPELL_SELECT;
                shouldRenderAfterInput = false;
                return true;
            }

            // Клавиша информации — показать статус
            if (cmdType && *cmdType == CommandType::SHOW_INFO) {
                GameView<ConsoleRenderer> view(game);
                view.renderPlayerStatus();
                shouldRenderAfterInput = false;
                return true;
            }

            // Остальные команды — через парсер (WASD, M, H, SAVE, LOAD и т.п.)
            auto command = CommandParser::parse(input, keyConfig);
            if (!command) {
                std::cout << "Unknown command. Type 'H' for help.\n";
                shouldRenderAfterInput = false;
                return true;
            }
            command->execute(game);
            return true;
        }


        case InputMode::SPELL_SELECT: {
            // cancel — выйти из режима выбора
            if (upperInput == "CANCEL") {
                std::cout << "Spell selection cancelled.\n";
                currentMode = InputMode::NORMAL;
                shouldRenderAfterInput = false;
                return true;
            }


            // Ожидаем индекс спелла
            int k;
            std::istringstream iss(input);
            if (!(iss >> k)) {
                std::cout << "Invalid input. Enter spell index or 'cancel'.\n";
                shouldRenderAfterInput = false;
                return true;
            }


            // Проверяем наличие SpellHand
            if (!game->getSpellHand() || game->getSpellHand()->isEmpty()) {
                std::cout << "No spells available.\n";
                currentMode = InputMode::NORMAL;
                shouldRenderAfterInput = false;
                return true;
            }


            auto* hand = game->getSpellHand();
            if (k < 0 || k >= static_cast<int>(hand->getSpellCount())) {
                std::cout << "Invalid spell index.\n";
                shouldRenderAfterInput = false;
                return true;
            }


            // Получаем спелл
            Spell* spell = hand->getSpellAt(k);
            if (!spell) {
                std::cout << "Spell not found.\n";
                currentMode = InputMode::NORMAL;
                shouldRenderAfterInput = false;
                return true;
            }


            // Проверяем, нужны ли координаты
            if (spell->requiresTarget()) {
                std::cout << "Enter target coordinates (x y): ";
                selectedSpellIndex = k;
                currentMode = InputMode::SPELL_TARGET;
                shouldRenderAfterInput = false;
                return true;
            }


            // Спелл без таргета (Boost, Heal) — кастуем сразу
            bool ok = game->usePlayerSpell(k);
            if (!ok) {
                std::cout << "Failed to cast spell.\n";
            }


            currentMode = InputMode::NORMAL;
            return true;
        }


        case InputMode::SPELL_TARGET: {
            // cancel — отменить каст
            if (upperInput == "CANCEL") {
                std::cout << "Spell cancelled.\n";
                currentMode = InputMode::NORMAL;
                selectedSpellIndex = -1;
                shouldRenderAfterInput = false;
                return true;
            }


            // Парсим координаты
            int x, y;
            std::istringstream iss(input);
            if (!(iss >> x >> y)) {
                std::cout << "Invalid coordinates. Enter 'x y' or 'cancel'.\n";
                shouldRenderAfterInput = false;
                return true;
            }


            // Кастуем спелл с координатами
            bool ok = game->usePlayerSpell(selectedSpellIndex, x, y);
            if (!ok) {
                std::cout << "Failed to cast spell at (" << x << "," << y << ").\n";
            }


            currentMode = InputMode::NORMAL;
            selectedSpellIndex = -1;
            return true;
        }
        }


        return true;
    }


    TInputReader& getInputReader() {
        return inputReader;
    }


    KeyBindingConfig& getKeyConfig() {
        return keyConfig;
    }
};


#endif // GAMECONTROLLER_H


