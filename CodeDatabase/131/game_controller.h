#ifndef OOP_GAME_CONTROLLER_H_
#define OOP_GAME_CONTROLLER_H_

#include "game.h"
#include "game_visualizer.h"
#include "console_renderer.h"
#include "command.h"
#include "game_exceptions.h"
#include "direct_damage_spell.h"
#include "area_damage_spell.h"
#include "spell.h"
#include "spell_hand.h"
#include "player.h"
#include <memory>
#include <iostream>

namespace rpg {
    template<typename TInputReader>
    class GameController final {
    public:
        explicit GameController(Game& game) 
            : game_(game), 
              input_reader_(),
              visualizer_(std::make_unique<GameVisualizer<ConsoleRenderer>>()) {
        }

        GameController(Game& game, const std::string& config_file)
            : game_(game), 
              input_reader_(config_file),
              visualizer_(std::make_unique<GameVisualizer<ConsoleRenderer>>()) {
        }

        void ProcessPlayerTurn() {
            auto command = input_reader_.ReadCommand();
            
            if (!command) {
                return;
            }

            ExecuteCommand(command.get());
        }

        void Visualize() const {
            visualizer_->Visualize(game_);
        }

        void VisualizeWithMessage(const std::string& message) const {
            visualizer_->VisualizeWithMessage(game_, message);
        }

        [[nodiscard]] const GameVisualizer<ConsoleRenderer>& GetVisualizer() const {
            return *visualizer_;
        }

        [[nodiscard]] const TInputReader& GetInputReader() const {
            return input_reader_;
        }
        
        void RunGameLoop() {
            visualizer_->Visualize(game_);
            
            while (game_.IsRunning() && game_.IsPlayerAlive()) {
                ProcessPlayerTurn();
                
                game_.CheckAndHandleLevelCompletion();
                
                game_.CheckAndHandleGameOver();
                
                if (!game_.IsRunning()) {
                    break;
                }
            }
        }

    private:
        Game& game_;
        TInputReader input_reader_;
        std::unique_ptr<GameVisualizer<ConsoleRenderer>> visualizer_;

        void ExecuteCommand(ICommand* command) {
            if (!command) {
                return;
            }

            std::string result;
            
            switch (command->GetType()) {
                case CommandType::kMoveUp:
                case CommandType::kMoveDown:
                case CommandType::kMoveLeft:
                case CommandType::kMoveRight: {
                    auto* move_cmd = dynamic_cast<MoveCommand*>(command);
                    result = game_.ProcessCommand(GameCommand::kMove, move_cmd->GetDirection());
                    break;
                }
                
                case CommandType::kAttack:
                    result = game_.AttackNearby();
                    break;
                
                case CommandType::kSwitchMode:
                    result = game_.SwitchPlayerCombatMode();
                    break;
                
                case CommandType::kBuySpell:
                    result = game_.BuyRandomSpell();
                    break;
                
                case CommandType::kCastSpell0:
                case CommandType::kCastSpell1:
                case CommandType::kCastSpell2:
                case CommandType::kCastSpell3:
                case CommandType::kCastSpell4: {
                    auto* cast_cmd = dynamic_cast<CastSpellCommand*>(command);
                    result = HandleCastSpellCommand(cast_cmd);
                    return;
                }
                
                case CommandType::kSaveGame:
                    result = game_.SaveGame();
                    break;
                
                case CommandType::kQuit:
                    game_.Quit();
                    result = "Игра завершена по команде игрока.";
                    break;
                
                case CommandType::kInvalid:
                default:
                    result = "Неизвестная команда.";
                    break;
            }
            
            if (!result.empty()) {
                visualizer_->VisualizeWithMessage(game_, result);
            } else {
                visualizer_->Visualize(game_);
            }
        }

        std::string HandleCastSpellCommand(CastSpellCommand* cast_cmd) const {
            size_t spell_index = cast_cmd->GetSpellIndex();

            if (spell_index >= game_.GetPlayer().GetSpellHand().GetSize()) {
                visualizer_->VisualizeWithMessage(game_, 
                    "У вас нет заклинания с таким индексом!");
                return "";
            }

            const Spell* spell = game_.GetPlayer().GetSpellHand().GetSpell(spell_index);
            
            if (cast_cmd->HasTarget()) {
                std::string result = game_.CastSpell(spell_index, cast_cmd->GetTarget());
                visualizer_->VisualizeWithMessage(game_, result);
                return result;
            }
            
            if (spell->GetType() == SpellType::kEnhancement) {
                std::string result = game_.CastSpell(spell_index, game_.GetPlayer().GetPosition());
                visualizer_->VisualizeWithMessage(game_, result);
                return result;
            }

            int range = 2;
            int area_size = 1;
            
            if (spell->GetType() == SpellType::kDirectDamage) {
                range = DirectDamageSpell::kDefaultRange;
            } else if (spell->GetType() == SpellType::kAreaDamage) {
                range = AreaDamageSpell::kDefaultRange;
                area_size = AreaDamageSpell::kDefaultAreaSize;
            } else if (spell->GetType() == SpellType::kTrap) {
                range = 1;
            } else if (spell->GetType() == SpellType::kSummon) {
                range = 2;
            }

            auto target = visualizer_->SelectTarget(
                game_, 
                game_.GetPlayer().GetPosition(), 
                range, 
                area_size, 
                spell->GetType()
            );

            if (target.has_value()) {
                std::string result = game_.CastSpell(spell_index, target.value());
                visualizer_->VisualizeWithMessage(game_, result);
                return result;
            } else {
                visualizer_->VisualizeWithMessage(game_, "Применение заклинания отменено.");
                return "";
            }
        }
    };
}

#endif
