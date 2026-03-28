#pragma once

#include <ostream>
#include <string>

class Field;
class GameWorld;

class Renderer {
public:
	enum class Theme { Ascii, Fancy };

	static bool shouldUseAsciiFallback();

	explicit Renderer(Theme theme = Theme::Fancy);
	static void initConsoleOnce();
	void draw(const Field& field, const GameWorld& world, std::ostream& os) const;
	void setTheme(Theme theme) noexcept { theme_ = theme; }

private:
	Theme theme_;

	static std::string fg(int r, int g, int b);
	static std::string reset();

	char wallAscii() const noexcept;
	char slowAscii() const noexcept;
	char floorAscii() const noexcept;
	char playerAscii() const noexcept;
	char enemyAscii() const noexcept;
	char nestAscii() const noexcept;
	char towerAscii() const noexcept;
	char allyAscii() const noexcept;
	char trapAscii() const noexcept;

	std::string wallFancy() const;
	std::string slowFancy() const;
	std::string floorFancy() const;
	std::string playerFancy() const;
	std::string enemyFancy() const;
	std::string nestFancy() const;
	std::string towerFancy() const;
	std::string allyFancy() const;
	std::string trapFancy() const;
};
