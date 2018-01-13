#include "file_generator.h"

#include <fstream>
#include <algorithm>
#include <cstdlib>

namespace {
    std::string GenerateRecord(size_t size) {
        std::string s;
        s.resize(size);
        std::generate(s.begin(), s.end(), [] {
            return 'a' + std::rand() % 26;
        });
        return s;
    }
} // unnamed namespace

TFileGenerator::TFileGenerator(char lineDelimiter)
    : LineDelimiter(lineDelimiter)
{}

void TFileGenerator::Generate(const std::string& filename, size_t records, size_t size) {
    std::ofstream file(filename, std::ios::binary);
    for (size_t record = 0; record < records; ++record) {
        if (record) {
            file << LineDelimiter;
        }
        file << GenerateRecord(size);
    }
}