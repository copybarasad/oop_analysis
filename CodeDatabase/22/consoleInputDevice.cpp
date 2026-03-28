#include "console/consoleInputDevice.h"
#include "field/field.h"
#include "controllers/IGameView.h"

Command ConsoleInputDevice::getCommand()
{
    char c;
    std::cin >> c;
    if (keymap.count(c))
    {
        return Command(keymap[c]);
    }

    return Command(CommandType::None);
}

Command ConsoleInputDevice::selectIndex(int max)
{
    int value;
    bool flags = true;
    Command cmd{CommandType::SelectIndex};
    while (flags)
    {
        std::cin >> value;
        if (value >= 0 && value <= max)
        {
            
            cmd.index = value;
            flags = !flags;
            
        }
        std::cout << "Invalid input. Try again.\n";
    }
    return cmd;
}

Command ConsoleInputDevice::readString()
{
    std::string s;
    std::cin >> s;
    return Command{CommandType::SelectFilename, {}, {}, s};
}

Command ConsoleInputDevice::readOptions(){
    char opt;
    std::cin >> opt;
    return Command{CommandType::LevelUp, {}, {}, {}, opt};
}

Command ConsoleInputDevice::selectPosition(Field &field, int radius, IGameView &render)
{
    Position center{0, 0};
    Command cmd = Command{CommandType::SelectPosition, center};
    bool flag = true;
    while (flag)
    {
        field.markRegion(center, radius, true);
        render.drawField(field);

        char cmdChar;
        std::cin >> cmdChar;
        field.clearReady();

        switch (cmdChar)
        {
        case 'W':
            center.y--;
            break;
        case 'A':
            center.x--;
            break;
        case 'S':
            center.y++;
            break;
        case 'D':
            center.x++;
            break;
        case 'F':
            cmd = Command{CommandType::SelectPosition, center};
            flag = !flag;
        default:
            break;
        }
    }

    return cmd;
}

