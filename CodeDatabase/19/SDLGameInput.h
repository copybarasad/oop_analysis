#pragma once

#include <map>
#include <functional>
#include <SDL.h>
#include <string>
#include <set>

#include "core/input/IGameInput.h"


class SDLGameInput : public IGameInput {
private:
    std::map<SDL_Scancode, Action> key_map_;
    std::map<int, Action> mouse_map_;
    std::map<int, Action> wheel_map_;

    ActionCallback callback_;
    StopCallback stop_callback_;
    MouseClickCallback mouse_callback_;

    int mouse_x_ = 0;
    int mouse_y_ = 0;

    bool try_bind_inputs(const std::map<Action, std::string> &inputs) {
        if (inputs.empty()) return false;

        std::map<SDL_Scancode, Action> temp_keys;
        std::map<int, Action> temp_mouse;
        std::map<int, Action> temp_wheel;

        std::set<SDL_Scancode> used_scancodes;
        std::set<int> used_mouse_buttons;
        std::set<int> used_wheel_dirs;

        for (const auto &[action, key_name]: inputs) {
            int wheel_dir = 0;
            if (key_name == "mouse_wheel_up") wheel_dir = 1;
            else if (key_name == "mouse_wheel_down") wheel_dir = -1;

            if (wheel_dir != 0) {
                if (used_wheel_dirs.contains(wheel_dir)) return false;
                used_wheel_dirs.insert(wheel_dir);
                temp_wheel[wheel_dir] = action;
                continue;
            }

            int mouse_btn = -1;
            if (key_name == "mouse_left") mouse_btn = SDL_BUTTON_LEFT;
            else if (key_name == "mouse_right") mouse_btn = SDL_BUTTON_RIGHT;
            else if (key_name == "mouse_middle") mouse_btn = SDL_BUTTON_MIDDLE;

            if (mouse_btn != -1) {
                if (used_mouse_buttons.contains(mouse_btn)) return false;
                used_mouse_buttons.insert(mouse_btn);
                temp_mouse[mouse_btn] = action;
                continue;
            }

            SDL_Scancode code = SDL_GetScancodeFromName(key_name.c_str());
            if (code != SDL_SCANCODE_UNKNOWN) {
                if (used_scancodes.contains(code)) return false;
                used_scancodes.insert(code);
                temp_keys[code] = action;
                continue;
            }
            return false;
        }

        key_map_ = std::move(temp_keys);
        mouse_map_ = std::move(temp_mouse);
        wheel_map_ = std::move(temp_wheel);
        return true;
    }

public:
    SDLGameInput() = default;

    bool init(const std::map<Action, std::string> &defaults, const std::map<Action, std::string> &actions,
              ActionCallback callback, MouseClickCallback mouse_callback, StopCallback stopCallback) override {
        callback_ = std::move(callback);
        stop_callback_ = std::move(stopCallback);
        mouse_callback_ = std::move(mouse_callback);

        if (try_bind_inputs(actions)) return true;
        if (try_bind_inputs(defaults)) return true;

        return false;
    }

    void tick() override {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYDOWN: {
                    if (event.key.repeat != 0) return;

                    auto it = key_map_.find(event.key.keysym.scancode);
                    if (it != key_map_.end()) {
                        if (callback_) callback_(it->second);
                    }
                    break;
                }

                case SDL_MOUSEBUTTONDOWN: {
                    if (mouse_callback_) mouse_callback_();
                    auto it = mouse_map_.find(event.button.button);
                    if (it != mouse_map_.end()) {
                        if (callback_) {
                            callback_(it->second);
                        }
                    }
                    break;
                }

                case SDL_MOUSEWHEEL: {
                    if (event.wheel.y > 0) {
                        auto it = wheel_map_.find(1);
                        if (it != wheel_map_.end() && callback_) callback_(it->second);
                    } else if (event.wheel.y < 0) {
                        auto it = wheel_map_.find(-1);
                        if (it != wheel_map_.end() && callback_) callback_(it->second);
                    }
                    break;
                }

                case SDL_MOUSEMOTION:
                    mouse_x_ = event.motion.x;
                    mouse_y_ = event.motion.y;
                    break;
                case SDL_QUIT:
                    if (stop_callback_)
                        stop_callback_();
                    break;

                default:
                    break;
            }
        }
    }

    void get_mouse_pos(int &x, int &y) const override {
        x = mouse_x_;
        y = mouse_y_;
    }
};
