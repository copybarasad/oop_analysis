#pragma once

enum class KeyCode {
    Unknown,

    Up,
    Down,
    Left,
    Right,

    Num0,
    Num1,
    Num2,
    Num3,
    Num4,
    Num5,
    Num6,
    Num7,
    Num8,
    Num9,

    A, B, C, D, E, F, G, H, I, J,
    K, L, M, N, O, P, Q, R, S, T,
    U, V, W, X, Y, Z,

    Space,
    Enter,
    ESC,
    LShift,
    RShift,
    Ctrl,
    Alt
};


enum class SystemEventType {
    Unknown,
    WindowClosed
};


enum class InputDevice {
    Keyboard,
    Mouse,
    System
};


enum class MouseButton {
    Unknown,
    Left,
    Right,
    Middle
};


struct InputEvent
{
    InputDevice device;

    KeyCode key;

    SystemEventType systemEvent;

    MouseButton mouseButton;
    int x;
    int y;

    bool pressed;
};