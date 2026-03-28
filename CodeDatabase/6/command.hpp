#ifndef COMMAND
#define COMMAND

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>

enum class CommandType{
    MOVE_UP,
    MOVE_DOWN, 
    MOVE_LEFT,
    MOVE_RIGHT,
    TOGGLE_FIGHT,
    SHOW_ATTACK_RADIUS,
    BUY_SPELL,
    SAVE_GAME,
    CAST_SPELL_1,
    CAST_SPELL_2,
    CAST_SPELL_3,
    CAST_SPELL_4,
    CAST_SPELL_5,
    CAST_SPELL_6,
    CANCEL,
    NEW_GAME,
    CONTINUE_GAME,
    EXIT,
    CONTINUE_NEXT_LEVEL,
    MAIN_MENU,
    UPGRADE_1,
    UPGRADE_2,
    UPGRADE_3,
    UPGRADE_4,
    BUY_SPELL_1,
    BUY_SPELL_2,
    BUY_SPELL_3,
    BUY_SPELL_4,
    BUY_SPELL_5,
    MOUSE_CLICK,
    UNKNOWN
};

class Command{
    public:
        virtual ~Command() = default;
        virtual CommandType getType() const = 0;
        virtual std::string toString() const = 0;
};

class KeyboardCommand : public Command{
    private:
        CommandType type;
        
    public:
        explicit KeyboardCommand(CommandType cmdType);
        CommandType getType() const override;
        std::string toString() const override;
};

class MouseCommand : public Command{
    private:
        sf::Vector2i position;
        
    public:
        explicit MouseCommand(const sf::Vector2i& pos);
        CommandType getType() const override;
        std::string toString() const override;
        sf::Vector2i getPosition() const;
};

#endif