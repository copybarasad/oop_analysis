#pragma once

#include "character.h"
#include "fight.h"
#include "gameconfig.h"
#include "player_hand.h"

#include <random>

namespace game {
    /// @brief Player entity (<- Character)
    class Player final: public Character {
        private:
            FightMode fmode_{FightMode::Melee};
            PlayerHand hand_;
            int meleeDamage_{0};
            int rangedDamage_{0};

            bool isCriticalHit(const GameConfig& cfg, std::mt19937& rng) const;
        public:
            // === Constructors ====
            Player(const GameConfig& cfg, Pos startPos, std::mt19937& rng)
                : Character(cfg.playerDefaultHP, cfg.playerMeleeDamage,
                            startPos),
                  fmode_(FightMode::Melee),
                  hand_(cfg.playerHandCapacity),
                  meleeDamage_(cfg.playerMeleeDamage),
                  rangedDamage_(cfg.playerRangedDamage)
            {
                hand_.addRandomSpell(cfg, rng);     // Random spell in the hand at start
            }

            Player(const Player&) = default;
            Player(Player&&) = default;
            Player& operator=(const Player&) = default;
            Player& operator=(Player&&) = default;

            // == Getters ==
            FightMode getFightMode()    const { return fmode_; }
            PlayerHand&       getHand()       { return hand_; }
            const PlayerHand& getHand() const { return hand_; }

            void setHand(const PlayerHand& hand) { hand_ = hand; }

            // == Gameplay ==
            bool attack(Character& enemy, const GameConfig& cfg, std::mt19937& rng);
            Pos move(Direction dir, int w, int h);
            void switchFightMode();
            void setFightModeById(int id) { fmode_ = static_cast<FightMode>(id); }
    };
}