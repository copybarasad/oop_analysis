#include "save.hpp"

Save::Save() {}

Save::Save(bytedump bytes) {
    m_Bytes = bytes;
    validateSignature();
}

Save::Save(fs::path src) {
    std::ifstream file(src, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + src.string());
    }

    std::vector<char> buffer((std::istreambuf_iterator<char>(file)),
                             std::istreambuf_iterator<char>());

    m_Bytes.clear();
    m_Bytes.reserve(buffer.size());
    for (char c : buffer) {
        m_Bytes.push_back(static_cast<std::byte>(c));
    }

    validateSignature();
}

void Save::write(bytedump bytes) {
    if (m_Bytes.size() == 0) {
        insertSignature();
    } else {
        insertBoundary();
    }

    m_Bytes.insert(m_Bytes.end(), bytes.begin(), bytes.end());
}

std::vector<bytedump> Save::parts() {
    std::vector<bytedump> chunks;
    auto start = m_Bytes.begin() + 5;
    bytedump b = boundary();

    while (true) {
        auto it = std::search(start, m_Bytes.end(), b.begin(), b.end());
        chunks.emplace_back(start, it);

        if (it == m_Bytes.end()) {
            break;
        }

        start = it + b.size();
    }

    return chunks;
}

bytedump Save::bytes() { return m_Bytes; }

bytedump Save::boundary() {
    return {
        static_cast<std::byte>(255),
        static_cast<std::byte>(255),
        static_cast<std::byte>(255),
        static_cast<std::byte>(255),
    };
}

bytedump Save::signature() {
    bytedump bytes;

    bytes.push_back(static_cast<std::byte>(20));
    bytes.push_back(static_cast<std::byte>('k'));
    bytes.push_back(static_cast<std::byte>('y'));
    bytes.push_back(static_cast<std::byte>('o'));
    bytes.push_back(static_cast<std::byte>('\n'));

    return bytes;
}

void Save::insertBoundary() {
    bytedump b = boundary();
    m_Bytes.insert(m_Bytes.end(), b.begin(), b.end());
}

void Save::insertSignature() {
    bytedump s = signature();
    m_Bytes.insert(m_Bytes.end(), s.begin(), s.end());
}

void Save::validateSignature() {
    if (m_Bytes[0] != static_cast<std::byte>(20) ||
        m_Bytes[1] != static_cast<std::byte>('k') ||
        m_Bytes[2] != static_cast<std::byte>('y') ||
        m_Bytes[3] != static_cast<std::byte>('o') ||
        m_Bytes[4] != static_cast<std::byte>('\n')) {
        throw InvalidSignatureError();
    }
}

void Save::save(fs::path dst) {
    std::ofstream file(dst, std::ios::binary);
    if (!file) {
        throw std::runtime_error("failed to open: " + dst.string());
    }

    file.write(reinterpret_cast<const char *>(m_Bytes.data()), m_Bytes.size());

    if (!file) {
        throw std::runtime_error("failed to write save: " + dst.string());
    }
}
