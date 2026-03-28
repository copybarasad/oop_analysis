#include "xdg.hpp"

XDG &XDG::inConfigHome() {
    m_Base = ".config";
    return *this;
}

XDG &XDG::inDataHome() {
    m_Base = ".local/share";
    return *this;
}

XDG &XDG::withAppName(std::string name) {
    m_AppName = name;
    return *this;
}

XDG &XDG::withFileName(std::string name) {
    m_FileName = name;
    return *this;
}

fs::path XDG::prefix() {
    char *home_env = std::getenv("HOME");
    if (!home_env) {
        return fs::current_path();
    }
    fs::path home_dir(home_env);
    return home_dir;
}

fs::path XDG::build() {
    fs::path basedir = prefix() / m_Base / m_AppName;

    if (!fs::exists(basedir) && !fs::create_directories(basedir)) {
        throw std::runtime_error("failed to create directory: " +
                                 basedir.string());
    }

    if (!fs::is_directory(basedir)) {
        throw std::runtime_error("path exists but is not a directory: " +
                                 basedir.string());
    }

    return basedir / m_FileName;
}
