#pragma once
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <ncurses.h>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <string>
#include <thread>
#include <atomic>
#include <chrono>

#define MAC_ADR "f0:24:f9:0d:d4:82"
#define DEBOUNCE_MS 120

class InputController {
private:
    std::atomic<bool> running{true};
    std::atomic<bool> bt_event_ready{false};
    char bt_last_key = 0;
    int sock = -1;
    std::thread bt_thread;
    std::chrono::steady_clock::time_point last_press[6];
    static constexpr char BT_KEYS[6] = { 's', 'd', 'w', 'a', 'q', 'e' };

public:
    InputController();
    ~InputController();
    void stop();
    void join();
    char getInputChar();
private:
    void bluetooth_thread(const std::string& mac_addr);
};
