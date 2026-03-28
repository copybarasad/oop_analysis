#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#include "command.h"
#include "game_controller.h"
#include "save_manager.h"
#include "console_renderer.h"
#include <vector>

template<typename RendererPolicy>
class InternalDamageTracker {
private:
    int prevPlayerHealth;
    std::vector<int> prevEnemyHealth;

public:
    explicit InternalDamageTracker(const GameController& game)
        : prevPlayerHealth(game.getPlayer().getHealth()) {
        for (const auto& enemy : game.getEnemies()) {
            prevEnemyHealth.push_back(enemy.getHealth());
        }
    }

    void track(const GameController& game, RendererPolicy& renderer) {
        const Player& player = game.getPlayer();
        const std::vector<Enemy>& enemies = game.getEnemies();

        if (player.getHealth() < prevPlayerHealth) {
            int damage = prevPlayerHealth - player.getHealth();
            renderer.renderPlayerDamage(damage, prevPlayerHealth, player.getHealth());
        }

        for (size_t i = 0; i < enemies.size(); ++i) {
            if (enemies[i].isAlive() && enemies[i].getHealth() < prevEnemyHealth[i]) {
                int damage = prevEnemyHealth[i] - enemies[i].getHealth();
                renderer.renderEnemyDamage(
                    static_cast<int>(i + 1),
                    damage,
                    prevEnemyHealth[i],
                    enemies[i].getHealth(),
                    !enemies[i].isAlive()
                );
            }
        }

        prevPlayerHealth = player.getHealth();
        prevEnemyHealth.clear();
        for (const auto& enemy : enemies) {
            prevEnemyHealth.push_back(enemy.getHealth());
        }
    }
};

template<typename InputPolicy, typename RendererPolicy>
class GameLoop {
private:
    GameController& game;
    InputPolicy inputParser;
    RendererPolicy& renderer;
    InternalDamageTracker<RendererPolicy> damageTracker;

public:
    GameLoop(GameController& game, RendererPolicy& r)
        : game(game), renderer(r), damageTracker(game) {}

    bool runOneTurn() {
        Command cmd = inputParser.parseInput();

        if (isExit(cmd)) {
            renderer.renderGameExited();
            return false;
        }

        bool actionSuccess = executeCommand(cmd);

        if (actionSuccess) {
            game.updateGame();
            damageTracker.track(game, renderer);
        }

        return game.isGameRunning();
    }

private:
    bool executeCommand(const Command& cmd) {
        return std::visit([this](const auto& c) -> bool {
            using T = std::decay_t<decltype(c)>;

            if constexpr (std::is_same_v<T, MoveCommand>) {
                bool result = game.movePlayer(c.dx, c.dy);
                if (!result) {
                    renderer.renderCannotMove();
                } else {
                    renderer.renderPlayerMoved();
                }
                return result;
            }
            else if constexpr (std::is_same_v<T, AttackCommand>) {
                bool result = game.attackEnemy(c.dx, c.dy);
                if (result) {
                    renderer.renderPlayerAttacked(game.getPlayer().getDamage());
                } else {
                    renderer.renderAttackMissed();
                }
                return result;
            }
            else if constexpr (std::is_same_v<T, SwitchModeCommand>) {
                game.switchPlayerCombatMode();
                const Player& p = game.getPlayer();
                renderer.renderModeSwitched(p.getCombatMode(), p.getDamage());
                return true;
            }
            else if constexpr (std::is_same_v<T, BuySpellCommand>) {
                const Player& p = game.getPlayer();
                int oldSize = p.getHand().size();
                const_cast<Player&>(p).buySpell(30);
                if (p.getHand().size() > oldSize) {
                    renderer.renderSpellPurchased();
                } else {
                    if (p.getScore() < 30) {
                        renderer.renderNotEnoughScore();
                    } else {
                        renderer.renderHandFull();
                    }
                }
                return true;
            }
            else if constexpr (std::is_same_v<T, UseSpellCommand>) {
                if (game.usePlayerSpell(c.spellIndex, c.x, c.y)) {
                    renderer.renderSpellUsed();
                    return true;
                } else {
                    renderer.renderSpellFailed();
                    return false;
                }
            }
            else if constexpr (std::is_same_v<T, SaveGameCommand>) {
                try {
                    SaveManager::saveGame(game, c.filename);
                    renderer.renderGameSaved(c.filename);
                } catch (const SaveException& e) {
                    renderer.renderSaveFailed(e.what());
                }
                return true;
            }
            else {
                renderer.renderUnknownCommand();
                return false;
            }
        }, cmd);
    }
};

#endif