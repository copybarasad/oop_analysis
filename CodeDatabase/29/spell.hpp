#ifndef SPELL_HPP
#define SPELL_HPP

#include <memory>
#include <string>

class Game;
class ISpell {
public:
	virtual ~ISpell() = default;
	virtual const char* Name() const noexcept = 0;
	virtual int Range() const noexcept = 0;
	virtual bool Use(Game& game, int tx, int ty) = 0;
	virtual std::string Serialize() const = 0;

	static std::unique_ptr<ISpell> Deserialize(const std::string& code);
};

#endif
