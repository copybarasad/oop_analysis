#include "InputController.hpp"

InputController::InputController() {
    for(int i = 0; i < 6; i++)
        last_press[i] = std::chrono::steady_clock::now() - std::chrono::hours(1);
    bt_thread = std::thread(&InputController::bluetooth_thread, this, std::string(MAC_ADR));
}

InputController::~InputController() {
    running = false;
    if(sock >= 0)
        close(sock);

    if(bt_thread.joinable())
        bt_thread.join();
}


char InputController::getInputChar() {
    if (!running) return 0;
    if(bt_event_ready) {
        bt_event_ready = false;
        return bt_last_key;
    }
    int ch = getch();
    std::this_thread::sleep_for(std::chrono::milliseconds(15));
    if(ch != ERR) {
        return static_cast<char>(ch);
    }
    return 0;
}


void InputController::bluetooth_thread(const std::string& mac_addr) {
    sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    if(sock < 0) {
        std::cerr << "Bluetooth socket error\n";
        return;
    }
    struct sockaddr_rc addr = {0};
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = 1;
    str2ba(mac_addr.c_str(), &addr.rc_bdaddr);
    if(connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "Bluetooth connection failed\n";
        close(sock);
        sock = -1;
        return;
    }
    char buffer[256];
    while(running) {
        memset(buffer, 0, sizeof(buffer));
        int len = read(sock, buffer, sizeof(buffer)-1);
        if(len > 0) {
            for(int i = 1; i <= 6; i++) {
                std::string key = "PIN" + std::to_string(i);
                if(strstr(buffer, key.c_str())) {
                    auto now = std::chrono::steady_clock::now();
                    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_press[i-1]).count();
                    if(ms >= DEBOUNCE_MS) {
                        last_press[i-1] = now;
                        bt_last_key = BT_KEYS[i-1];
                        bt_event_ready = true;
                    }
                }
            }
        }
        else if(len == 0) {
            break;
        }
        usleep(50000);
    }
    if(sock >= 0)
        close(sock);
    sock = -1;
}


void InputController::stop() {
    running = false;
}

void InputController::join() {
    if (bt_thread.joinable())
        bt_thread.join();
}
