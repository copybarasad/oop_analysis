#pragma once
#include "Command.h"
#include <string>
#include <iostream>
#include <thread>
#include <chrono>

// Секция для Windows
#ifdef _WIN32
#include <windows.h>
#else
// Секция для Linux
#include <fcntl.h> 
#include <errno.h>
#include <termios.h> 
#include <unistd.h> 
#endif

class SerialInput {
private:
#ifdef _WIN32
    HANDLE hSerial;
#else
    int serialPort;
#endif
    bool connected = false;

public:
    SerialInput() {
#ifdef _WIN32

        // В строке C++ это выглядит как "\\\\.\\COM17"
        std::string portName = "\\\\.\\COM17"; 

        hSerial = CreateFile(portName.c_str(),
                             GENERIC_READ | GENERIC_WRITE,
                             0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

        if (hSerial == INVALID_HANDLE_VALUE) {
            DWORD errorCode = GetLastError();
            std::cerr << "[Serial] Ошибка: Не удалось открыть порт " << portName << std::endl;
            std::cerr << "[Serial] Код ошибки Windows: " << errorCode << std::endl;
            if (errorCode == 2) std::cerr << " (Скорее всего, неверное имя порта или нужен префикс \\\\.\\)" << std::endl;
            if (errorCode == 5) std::cerr << " (Доступ запрещен. Закройте монитор порта в Arduino IDE!)" << std::endl;
        } else {
            DCB dcbSerialParams = {0};
            dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
            
            if (!GetCommState(hSerial, &dcbSerialParams)) {
                 std::cerr << "[Serial] Ошибка получения состояния порта" << std::endl;
            }
            
            dcbSerialParams.BaudRate = CBR_9600;
            dcbSerialParams.ByteSize = 8;
            dcbSerialParams.StopBits = ONESTOPBIT;
            dcbSerialParams.Parity = NOPARITY;
            
            if (!SetCommState(hSerial, &dcbSerialParams)) {
                 std::cerr << "[Serial] Ошибка установки состояния порта" << std::endl;
            }
            
            COMMTIMEOUTS timeouts = {0};
            timeouts.ReadIntervalTimeout = 1000;
            timeouts.ReadTotalTimeoutConstant = 1000;
            timeouts.ReadTotalTimeoutMultiplier = 10;
            SetCommTimeouts(hSerial, &timeouts);
            
            connected = true;
            std::cout << "[Serial] Порт COM17 успешно открыт!" << std::endl;
        }
#else
        std::string portName = "/dev/ttyACM0"; 
        serialPort = open(portName.c_str(), O_RDWR);

        if (serialPort < 0) {
            std::cerr << "[Serial] Ошибка: Не удалось открыть порт " << portName << std::endl;
        } else {
            struct termios tty;
            if(tcgetattr(serialPort, &tty) != 0) return;

            cfsetospeed(&tty, B9600);
            cfsetispeed(&tty, B9600);

            tty.c_cflag &= ~PARENB; 
            tty.c_cflag &= ~CSTOPB; 
            tty.c_cflag &= ~CSIZE;
            tty.c_cflag |= CS8;     
            
            tty.c_cc[VTIME] = 10;   
            tty.c_cc[VMIN] = 0;

            tcsetattr(serialPort, TCSANOW, &tty);
            connected = true;
             std::cout << "[Serial] Порт Linux успешно открыт!" << std::endl;
        }
#endif
    }
    ~SerialInput() {
        if (connected) {
#ifdef _WIN32
            CloseHandle(hSerial);
#else
            close(serialPort);
#endif
        }
    }

    GameCommand getCommand() {
        if (!connected) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            return {CommandType::None};
        }

        char readBuffer[1] = {0};
        int bytesRead = 0;

#ifdef _WIN32
        DWORD dwBytesRead = 0;
        if (ReadFile(hSerial, readBuffer, 1, &dwBytesRead, NULL)) {
            bytesRead = dwBytesRead;
        }
#else
        bytesRead = read(serialPort, readBuffer, 1);
#endif

        GameCommand cmd;
        cmd.type = CommandType::None;

        if (bytesRead > 0) {
            char input = readBuffer[0];

            if (input == 'w') { cmd.type = CommandType::Move; cmd.x = 0; cmd.y = -1; }
            else if (input == 's') { cmd.type = CommandType::Move; cmd.x = 0; cmd.y = 1; }
            else if (input == 'a') { cmd.type = CommandType::Move; cmd.x = -1; cmd.y = 0; }
            else if (input == 'd') { cmd.type = CommandType::Move; cmd.x = 1; cmd.y = 0; }
            else if (input == 'f') { cmd.type = CommandType::Attack; }
            else if (input == 'q') { cmd.type = CommandType::SwitchMode; }
            
            if (cmd.type != CommandType::None) {
                 std::cout << "[ARDUINO]: Сигнал принят: " << input << std::endl;
            }
        }
        
        return cmd;
    }
};