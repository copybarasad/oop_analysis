#ifndef JSONFILE_H
#define JSONFILE_H

#include <fstream>
#include <string>
#include <QByteArray>
#include <QJsonDocument>
#include "saveloadexceptions.h"

class JsonOutputFile
{
public:
    static JsonOutputFile open(const std::string& path)
    {
        std::ofstream out(path);
        if (!out.is_open())
            throw FileNotFoundException(path);

        return JsonOutputFile(std::move(out), path);
    }

    JsonOutputFile& operator<<(const QByteArray& arr)
    {
        m_out.write(arr.constData(), arr.size());
        if (!m_out.good())
            throw SaveException("Error writing to file: " + m_path);
        return *this;
    }

    JsonOutputFile& operator<<(const QJsonDocument& doc)
    {
        QByteArray json = doc.toJson(QJsonDocument::Indented);
        return (*this << json);
    }

    JsonOutputFile(const JsonOutputFile&) = delete;
    JsonOutputFile& operator=(const JsonOutputFile&) = delete;

private:
    std::ofstream m_out;
    std::string m_path;

    JsonOutputFile(std::ofstream&& out, const std::string& path)
        : m_out(std::move(out)), m_path(path) {}
};


class JsonInputFile
{
public:
    static JsonInputFile open(const std::string& path)
    {
        std::ifstream in(path);
        if (!in.is_open())
            throw FileNotFoundException(path);

        return JsonInputFile(std::move(in), path);
    }

    JsonInputFile& operator>>(QByteArray& out)
    {
        std::string buffer((std::istreambuf_iterator<char>(m_in)), std::istreambuf_iterator<char>());

        if (!m_in.good())
            throw SaveException("Error reading file: " + m_path);

        out = QByteArray::fromStdString(buffer);
        return *this;
    }

    JsonInputFile& operator>>(QJsonDocument& doc)
    {
        QByteArray bytes;
        *this >> bytes;

        QJsonParseError err;
        doc = QJsonDocument::fromJson(bytes, &err);

        if (err.error != QJsonParseError::NoError)
            throw SaveException("JSON parse error in file:" + m_path);
        return *this;
    }

    JsonInputFile(const JsonInputFile&) = delete;
    JsonInputFile& operator=(const JsonInputFile&) = delete;

private:
    std::ifstream m_in;
    std::string   m_path;

    JsonInputFile(std::ifstream&& in, const std::string& path)
        : m_in(std::move(in)), m_path(path) {}
};

#endif // JSONFILE_H
