#include "game.h"
#include "field.h"
#include "player_hand.h"
#include "ispell.h"
#include "error.h"

#include <iostream>
#include <fstream>

namespace game {

    Game::Game(GameConfig cfg, std::mt19937 rng)
        : baseCfg_(cfg)
        , cfg_(cfg)
        , rng_(std::move(rng))
        , field_(std::nullopt)
        , state_(GameState::MainMenu)
        , exitRequested_(false)
        , currentLevel_(1)
        , resumeFromSave_(false)
    {}

    void Game::openPopup(PopupType type, std::string text, GameState returnState)
    {
        popupType_ = type;
        popupText_ = text;
        popupReturnState_ = returnState;

        state_ = GameState::Popup;
    }

    void Game::startNewGame() {
        currentLevel_ = 1;
        savedHand_.reset();
        savedFightMode_.reset();

        cfg_ = baseCfg_;
        field_.reset();
        resumeFromSave_ = false;

        state_ = GameState::Help;
    }

    // In future: LevelComplicator class
    void Game::prepareConfigForLevel()
    {
        cfg_ = baseCfg_;

        // Field size (+[2x2] per level)
        cfg_.defaultSize = std::min(
            baseCfg_.defaultSize + (currentLevel_ - 1) * 2,
            baseCfg_.maxSize
        );

        // Enemy damage (+2 DMG per level)
        cfg_.enemyDefaultDamage += (currentLevel_ - 1) * 2;

        // Enemy HP (+10 HP per level)
        cfg_.enemyDefaultHP += (currentLevel_ - 1) * 10;

        // Enemy spawn interval (-1 turn per 2 level)
        cfg_.defaultSpawnInterval = std::max(
            cfg_.defaultSpawnInterval - ((currentLevel_ - 1) / 2),
            cfg_.minSpawnInterval
        );

        // Enemy tower cooldown (-1 turn per 2 level)
        cfg_.eTowerCooldown = std::max(
            cfg_.eTowerCooldown - ((currentLevel_ - 1) / 2),
            cfg_.eTowerMinCooldown
        );
    }

    void Game::startLevel()
    {
        prepareConfigForLevel();

        const int w = cfg_.defaultSize;
        const int h = cfg_.defaultSize;

        cfg_.maxEnemies = w * h / 20;
        cfg_.eBuildingInitialPosition = Pos(w - 2, h - 2);
        cfg_.eTowerInitialPosition    = Pos(w - 2, 1);

        field_.emplace(w, h, cfg_, rng_);

        if (savedHand_) {
            field_->setPlayerHand(*savedHand_);
        }

        if (savedFightMode_) {
            if (*savedFightMode_ != field_->getPlayer().getFightMode()) {
                field_->switchPlayerFightMode();
            }
        }

        resumeFromSave_ = false;
        state_ = GameState::RunningLevel;
    }

    void Game::onLevelWin()
    {
        const PlayerHand& hand = field_->getPlayer().getHand();

        if (savedHand_) {
            *savedHand_ = hand;
        } else {
            savedHand_.emplace(hand);
        }

        savedHand_->removeRandomHalf(rng_);
        if (savedHand_->empty()) {
            savedHand_->addRandomSpell(cfg_, rng_);
        }

        savedFightMode_ = field_->getPlayer().getFightMode();
    }

    PlayerTurnResult Game::applyInLevel(const Command& cmd)
    {
        if (!field_) return PlayerTurnResult::NoWorldTurn;

        switch (cmd.type) {
            case CommandType::Quit:
                exitRequested_ = true;
                state_ = GameState::Exit;
                return PlayerTurnResult::ExitLevel;
            
            case CommandType::Move:
                field_->movePlayer(cmd.dir);
                return PlayerTurnResult::WorldTurn;
            
            case CommandType::Attack:
                field_->playerAttack();
                return PlayerTurnResult::NoWorldTurn;
            
            case CommandType::SwitchMode:
                field_->switchPlayerFightMode();
                return PlayerTurnResult::NoWorldTurn;
            
            case CommandType::CastSpell:
                field_->tryCastByIndex(
                    cmd.index,
                    field_->findNearestEnemy(field_->getPlayer().getPosition())
                );
                return PlayerTurnResult::NoWorldTurn;
            
            default:
                return PlayerTurnResult::NoWorldTurn;
        }
    }

    void Game::tick(const Command& cmd) {
        // Popup handling: any key closes popup
        if (state_ == GameState::Popup) {
            if (cmd.type == CommandType::PopupConfirm) {
                popupType_ = PopupType::None;
                popupText_.clear();
                state_ = popupReturnState_;
            }
            return;
        }

        switch (state_) {
            case GameState::MainMenu: {
                if (cmd.type == CommandType::MenuQuit || cmd.type == CommandType::Quit) {
                    exitRequested_ = true;
                    state_ = GameState::Exit;
                    return;
                }

                if (cmd.type == CommandType::MenuNewGame) {
                    startNewGame();
                    return;
                }

                if (cmd.type == CommandType::MenuLoadGame) {
                    try {
                        loadGame(baseCfg_.saveName + ".dat");
                        openPopup(PopupType::LoadOk, "", GameState::RunningLevel);
                    } catch (const LoadError& e) {
                        openPopup(PopupType::LoadError, e.what(), GameState::MainMenu);
                    }

                    return;
                }

                return;
            }

            case GameState::Help: {
                if (cmd.type == CommandType::Quit) {
                    exitRequested_ = true;
                    state_ = GameState::Exit;

                    return;
                }

                if (cmd.type == CommandType::HelpOpenSizeMenu) {
                    state_ = GameState::SizeMenu;
                    return;
                }

                if (cmd.type == CommandType::HelpConfirm) {
                    startLevel();
                    return;
                }

                return;
            }

            case GameState::SizeMenu: {
                if (cmd.type == CommandType::Quit) {
                    exitRequested_ = true;
                    state_ = GameState::Exit;

                    return;
                }

                if (cmd.type == CommandType::SetFieldSize) {
                    int w = cmd.w;
                    int h = cmd.h;

                    w = std::max(baseCfg_.minSize, std::min(w, baseCfg_.maxSize));
                    h = std::max(baseCfg_.minSize, std::min(h, baseCfg_.maxSize));

                    baseCfg_.defaultSize = w;
                    cfg_.defaultSize     = w;

                    (void)h;

                    state_ = GameState::Help;
                }
                return;
            }

            case GameState::RunningLevel: {
                if (!field_) {
                    state_ = GameState::MainMenu;
                    return;
                }

                if (!field_->isPlayerAlive()) {
                    state_ = GameState::RestartPrompt;
                    return;
                }
                if (field_->enemiesAlive() == 0) {
                    onLevelWin();
                    state_ = GameState::LevelUp;
                    return;
                }

                if (cmd.type == CommandType::Save) {
                    try {
                        saveGame(baseCfg_.saveName + ".dat");
                        openPopup(PopupType::SaveOk, "", GameState::RunningLevel);
                    } catch (const SaveError& e) {
                        openPopup(PopupType::SaveError, e.what(), GameState::RunningLevel);
                    }
                    return;
                }

                if (cmd.type == CommandType::Load) {
                    try {
                        loadGame(baseCfg_.saveName + ".dat");
                        openPopup(PopupType::LoadOk, "", GameState::RunningLevel);
                    } catch (const LoadError& e) {
                        openPopup(PopupType::LoadError, e.what(), GameState::RunningLevel);
                    }
                    return;
                }

                PlayerTurnResult r = applyInLevel(cmd);
                if (r == PlayerTurnResult::ExitLevel) return;

                if (r == PlayerTurnResult::WorldTurn) {
                    field_->nextTurn();
                }

                // Recheck
                if (!field_->isPlayerAlive()) {
                    state_ = GameState::RestartPrompt;
                    return;
                }
                if (field_->enemiesAlive() == 0) {
                    onLevelWin();
                    state_ = GameState::LevelUp;
                    return;
                }

                return;
            }

            case GameState::RestartPrompt: {
                if (cmd.type == CommandType::RestartNo || cmd.type == CommandType::Quit) {
                    exitRequested_ = true;
                    state_ = GameState::Exit;
                    return;
                }

                if (cmd.type == CommandType::RestartYes) {
                    if (currentLevel_ > 1) currentLevel_ -= 1;
                    field_.reset();
                    resumeFromSave_ = false;

                    if (currentLevel_ == 1) {
                        state_ = GameState::Help;
                    }
                    else {
                        startLevel();
                    }

                    return;
                }

                return;
            }

            case GameState::LevelUp: {
                bool chosen = true;

                if (cmd.type == CommandType::LevelUp1) {
                    baseCfg_.playerDefaultHP += baseCfg_.hpUpgradeValue;

                } else if (cmd.type == CommandType::LevelUp2) {
                    baseCfg_.playerMeleeDamage  += baseCfg_.damageUpgradeValue;
                    baseCfg_.playerRangedDamage += (int)(baseCfg_.damageUpgradeValue / 2);

                } else if (cmd.type == CommandType::LevelUp3) {
                    if (!savedHand_) {
                        savedHand_.emplace(baseCfg_.playerHandCapacity);
                    }

                    const ISpell* upgraded = nullptr;
                    std::string msg;

                    if (savedHand_->upgradeRandomSpell(cfg_, rng_, &upgraded) && upgraded) {
                        msg = upgraded->name();
                    } else {
                        msg = "No spells to upgrade...";
                    }

                    currentLevel_ += 1;
                    field_.reset();
                    resumeFromSave_ = false;

                    startLevel();
                    openPopup(PopupType::SpellUpgraded, msg, GameState::RunningLevel);

                } else {
                    chosen = false;
                }

                if (!chosen) return;

                currentLevel_ += 1;
                field_.reset();
                resumeFromSave_ = false;

                startLevel();
                return;
            }

            case GameState::Exit: {
                exitRequested_ = true;
                return;
            }

            case GameState::Popup:
                return;
        }
    }

    void Game::saveGame(const std::string& filename) const
    {
        std::ofstream out(filename, std::ios::binary);
        if (!out) {
            throw SaveError("Failed to open file for saving: " + filename);
        }

        // Save header (save version)
        out << "SAVE v" << baseCfg_.saveVersion << '\n';
        
        // Current level
        out << currentLevel_ << '\n';

        // Basic field size
        out << baseCfg_.defaultSize << '\n';

        // RNG
        out << rng_ << '\n';

        // Field
        if (field_) {
            out << 1 << '\n';
            field_->save(out);
        } else {
            out << 0 << '\n';
        }
    }

    void Game::loadGame(const std::string& filename)
    {
        std::ifstream in(filename, std::ios::binary);
        if (!in) {
            throw LoadError("Failed to open save file: " + filename);
        }
        
        // Save version
        std::string magic;
        if (!std::getline(in, magic) || magic != ("SAVE v" + baseCfg_.saveVersion)) {
            throw LoadError(
                "Invalid save file header or old save version: " + magic
                + "\nActual save version: SAVE v" + baseCfg_.saveVersion
            );
        }

        // Current level
        int level = 1;
        if (!(in >> level)) {
            throw LoadError("Failed to read level from save file");
        }
        currentLevel_ = std::max(level, 1);

        // Basic field size
        int baseSize = 10;
        if (!(in >> baseSize)) {
            throw LoadError("Failed to read basic field size from save file");
        }
        baseCfg_.defaultSize = baseSize;

        // RNG
        if (!(in >> rng_)) {
            throw LoadError("Failed to read RNG state from save file");
        }

        int hasField = 0;
        if (!(in >> hasField)) {
            throw LoadError("Failed to read field state from save file");
        }

        if (hasField) {
            prepareConfigForLevel();

            int w = 0, h = 0;
            if (!(in >> w >> h)) {
                throw LoadError("Failed to read field size from save file");
            }

            Field tmpField(w, h, cfg_, rng_);
            tmpField.load(in);

            field_.emplace(std::move(tmpField));
            resumeFromSave_ = true;

        } else {
            field_.reset();
            resumeFromSave_ = false;
        }
    }
}