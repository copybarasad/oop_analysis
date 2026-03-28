  #ifndef CONTROLCONFIG_H
#define CONTROLCONFIG_H

#include <map>
#include <string>

// Класс для загрузки и хранения настроек управления
class ControlConfig {
public:
	ControlConfig();
	
	bool loadFromFile(const std::string& filename);
	
	std::string getCommandForKey(char key) const;
	
	char getKeyForCommand(const std::string& command) const;
	
	//команда -> клавиша
	std::map<std::string, char> getAllBindings() const;
	
	void setDefaultBindings();

private:
	std::map<char, std::string> keyToCommand_;
	std::map<std::string, char> commandToKey_;
	
	bool validateConfig() const;
};

#endif 

