 #ifndef CONTROL_CONFIG_H
 #define CONTROL_CONFIG_H

 #include "game_command.h"
 #include <map>
 #include <optional>
 #include <string>

 // Перечень поддерживаемых команд для назначения на клавиши
 enum class CommandName {
 	MoveUp,
 	MoveDown,
 	MoveLeft,
 	MoveRight,
 	SwitchMode,
 	RangedAttack,
 	CastSpell,
 	BuySpell,
 	ShowInfo,
 	Save,
 	Load,
 	Quit,
 	Help
 };

 struct CommandBinding {
 	CommandName name;
 	CommandType type;
 	int deltaX = 0;
 	int deltaY = 0;
 	bool requiresDirection = false;
 	bool requiresSpellIndex = false;
 };

 // Класс инкапсулирует настройку управления: сопоставление клавиши и действия.
 class ControlConfig {
 public:
 	static ControlConfig createDefault();
 	static ControlConfig loadFromFile(const std::string& path);

 	const CommandBinding* findBinding(char key) const;
 	std::optional<char> keyFor(CommandName name) const;
 	bool isUsingDefault() const { return fallbackUsed; }

 private:
 	std::map<char, CommandBinding> bindings;
 	bool fallbackUsed = true;
 };

 #endif

