#include "../include/io/console_command_reader.h"

void ConsoleCommandReader::ClearBadInputLine() {
    if (std::cin.fail()) {
        std::cin.clear();
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

char ConsoleCommandReader::ReadChar() {
    char ch{};
    std::cin >> ch;
    ClearBadInputLine();
    return ch;
}

int ConsoleCommandReader::ReadInt() {
    int v{};
    std::cin >> v;
    if (std::cin.fail()) {
        ClearBadInputLine();
        return std::numeric_limits<int>::min();
    }
    ClearBadInputLine();
    return v;
}

Command ConsoleCommandReader::ReadStartCommand() {
    std::cout << "Загрузка сохранения? (Y/N): ";
    char ans = ReadChar();
    if (ans == 'Y' || ans == 'y') return Command::StartLoadGame();
    return Command::StartNewGame();
}

Command ConsoleCommandReader::ReadMainCommand(bool inventoryShown) {
    std::cout << "Введите команду (W/A/S/D/C/F/I/M/K=Save/L=Load/Q): ";
    char input = ReadChar();

    if (inventoryShown && input >= '1' && input <= '9') {
        return Command::EquipSpellSlot((input - '1'));
    }

    switch (input) {
        case 'q':
        case 'Q':
            return Command::Quit();

        case 'w':
        case 'W':
            return Command::Move(Direction::Up);
        case 's':
        case 'S':
            return Command::Move(Direction::Down);
        case 'a':
        case 'A':
            return Command::Move(Direction::Left);
        case 'd':
        case 'D':
            return Command::Move(Direction::Right);

        case 'c':
        case 'C':
            return Command::ChangeWeapon();

        case 'i':
        case 'I':
            return Command::ToggleInventory();

        case 'm':
        case 'M':
            return Command::OpenShop();

        case 'f':
        case 'F':
            return Command::CastSpell();

        case 'k':
        case 'K': {
            std::cout << "Имя сохранения (без пробелов): ";
            std::string name;
            std::cin >> name;
            ClearBadInputLine();
            if (name.empty()) return Command::Cancel();
            return Command::SaveGame(name);
        }

        case 'l':
        case 'L':
            return Command::LoadPrompt();

        default:
            return Command::None();
    }
}

Command ConsoleCommandReader::ReadLoadCommand(const std::vector<std::string>& saves) {
    if (saves.empty()) {
        return Command::Cancel();
    }

    std::cout << "Доступные сохранения:\n";
    for (size_t i = 0; i < saves.size(); ++i) {
        std::cout << (i + 1) << ") " << saves[i] << "\n";
    }
    std::cout << "Введите номер или 0 для отмены: ";

    int ch = ReadInt();
    if (ch == std::numeric_limits<int>::min()) {
        std::cout << "Неверный ввод!\n";
        return Command::Cancel();
    }

    if (ch <= 0 || ch > static_cast<int>(saves.size())) {
        return Command::Cancel();
    }

    return Command::LoadGame(saves[ch - 1]);
}

Command ConsoleCommandReader::ReadShopCommand() {
    std::cout << "Введите номер товара для покупки (0 - выход): ";

    int choice = ReadInt();
    if (choice == std::numeric_limits<int>::min()) {
        std::cout << "Неверный ввод! Введите число." << std::endl;
        return Command::Cancel();
    }

    if (choice == 0) {
        return Command::ShopExit();
    }

    if (choice < 0) {
        return Command::Cancel();
    }

    return Command::ShopBuy(choice - 1);
}

Command ConsoleCommandReader::ReadTargetSelectionCommand() {
    std::cout << "Введите номер цели: ";
    int choice = ReadInt();

    if (choice == std::numeric_limits<int>::min()) {
        return Command::Cancel();
    }

    if (choice <= 0) {
        return Command::Cancel();
    }

    return Command::SelectTarget(choice);
}

Command ConsoleCommandReader::ReadDirectionSelectionCommand() {
    std::cout << "Выберите направление атаки (W - вверх, S - вниз, A - влево, D - вправо): ";
    char dir_input = ReadChar();

    switch (dir_input) {
        case 'w':
        case 'W':
            return Command::SelectDirection(Direction::Up);
        case 's':
        case 'S':
            return Command::SelectDirection(Direction::Down);
        case 'a':
        case 'A':
            return Command::SelectDirection(Direction::Left);
        case 'd':
        case 'D':
            return Command::SelectDirection(Direction::Right);
        default:
            return Command::Cancel();
    }
}

Command ConsoleCommandReader::ReadConfirmAttackCommand() {
    std::cout << "Подтвердите атаку (E) или отмените (любая другая клавиша): ";
    char confirm = ReadChar();
    return Command::Confirm(confirm == 'e' || confirm == 'E');
}

Command ConsoleCommandReader::ReadUpgradeChoiceCommand() {
    std::cout << "Ваш выбор (1-4): ";
    int choice = ReadInt();
    if (choice == std::numeric_limits<int>::min()) {
        return Command::Cancel();
    }
    return Command::UpgradeSelect(choice);
}

void ConsoleCommandReader::WaitForEnter() {
    std::cout << "Нажмите Enter для продолжения...";
    std::cin.get();
}
