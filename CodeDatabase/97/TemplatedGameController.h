#ifndef UNTITLED_TEMPLATEDGAMECONTROLLER_H
#define UNTITLED_TEMPLATEDGAMECONTROLLER_H

#include "GameState.h"
#include "LevelManager.h"
#include "ProgressionSystem.h"
#include "InputHandler.h"
#include "GameView.h"
#include "MovementSystem.h"
#include "CombatSystem.h"
#include "EnemyAISystem.h"
#include "SpawnerSystem.h"
#include "BuildingSystem.h"
#include "SaveManager.h"
#include "Player.h"

#include <memory>
#include <iostream>
#include <sstream>
#include <cctype>

constexpr int MaxLevel = 3;

template <class InputT, class VisualizerT>
class TemplatedGameController {
public:
    explicit TemplatedGameController(GameState& state, const std::string& inputCfg = "")
            : state_(state),
              levelMgr_(std::make_unique<LevelManager>()),
              prog_(std::make_unique<ProgressionSystem>()),
              input_(inputCfg),
              visualizer_(),
              combat_(std::make_unique<CombatSystem>()),
              mover_(std::make_unique<MovementSystem>(state_.model(), *combat_)),
              ai_(std::make_unique<EnemyAISystem>()),
              spawner_(std::make_unique<SpawnerSystem>()),
              building_(std::make_unique<BuildingSystem>())
    {
        levelMgr_->populateLevel(state_);
    }

    ~TemplatedGameController() = default;

    void run() {
        bool running = true;
        while (running) {
            visualizer_.onGameUpdated(state_);

            auto pc = state_.playerCoord();
            if (!pc) {
                std::cout << "Player missing. Game over. Enter 'r' to restart or 'q' to quit: ";
                std::string ans; if (!std::getline(std::cin, ans)) break;
                if (!ans.empty() && std::tolower(ans[0])=='r') {
                    state_.setLevel(1);
                    state_.setGrantsGiven(0);
                    state_.setPlayer(nullptr, std::nullopt);
                    levelMgr_->populateLevel(state_);
                    continue;
                } else break;
            }

            auto pcEntity = state_.model().field().cellAt(*pc).getEntity();
            auto playerPtr = std::dynamic_pointer_cast<Player>(pcEntity);
            if (!playerPtr || !playerPtr->isAlive()) {
                std::cout << "You died. Enter 'r' to restart or 'q' to quit: ";
                std::string ans; if (!std::getline(std::cin, ans)) break;
                if (!ans.empty() && std::tolower(ans[0])=='r') {
                    state_.setLevel(1);
                    state_.setGrantsGiven(0);
                    state_.setPlayer(nullptr, std::nullopt);
                    levelMgr_->populateLevel(state_);
                    continue;
                } else break;
            }

            Command cmd = input_.readCommand();

            switch (cmd.type) {
                case CommandType::Quit:
                    running = false;
                    break;

                case CommandType::ShowHand: {
                    const auto &spells = playerPtr->hand().spells();
                    std::cout << "Hand ("<<spells.size()<<"/"<<playerPtr->hand().capacity()<<"):\n";
                    for (size_t i=0;i<spells.size();++i) std::cout<<i<<": "<<spells[i]<<"\n";
                    break;
                }

                case CommandType::Save: {
                    if (cmd.arg.empty()) { std::cout<<"Usage: save <file>\n"; break; }
                    try { SaveManager::save(state_, cmd.arg); std::cout<<"Saved to "<<cmd.arg<<"\n";} catch(const std::exception &e) { std::cout<<"Save failed: "<<e.what()<<"\n";} break;
                }

                case CommandType::Load: {
                    if (cmd.arg.empty()) { std::cout<<"Usage: load <file>\n"; break; }
                    try { SaveManager::load(state_, cmd.arg); std::cout<<"Loaded from "<<cmd.arg<<"\n";} catch(const std::exception &e) { std::cout<<"Load failed: "<<e.what()<<"\n";} break;
                }

                case CommandType::SwitchMode: {
                    playerPtr->switchMode();
                    enemiesTurn();
                    spawnerTurn();
                    buildingsTurn();
                    break;
                }

                case CommandType::Cast: {
                    std::optional<Coord> target = cmd.target;
                    bool ok = playerPtr->hand().cast(cmd.castIndex, state_.model().field(), *pc, target, playerPtr);
                    if (!ok) std::cout << "Spell failed (invalid index/target)\n";
                    else {
                        std::cout << "Spell used.\n";
                        int currentScore = playerPtr->score();
                        int timesShouldHave = currentScore / state_.nextSpellThreshold();
                        while (state_.grantsGiven() < timesShouldHave) {
                            if (playerPtr->hand().addSpell("DirectDamage")) std::cout<<"You gained a new spell!\n";
                            state_.setGrantsGiven(state_.grantsGiven()+1);
                        }
                        enemiesTurn();
                        spawnerTurn();
                        buildingsTurn();
                    }
                    break;
                }

                case CommandType::MoveUp: {
                    if (playerPtr->consumeSkipMove()) { std::cout<<"You are slowed and cannot move this turn.\n"; enemiesTurn(); spawnerTurn(); buildingsTurn(); break; }
                    Coord dest = *pc; dest.row -= 1;
                    handleMove(pc, dest, playerPtr);
                    break;
                }

                case CommandType::MoveDown: {
                    if (playerPtr->consumeSkipMove()) { std::cout<<"You are slowed and cannot move this turn.\n"; enemiesTurn(); spawnerTurn(); buildingsTurn(); break; }
                    Coord dest = *pc; dest.row += 1;
                    handleMove(pc, dest, playerPtr);
                    break;
                }

                case CommandType::MoveLeft: {
                    if (playerPtr->consumeSkipMove()) { std::cout<<"You are slowed and cannot move this turn.\n"; enemiesTurn(); spawnerTurn(); buildingsTurn(); break; }
                    Coord dest = *pc; dest.col -= 1;
                    handleMove(pc, dest, playerPtr);
                    break;
                }

                case CommandType::MoveRight: {
                    if (playerPtr->consumeSkipMove()) { std::cout<<"You are slowed and cannot move this turn.\n"; enemiesTurn(); spawnerTurn(); buildingsTurn(); break; }
                    Coord dest = *pc; dest.col += 1;
                    handleMove(pc, dest, playerPtr);
                    break;
                }

                default:
                    std::cout<<"Unknown command\n";
            }
        }

        std::cout<<"Exiting game.\n";
    }

    void save(const std::string &filename) const { SaveManager::save(state_, filename); }
    void load(const std::string &filename) { SaveManager::load(state_, filename); }

private:
    void handleMove(std::optional<Coord> fromOpt, const Coord& dest, std::shared_ptr<Player> playerPtr) {
        if (!fromOpt) return;
        Coord from = *fromOpt;
        auto [moved, playerDied, towerDestroyed, enemyKilled, spawnerKilled] = mover_->moveEntity(from, dest);

        if (playerDied) {
            std::cout<<"You died.\n";
            std::cout<<"Enter 'r' to restart or 'q' to quit: ";
            std::string ans; if (!std::getline(std::cin, ans)) { return; }
            if (!ans.empty() && std::tolower(ans[0])=='r') {
                state_.setPlayer(nullptr, std::nullopt); state_.setLevel(1); state_.setGrantsGiven(0); levelMgr_->populateLevel(state_);
                return;
            } else { std::exit(0); }
        }

        if (towerDestroyed) {
            std::cout<<"Tower destroyed! Level completed.\n";
            if(state_.level() >= MaxLevel){
                std::cout << "Congratulations. Y won\n";
                std::exit(0);
            } else {
                state_.setLevel(state_.level() + 1);
                prog_->applyLevelUp(state_);
                levelMgr_->populateLevel(state_);
                return;
            }
        }

        if (enemyKilled) {
            int currentScore = playerPtr->score();
            int timesShouldHave = currentScore / state_.nextSpellThreshold();
            while (state_.grantsGiven() < timesShouldHave) {
                if (playerPtr->hand().addSpell("DirectDamage")) std::cout<<"You gained a new spell!\n";
                state_.setGrantsGiven(state_.grantsGiven()+1);
            }
        }

        enemiesTurn();
        buildingsTurn();
        spawnerTurn();
    }

    void enemiesTurn() {
        ai_->step(state_.model(), *mover_);
    }

    void buildingsTurn() {
        building_->step(state_.model());
    }

    void spawnerTurn() {
        spawner_->step(state_.model());
    }

    GameState& state_;
    std::unique_ptr<LevelManager> levelMgr_;
    std::unique_ptr<ProgressionSystem> prog_;

    InputT input_;
    VisualizerT visualizer_;

    std::unique_ptr<CombatSystem> combat_;
    std::unique_ptr<MovementSystem> mover_;
    std::unique_ptr<EnemyAISystem> ai_;
    std::unique_ptr<SpawnerSystem> spawner_;
    std::unique_ptr<BuildingSystem> building_;
};

#endif // UNTITLED_TEMPLATEDGAMECONTROLLER_H
