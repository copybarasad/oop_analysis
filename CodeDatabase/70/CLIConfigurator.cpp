#include "CLIConfigurator.h"

std::map<char, COMMAND_TYPE> CLIConfigurator::initDefaultCommands()
{
	std::map<char, COMMAND_TYPE> commandMap;

	commandMap['q'] = COMMAND_TYPE::PAUSE;
    commandMap['w'] = COMMAND_TYPE::UP;
    commandMap['a'] = COMMAND_TYPE::LEFT;
    commandMap['s'] = COMMAND_TYPE::DOWN;
    commandMap['d'] = COMMAND_TYPE::RIGHT;
    commandMap['z'] = COMMAND_TYPE::SHOWENEMIES;
    commandMap['f'] = COMMAND_TYPE::ATTACK;
    commandMap['x'] = COMMAND_TYPE::SWAPRANGE;
    commandMap['p'] = COMMAND_TYPE::PASS;
    commandMap['b'] = COMMAND_TYPE::BUYSPELL;
    commandMap['g'] = COMMAND_TYPE::SHOWHAND;
    commandMap['y'] = COMMAND_TYPE::USESPELL;
    return commandMap;
}

std::map<std::string, COMMAND_TYPE> CLIConfigurator::initConvertionMap()
{
	std::map<std::string, COMMAND_TYPE> convertionMap;

	convertionMap["PAUSE"] = COMMAND_TYPE::PAUSE;
    convertionMap["UP"] = COMMAND_TYPE::UP;
    convertionMap["LEFT"] = COMMAND_TYPE::LEFT;
    convertionMap["DOWN"] = COMMAND_TYPE::DOWN;
    convertionMap["RIGHT"] = COMMAND_TYPE::RIGHT;
    convertionMap["SHOWENEMIES"] = COMMAND_TYPE::SHOWENEMIES;
    convertionMap["ATTACK"] = COMMAND_TYPE::ATTACK;
    convertionMap["SWAPRANGE"] = COMMAND_TYPE::SWAPRANGE;
    convertionMap["PASS"] = COMMAND_TYPE::PASS;
    convertionMap["BUYSPELL"] = COMMAND_TYPE::BUYSPELL;
    convertionMap["SHOWHAND"] = COMMAND_TYPE::SHOWHAND;
    convertionMap["USESPELL"] = COMMAND_TYPE::USESPELL;
    return convertionMap;
}

void CLIConfigurator::changeCommand(std::map<char, COMMAND_TYPE>& commandMap, COMMAND_TYPE option, char command)
{
	auto it = commandMap.begin();
    while (it != commandMap.end()) {
        if (it->second == option) {
            it = commandMap.erase(it);  // erase возвращает следующий валидный итератор
            break;
        } else {
            ++it;
        }
    }
    commandMap[command] = option;
}


std::map<char, COMMAND_TYPE> CLIConfigurator::mergeCFGs(std::map<char, COMMAND_TYPE> commandUserMap, std::map<char, COMMAND_TYPE> defaultCommandMap)
{
    //MERGING
    std::map<char, COMMAND_TYPE> commandMap = commandUserMap;

    for (const auto& defaultPair : defaultCommandMap) {
        char defaultKey = defaultPair.first;
        COMMAND_TYPE defaultCmd = defaultPair.second;
        
        //CHECK IF BIND WAS SET
        bool setBind = false;
        for (const auto& commandPair : commandMap)
        {
            COMMAND_TYPE commandCmd = commandPair.second;
            if (defaultCmd == commandCmd)
            {
                setBind = true;
                break;
            }
        }

        if (setBind) continue;

        //CHECKING IF THE DEFAULT KEY WAS USED
        if (commandMap.find(defaultKey) == commandMap.end()) {
            commandMap[defaultKey] = defaultCmd;
        } else {
            COMMAND_TYPE reversedCmd = commandMap[defaultKey];
            if (reversedCmd != defaultCmd)
            {
                char defaultKeyForReversed;
                for(const auto& defaultPairForReversed : defaultCommandMap)
                {
                    COMMAND_TYPE defaultCmdForReversed = defaultPairForReversed.second;

                    if(reversedCmd == defaultCmdForReversed)
                    {
                        defaultKeyForReversed = defaultPairForReversed.first;
                        break;
                    }
                }
                changeCommand(commandMap, reversedCmd, defaultKeyForReversed);
            }
            commandMap[defaultKey] = defaultCmd;
        }
    }
    return commandMap;
}

std::map<char, COMMAND_TYPE> CLIConfigurator::readCFG()
{
    std::map<char, COMMAND_TYPE> defaultCommandMap = initDefaultCommands();
    std::map<char, COMMAND_TYPE> commandUserMap;
    std::map<std::string, COMMAND_TYPE> convertionMap = initConvertionMap();

    std::ifstream file("cfg");
    std::string line;
    COMMAND_TYPE option;
    std::vector<std::string> words;

    char command;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string word;
            while (ss >> word) {
                words.push_back(word);
            }

            if (words.size() == 2)
            {
		        if (convertionMap.find(words[0]) != convertionMap.end()) {
		        	if (words[1].size() == 1)
		        	{
						command = words[1][0];
						option = convertionMap[words[0]];
						if (commandUserMap.find(command) != commandUserMap.end()) 
						{
							commandUserMap.erase(command);
			        	} else
			        	{
			        		commandUserMap[command] = option;
			        	}
		        	}
			    }
			}
		    words.clear();
        }
        file.close();
    }   	

    std::map<char, COMMAND_TYPE> commandMap = mergeCFGs(commandUserMap, defaultCommandMap);


    return commandMap;
}

