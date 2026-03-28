#pragma once

#include <stdexcept>

namespace game {
    struct SaveError: std::runtime_error {
        using std::runtime_error::runtime_error;
    };

    struct LoadError: std::runtime_error {
        using std::runtime_error::runtime_error;
    };
}