#pragma once
#include <string>

class Player;
class PlayingField;
class PlayerHand;
enum class SpellType;

enum class SpellCastResult {
	SUCCESS,            
	MISS,               
	INVALID_TARGET      
};

class Spell {
public:
	virtual ~Spell() = default;
	virtual SpellCastResult apply(Player* caster, PlayingField& field) = 0;
	virtual std::string getName() const = 0;
	virtual std::string getDescription() const = 0;
	virtual SpellType getType() const = 0;
};