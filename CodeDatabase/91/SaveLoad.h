#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H

#include "json.hpp"
#include "Game.h"
#include "Spells/DamageSpell.h"
#include "Spells/AreaSpell.h"
#include "Spells/TrapSpell.h"
#include "Towers/Tower.h"
#include <string>
#include <memory>

// Player
void to_json(nlohmann::json& j, const Player& p);
void from_json(const nlohmann::json& j, Player& p);

// SpellHand
void to_json(nlohmann::json& j, const SpellHand& sh);
void from_json(const nlohmann::json& j, SpellHand& sh);

// ISpell
void to_json(nlohmann::json& j, const ISpell& s);
void from_json(const nlohmann::json& j, std::unique_ptr<ISpell>& s);

// DamageSpell
void to_json(nlohmann::json& j, const DamageSpell& s);
void from_json(const nlohmann::json& j, DamageSpell& s);

// AreaSpell
void to_json(nlohmann::json& j, const AreaSpell& s);
void from_json(const nlohmann::json& j, AreaSpell& s);

// TrapSpell
void to_json(nlohmann::json& j, const TrapSpell& s);
void from_json(const nlohmann::json& j, TrapSpell& s);

// Enemy
void to_json(nlohmann::json& j, const Enemy& e);
void from_json(const nlohmann::json& j, Enemy& e);

// GameField
void to_json(nlohmann::json& j, const GameField& gf);
void from_json(const nlohmann::json& j, GameField& gf);

// ITower
void to_json(nlohmann::json& j, const ITower& t);
void from_json(const nlohmann::json& j, std::unique_ptr<ITower>& t);

// Tower
void to_json(nlohmann::json& j, const Tower& t);
void from_json(const nlohmann::json& j, Tower& t);

// Game
void to_json(nlohmann::json& j, const Game& g);
void from_json(const nlohmann::json& j, Game& g);

// === Save/Load ===
void saveGame(const Game& game, const std::string& filepath);
std::unique_ptr<Game> loadGame(const std::string& filepath);

#endif