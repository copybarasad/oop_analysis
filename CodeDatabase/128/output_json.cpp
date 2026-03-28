#include "output_json.hpp"
#include "errors.hpp"

JsonWriter::JsonWriter() = default;

JsonWriter::JsonWriter(const std::string& path) {
    Open(path);
}

void JsonWriter::Open(const std::string& path) {
    file_path_ = path;
    output_stream_.open(path);

    if (!output_stream_.is_open())
        throw SaveError("Cannot open file for writing: " + path);
}

void JsonWriter::Write(const nlohmann::json& j) {
    try {
        output_stream_ << j.dump(4);  
    }
    catch (const std::exception& e) {
        throw SaveError(e.what());
    }
    catch (...) {
        throw SaveError("Unknown write error");
    }
}

JsonWriter::~JsonWriter() {
    if (output_stream_.is_open())
        output_stream_.close();
}
