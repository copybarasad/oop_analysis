#ifndef OOP_COMMAND_H_
#define OOP_COMMAND_H_

#include "command_types.h"
#include "position.h"
#include <string>
#include <memory>

namespace rpg {
    enum class Direction;

    class ICommand {
    public:
        virtual ~ICommand() = default;
        [[nodiscard]] virtual CommandType GetType() const = 0;
        [[nodiscard]] virtual std::string GetDescription() const = 0;
    };

    class MoveCommand final : public ICommand {
    public:
        explicit MoveCommand(Direction direction);
        
        [[nodiscard]] CommandType GetType() const override;
        [[nodiscard]] std::string GetDescription() const override { return "Move"; }
        [[nodiscard]] Direction GetDirection() const { return direction_; }
        
    private:
        Direction direction_;
    };

    class AttackCommand final : public ICommand {
    public:
        AttackCommand() = default;
        
        [[nodiscard]] CommandType GetType() const override { return CommandType::kAttack; }
        [[nodiscard]] std::string GetDescription() const override { return "Attack"; }
    };

    class SwitchCombatModeCommand final : public ICommand {
    public:
        SwitchCombatModeCommand() = default;
        
        [[nodiscard]] CommandType GetType() const override { return CommandType::kSwitchMode; }
        [[nodiscard]] std::string GetDescription() const override { return "Switch Combat Mode"; }
    };

    class BuySpellCommand final : public ICommand {
    public:
        BuySpellCommand() = default;
        
        [[nodiscard]] CommandType GetType() const override { return CommandType::kBuySpell; }
        [[nodiscard]] std::string GetDescription() const override { return "Buy Spell"; }
    };

    class CastSpellCommand final : public ICommand {
    public:
        explicit CastSpellCommand(size_t spell_index) 
            : spell_index_(spell_index), has_target_(false), target_pos_{0, 0} {}

        CastSpellCommand(size_t spell_index, const Position& target_pos)
            : spell_index_(spell_index), has_target_(true), target_pos_(target_pos) {}
        
        [[nodiscard]] CommandType GetType() const override;
        [[nodiscard]] std::string GetDescription() const override { 
            return "Cast Spell " + std::to_string(spell_index_); 
        }
        
        [[nodiscard]] size_t GetSpellIndex() const { return spell_index_; }
        [[nodiscard]] bool HasTarget() const { return has_target_; }
        [[nodiscard]] const Position& GetTarget() const { return target_pos_; }
        
    private:
        size_t spell_index_;
        bool has_target_;
        Position target_pos_;
    };

    class SaveGameCommand final : public ICommand {
    public:
        SaveGameCommand() = default;
        
        [[nodiscard]] CommandType GetType() const override { return CommandType::kSaveGame; }
        [[nodiscard]] std::string GetDescription() const override { return "Save Game"; }
    };

    class QuitCommand final : public ICommand {
    public:
        QuitCommand() = default;
        
        [[nodiscard]] CommandType GetType() const override { return CommandType::kQuit; }
        [[nodiscard]] std::string GetDescription() const override { return "Quit"; }
    };

    class NullCommand final : public ICommand {
    public:
        NullCommand() = default;
        
        [[nodiscard]] CommandType GetType() const override { return CommandType::kInvalid; }
        [[nodiscard]] std::string GetDescription() const override { return "Invalid Command"; }
    };
}

#endif
