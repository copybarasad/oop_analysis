#include "Input.h"
#include "CommandGameControl.h"
#include <conio.h>
#include <memory.h>

using namespace std;

Input::Input() {
    loadConfig();
}

void Input::loadConfig() {
    const string configFile = "keyboardControlConfig.txt";
    
    if (!config.loadFromFile(configFile)) {
        config.saveToFile(configFile);
    }
    
    keyBindings.clear();
    reverseBindings.clear();
    
    auto commands = config.getAllCommands();
    for (const auto& command : commands) {
        char key = config.getKeyForCommand(command);
        keyBindings[command] = key;
        reverseBindings[toupper(key)] = command;
        reverseBindings[tolower(key)] = command;
    }
}

void Input::reloadConfig() {
    loadConfig();
}

void Input::saveConfig(const std::string& filename) {
    config.saveToFile(filename);
}

std::string Input::getCommandForChar(char c) const {
    auto it = reverseBindings.find(toupper(c));
    if (it != reverseBindings.end()) {
        return it->second;
    }
    return "";
}

char Input::getCharForCommand(const std::string& command) const {
    auto it = keyBindings.find(command);
    if (it != keyBindings.end()) {
        return it->second;
    }
    return '\0';
}

bool Input::validateCoordinates(int x, int y, const GameField& field) {
    return x >= 0 && x < field.getWidth() && y >= 0 && y < field.getHeight();
}

bool Input::validateDirection(char direction) {
    return direction == 'w' || direction == 'W' ||
           direction == 'a' || direction == 'A' ||
           direction == 's' || direction == 'S' ||
           direction == 'd' || direction == 'D';
}

void Input::clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::unique_ptr<Command> Input::processSpellInput(SpellID spellID, Player& player, GameField& field) {
    int x, y;

    switch(spellID) {
        case MEGASHOTGUN: {
            cout << "Введите угол в который вы хотите применить MegaShotgun" << endl;
            cout << "1 - RIGHTUP, 2 - RIGHTDOWN, 3 - LEFTUP, 4 - LEFTDOWN" << endl;
            int direction;
            cin >> direction;
            if (std::cin.fail()) {
                std::cout << "Ошибка: введена строка вместо числа!" << std::endl;
                clearInputBuffer();
                return nullptr;
            }                        
            int range = player.getHandSpell()->getMegaShotgun()->getAreaOfEffect();
            switch (direction) {
                case 1: x = player.getX() + range; y = player.getY() - range; break;
                case 2: x = player.getX() + range; y = player.getY() + range; break;
                case 3: x = player.getX() - range; y = player.getY() - range; break;
                case 4: x = player.getX() - range; y = player.getY() + range; break;
                default: 
                    cout << "Неверное направление!" << endl;
                    return nullptr;
            }
            return std::make_unique<UseSpellCommand>(spellID, player, field, x, y);
        }
            
        case BULLETARAIN: {
            cout << "Введите координаты куда применить BulletRain (x y): ";
            cin >> x >> y;
            if (std::cin.fail()) {
                std::cout << "Ошибка: введена строка вместо числа!" << std::endl;
                clearInputBuffer();
                return nullptr;
            }                        
            return std::make_unique<UseSpellCommand>(spellID, player, field, x, y);
        }
            
        case CHEAPWHISKEY: {
            cout << "Введите координаты для размещения ловушки CheapWhiskey (x y): ";
            int x, y;
            cin >> x >> y;
            if (std::cin.fail()) {
                std::cout << "Ошибка: введена строка вместо числа!" << std::endl;
                clearInputBuffer();
                return nullptr;
            }                        
            return std::make_unique<UseSpellCommand>(spellID, player, field, x, y);
        }
            
        case GHOSTOFJHONNY: {
            int spawnX, spawnY;
            cout << "Введите координаты для размещения союзника JhonnyGhost (x y): ";
            cin >> x >> y;
            if (std::cin.fail()) {
                std::cout << "Ошибка: введена строка вместо числа!" << std::endl;
                clearInputBuffer();
                return nullptr;
            }
            return std::make_unique<UseSpellCommand>(spellID, player, field, x, y);
        }
            
        case UPDATESPELL: {
            x = 0;
            y = 0;
            return std::make_unique<UseSpellCommand>(spellID, player, field, x, y);
        }
            
        default:
            cout << "Неверный выбор заклинания." << endl;
            return nullptr;
    }
}


std::unique_ptr<Command> Input::processShopInput(Player& player) {
    cout << "Магазин способностей" << endl;
    cout << "1. Добавить BulletRain (3 монеты)" << endl;
    cout << "2. Добавить MegaShotgun (5 монет)" << endl;
    cout << "3. Добавить CheapWhiskey (7 монеты)" << endl;
    cout << "4. Добавить JohnnysGhost (30 монет)" << endl;
    cout << "5. Добавить UpdateSpell (10 монет)" << endl;
    cout << "0. Выход" << endl;
    cout << "Ваши монеты: " << player.getCoins() << endl;
    cout << "Выберите опцию или любую другую клавишу для выхода: ";
    
    char opt = getPlayerInput();
    
    SpellID buySpellID;
    int costPerItem = 0;
    
    switch (opt) {
        case '1': buySpellID = BULLETARAIN; costPerItem = COST_BULLETRAIN; break;
        case '2': buySpellID = MEGASHOTGUN; costPerItem = COST_MEGASHOTGUN; break;
        case '3': buySpellID = CHEAPWHISKEY; costPerItem = COST_CHEAPWHISKEY; break;
        case '4': buySpellID = GHOSTOFJHONNY; costPerItem = COST_JOHNYSGHOST; break;
        case '5': buySpellID = UPDATESPELL; costPerItem = COST_UPDATE_SPELL; break;
        case '0': 
            cout << "Выход из магазина." << endl;
            return nullptr;
        default:
            cout << "Выход из магазина." << endl;
            return nullptr;
    }
    
    
    cout << "Сколько вы хотите купить заклинаний: ";
    int count;
    std::cin >> count;
    if (std::cin.fail()) {
        std::cout << "Ошибка: введена строка вместо числа!" << std::endl;
        clearInputBuffer();
        return nullptr;
    }
    
    if(count <= 0){
        cout << "Ну ты приколист)" << endl;
        return nullptr;
    }
    
    int totalCost = costPerItem * count;
    
    if(player.getCoins() < totalCost){
        cout << "Недостаточно монет для покупки." << endl;
        return nullptr;
    }
    
    cout << "Покупка подтверждена! Куплено " << count << " заклинаний за " << totalCost << " монет." << endl;
    return std::make_unique<BuySpellCommand>(player, buySpellID, count);
}


char Input::getPlayerInput() {
    return _getch();
}

std::unique_ptr<Command> Input::processInput(char input, Player& player, GameField& field, Game& game) {
    char upperInput = toupper(input);
    
    string command = getCommandForChar(input);
    
    if (command == "MOVE_UP" || command == "MOVE_DOWN" || 
        command == "MOVE_LEFT" || command == "MOVE_RIGHT") {
        if(command == "MOVE_UP") input = 'W';
        else if(command == "MOVE_DOWN") input = 'S';
        else if(command == "MOVE_LEFT") input = 'A';
        else if(command == "MOVE_RIGHT") input = 'D';
            
        auto cmd = std::make_unique<PlayeMoVeCommand>(input, field, player);
        return cmd;
    }
    else if (command == "ATTACK") {
        std::cout << "Направление атаки (W/A/S/D): ";
        char attackDir = getPlayerInput();
        
        if (validateDirection(attackDir)) {
            auto cmd = std::make_unique<PlayerAttackCommand>(attackDir, field, player);
            return cmd;
        } else {
            std::cout << "Неверное направление атаки!" << std::endl;
            return nullptr;
        }
    }
    else if (command == "SWITCH_ATTACK") {
        return std::make_unique<SwitchAttackTypeCommand>(player);
    }   
    else if (command == "USE_SPELL") {
        cout << "Какое заклинание использовать?" << endl;
        cout << "(1 - MegaShotgun, 2 - BulletRain, 3 - CheapWhiskey, 4 - JohnnysGhost, 5 - UpdateSpell): ";
        int spell_choice;
        cin >> spell_choice;
        
        if (std::cin.fail()) {
            std::cout << "Ошибка: введена строка вместо числа!" << std::endl;
            clearInputBuffer();
            return nullptr;
        }
        SpellID spellID;
        switch(spell_choice){
            case 1: spellID = MEGASHOTGUN; break;
            case 2: spellID = BULLETARAIN; break;
            case 3: spellID = CHEAPWHISKEY; break;
            case 4: spellID = GHOSTOFJHONNY; break;
            case 5: spellID = UPDATESPELL; break;
            default: 
                std::cout << "Неверный выбор заклинания." << std::endl;
                break;
        }
        return processSpellInput(spellID, player, field);
    }
    else if (command == "OPEN_SHOP") {
        return processShopInput(player);
    }
    else if (command == "QUIT") {
        std::cout << "Выход в главное меню..." << std::endl;
        return std::make_unique<QuitCommand>(game);
    }
    else if (command == "SAVE") {
        std::string saveName = getSaveName();
        return std::make_unique<SaveGameCommand>(game, saveName);
    }
    else if (command == "LOAD") {
        return std::make_unique<LoadGameCommand>(game);
    }
    else {
        std::cout << "Неизвестная команда! (клавиша '" << input << "')" << std::endl;
        return nullptr;
    }
}


int Input::getLevelChoice() {
    int choice = 0;
    
    while (choice < 1 || choice > 3) {
        cout << "\n=== ВЫБОР УРОВНЯ ===" << endl;
        cout << "1. Уровень 1: Ранчо 'Заброшенный Каньон'" << endl;
        cout << "   - Просторное ранчо с укрытиями" << endl;
        cout << "   - Умеренное количество врагов" << endl;
        cout << "   - Идеально для новичков" << endl;
        
        cout << "2. Уровень 2: Город-призрак 'Стимпунк-Сити'" << endl;
        cout << "   - Узкие улочки и разрушенные здания" << endl;
        cout << "   - Больше врагов и ловушек" << endl;
        cout << "   - Требует тактического подхода" << endl;
        
        cout << "3. Уровень 3: Шахта 'Проклятый Рубин'" << endl;
        cout << "   - Тесные туннели и ограниченная видимость" << endl;
        cout << "   - Огромные орды врагов" << endl;
        cout << "   - Высокая сложность для опытных ковбоев" << endl;
        
        cout << "\nВыберите уровень (1-3): ";
        choice = int(getPlayerInput()) - '0';
        
        if (choice < 1 || choice > 3) {
            cout << "Неверный выбор! Пожалуйста, введите число от 1 до 3." << endl;
            choice = 0;
        }
    }
    
    return choice;
}

int Input::getMainMenuChoice() {
    int choice = 0;
    
    while (choice < 1 || choice > 3) {
        cout << "\n=== WILD WEST ADVENTURE ===" << endl;
        cout << "1. Новая игра" << endl;
        cout << "2. Загрузить игру" << endl;
        cout << "3. Выход" << endl;
        cout << "Выберите опцию (1-3): ";
        choice = int(getPlayerInput()) - '0';
        
        if (choice < 1 || choice > 3) {
            cout << "Неверный выбор! Пожалуйста, введите число от 1 до 3." << endl;
            choice = 0;
        }
    }
    cout << endl;
    
    return choice;
}

std::string Input::getSaveName() {
    cout << "\n=== СОХРАНЕНИЕ ИГРЫ ===" << endl;
    cout << "Введите название сохранения: ";
    std::string saveName;
    cin >> saveName;
    return saveName;
}

std::string Input::getPlayerName() {
    cout << "=== СОЗДАНИЕ ПЕРСОНАЖА ===" << endl;
    cout << "Введите имя вашего ковбоя: ";
    std::string name;
    cin >> name;
    
    if (name.empty()) {
        name = "Jhohny";
    }
    cout << "Добро пожаловать, ковбой " << name << "!" << endl;
    return name;
}

int Input::getLoadChoice(const std::vector<std::string>& saves) {
    cout << "\n=== ЗАГРУЗКА ИГРЫ ===" << endl;
    for (size_t i = 0; i < saves.size(); ++i) {
        cout << i + 1 << ". " << saves[i] << endl;
    }
    cout << "0. Назад" << endl;
    
    int choice = -1;
    cout << "Выберите сохранение (1-" << saves.size() << "): ";
    choice = int(getPlayerInput()) - '0';
    
    return choice;
}

char Input::getLevelTransitionChoice() {
    cout << "Хотите перейти на следующий уровень? (y/n): ";
    char choice;
    cin >> choice;
    return choice;
}

int Input::getUpgradeChoice() {
    int choice = 0;
    while (choice < 1 || choice > 3) {
        cout << "\nВыберите характеристику для улучшения (1-3): ";
        choice = int(getPlayerInput()) - '0';
        
        if (choice < 1 || choice > 3) {
            cout << "Неверный выбор! Пожалуйста, введите число от 1 до 3." << endl;
            choice = 0;
        }
    }
    return choice;
}

