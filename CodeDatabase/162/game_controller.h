#ifndef GAME_CONTROLLER_H_
#define GAME_CONTROLLER_H_

#include <memory>
#include <vector>
#include <unordered_map>
#include <functional>
#include <string>
#include "input_handler.h"
#include "player.h"
#include "field.h"

template<typename T>
class InputHandlerInterface {
public:
    virtual ~InputHandlerInterface() = default;
    virtual typename T::Command ParseInput(const std::string& input) const = 0;
};

class ConsoleInputHandler : public InputHandlerInterface<InputHandler> {
public:
    ConsoleInputHandler() : handler_() {
        handler_.LoadControls(); 
    }

    using Command = InputHandler::Command;
    
    InputHandler::Command ParseInput(const std::string& input) const override {
        return handler_.ParseInput(input); 
    }
    
private:
    mutable InputHandler handler_;  
};

template<typename InputHandlerType>
class GameController {
public:
    using Command = typename InputHandlerType::Command;
    
    struct ProcessResult {
        bool turn_used; 
        enum class Action { Continue, Save, Load, Quit, Invalid } action; 
    };
    
    GameController(Field* field, Player* player) 
        : field_(field), player_(player), input_handler_(std::make_shared<InputHandlerType>()) {
        SetupCommandHandlers();
    }
    
    ~GameController() = default;
    
    ProcessResult ProcessInput(const std::string& input) {
        auto cmd = input_handler_->ParseInput(input);
        
        if (cmd.action.empty() || cmd.action == "unknown") {
            return {false, ProcessResult::Action::Invalid};
        }
        
        auto it = command_handlers_.find(cmd.action);
        if (it != command_handlers_.end()) {
            return it->second(cmd);
        }
        
        return {false, ProcessResult::Action::Invalid};
    }
    
    std::vector<std::string> GetAvailableCommands() const {
        std::vector<std::string> commands;
        for (const auto& pair : command_handlers_) {
            commands.push_back(pair.first);
        }
        return commands;
    }
    
    InputHandlerType* GetInputHandler() const {
        return input_handler_.get();
    }
    
private:
    void SetupCommandHandlers() {
        command_handlers_["move_up"] = [this](const Command& cmd) {
            if (!field_->IsPlayerSlowed()) { 
                bool used = field_->MovePlayer(0, -1);
                if (used) return ProcessResult{true, ProcessResult::Action::Continue};
            }
            return ProcessResult{false, ProcessResult::Action::Continue};
        };
        command_handlers_["move_left"] = [this](const Command& cmd) {
            if (!field_->IsPlayerSlowed()) {
                bool used = field_->MovePlayer(-1, 0);
                if (used) return ProcessResult{true, ProcessResult::Action::Continue};
            }
            return ProcessResult{false, ProcessResult::Action::Continue};
        };
        command_handlers_["move_down"] = [this](const Command& cmd) {
            if (!field_->IsPlayerSlowed()) {
                bool used = field_->MovePlayer(0, 1);
                if (used) return ProcessResult{true, ProcessResult::Action::Continue};
            }
            return ProcessResult{false, ProcessResult::Action::Continue};
        };
        command_handlers_["move_right"] = [this](const Command& cmd) {
            if (!field_->IsPlayerSlowed()) {
                bool used = field_->MovePlayer(1, 0);
                if (used) return ProcessResult{true, ProcessResult::Action::Continue};
            }
            return ProcessResult{false, ProcessResult::Action::Continue};
        };
        
        command_handlers_["attack"] = [this](const Command& cmd) {
            if (cmd.HasArgs(2)) {
                int x = cmd.GetIntArg(0);
                int y = cmd.GetIntArg(1);
                field_->AttackEnemiesAt(x, y);
                return ProcessResult{true, ProcessResult::Action::Continue};
            }
            return ProcessResult{false, ProcessResult::Action::Continue};
        };
        
        command_handlers_["switch_mode"] = [this](const Command& cmd) {
            field_->PlayerSwitchMode();
            return ProcessResult{false, ProcessResult::Action::Continue};
        };
        
        command_handlers_["buy_spell"] = [this](const Command& cmd) {
            player_->BuySpell();
            return ProcessResult{false, ProcessResult::Action::Continue};
        };
        
        command_handlers_["cast"] = [this](const Command& cmd) {
            if (cmd.HasArgs(3)) {
                int index = cmd.GetIntArg(0);
                int x = cmd.GetIntArg(1);
                int y = cmd.GetIntArg(2);
                bool success = player_->GetHand().UseSpell(index, *player_, *field_, x, y);
                if (success) return ProcessResult{true, ProcessResult::Action::Continue};
            }
            return ProcessResult{false, ProcessResult::Action::Continue};
        };
        
        command_handlers_["info"] = [this](const Command& cmd) {
            std::cout << "\n=== DETAILED INFO ===\n";
            std::cout << "Player position: (" << field_->GetPlayerX() << ", " << field_->GetPlayerY() << ")\n";
    
            auto findClosest = [&](const std::vector<std::pair<int, int>>& positions, bool isEntityVec = false, const auto& entityVec = std::vector<std::unique_ptr<Enemy>>{}) -> std::tuple<std::pair<int, int>, int, int> {
                std::pair<int, int> closest = {-1, -1};
                int minDist = INT_MAX;
                int health = -1;
                for (size_t i = 0; i < positions.size(); ++i) {
                    const auto& pos = positions[i];
                    int dist = std::abs(pos.first - field_->GetPlayerX()) + std::abs(pos.second - field_->GetPlayerY());
                    if (dist < minDist) {
                        minDist = dist;
                        closest = pos;
                        if (isEntityVec && i < entityVec.size() && entityVec[i]) {
                            health = entityVec[i]->GetHealth();
                        }
                    }
                }
                return {closest, minDist, health};
            };
    
            // Closest enemy
            auto [closest_enemy, min_dist_enemy, enemy_health] = findClosest(field_->GetEnemiesPositions(), true, field_->GetEnemies());
            if (closest_enemy.first != -1) {
                std::cout << "Closest enemy: (" << closest_enemy.first << ", " << closest_enemy.second 
                          << ") distance: " << min_dist_enemy;
                if (enemy_health != -1) std::cout << ", health: " << enemy_health;
                std::cout << "\n";
            } else {
                std::cout << "No enemies detected.\n";
            }
    
            // Closest building
            auto [closest_building, min_dist_building, building_health] = findClosest(field_->GetBuildingPositions(), true, field_->GetBuildings());
            if (closest_building.first != -1) {
                std::cout << "Closest building: (" << closest_building.first << ", " << closest_building.second 
                          << ") distance: " << min_dist_building;
                if (building_health != -1) std::cout << ", health: " << building_health;
                std::cout << "\n";
            } else {
                std::cout << "No buildings detected.\n";
            }
    
            auto [closest_tower, min_dist_tower, tower_health] = findClosest(field_->GetTowerPositions(), true, field_->GetTowers());
            if (closest_tower.first != -1) {
                std::cout << "Closest tower: (" << closest_tower.first << ", " << closest_tower.second 
                          << ") distance: " << min_dist_tower;
                if (tower_health != -1) std::cout << ", health: " << tower_health;
                std::cout << "\n";
            } else {
                std::cout << "No towers detected.\n";
            }
    
            auto [closest_ally, min_dist_ally, ally_health] = findClosest(field_->GetAllyPositions(), true, field_->GetAllies());
            if (closest_ally.first != -1) {
                std::cout << "Closest ally: (" << closest_ally.first << ", " << closest_ally.second 
                          << ") distance: " << min_dist_ally;
                if (ally_health != -1) std::cout << ", health: " << ally_health;
                std::cout << "\n";
            } else {
                std::cout << "No allies detected.\n";
            }
     
            std::cout << "Total enemies: " << field_->GetEnemies().size() << "\n";
            std::cout << "Total buildings: " << field_->GetBuildings().size() << "\n";
            std::cout << "Total towers: " << field_->GetTowers().size() << "\n";
            std::cout << "Total allies: " << field_->GetAllies().size() << "\n";
    
            return ProcessResult{false, ProcessResult::Action::Continue};
        };
            
        
        command_handlers_["save"] = [this](const Command& cmd) {
            return ProcessResult{false, ProcessResult::Action::Save};
        };
        command_handlers_["load"] = [this](const Command& cmd) {
            return ProcessResult{false, ProcessResult::Action::Load};
        };
        command_handlers_["quit"] = [this](const Command& cmd) {
            return ProcessResult{false, ProcessResult::Action::Quit};
        };
        
        command_handlers_["help"] = [this](const Command& cmd) {
            std::ifstream controlsFile("controls.txt");
            if (!controlsFile.is_open()) {
                std::cout << "Ошибка: файл controls.txt не найден или не может быть открыт.\n";
            } else {
                std::cout << "\n=== CONTROLS ===\n";
                std::string line;
                while (std::getline(controlsFile, line)) {
                    std::cout << line << "\n";
                }
                controlsFile.close();
            }
            return ProcessResult{false, ProcessResult::Action::Continue};
        };
    }
    
private:
    Field* field_;
    Player* player_;
    std::shared_ptr<InputHandlerType> input_handler_;
    std::map<std::string, std::function<ProcessResult(const Command&)>> command_handlers_;
};

#endif // GAME_CONTROLLER_H_
