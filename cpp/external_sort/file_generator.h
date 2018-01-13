#pragma once

#include <string>

class TFileGenerator {
public:
    TFileGenerator(char lineDelimiter = '\n');
    void Generate(const std::string& filename, size_t records, size_t size);

private:
    const char LineDelimiter;
};