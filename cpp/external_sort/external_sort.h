#pragma once

#include <string>
#include <vector>

class TExternalSort {
public:
    TExternalSort(size_t maxMemorySize, char lineDelimiter = '\n');
    void Sort(const std::string& inputFilename, const std::string& outputFilename);

private:
    void InMemorySort(const std::string& filename);
    void Merge(const std::string& filename);
    void CleanUp();

    const size_t MaxMemorySize;
    const char LineDelimiter;
    std::vector<std::string> TmpFilenames;
};