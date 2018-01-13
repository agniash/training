#include "external_sort.h"

#include <fstream>
#include <algorithm>
#include <queue>
#include <functional>
#include <cstdlib>

namespace {
    const size_t RANDOM_SUFFIX_SIZE = 6;

    std::string GenerateTmpFilename(const std::string& baseName, size_t counter) {
        std::string suffix;
        suffix.resize(RANDOM_SUFFIX_SIZE);
        std::generate(suffix.begin(), suffix.end(), [] {
            return 'a' + std::rand() % 26;
        });
        suffix += '.' + std::to_string(counter);
        return baseName + suffix;
    }

    class TBufferBase {
    public:
        TBufferBase(const std::string& filename, std::ios_base::openmode mode, size_t maxSize, char lineDelimiter)
            : Stream(filename, mode)
            , MaxSize(maxSize)
            , Size(0)
            , LineDelimiter(lineDelimiter)
        {}

        bool AddRecord(const std::string& record) {
            Size += record.size();
            Records.push_back(record);
            return Size < MaxSize;
        }

        void Clear() {
            Records.clear();
            Size = 0;
        }

        using TRecords = std::vector<std::string>;

        TRecords Records;
        std::fstream Stream;
        const size_t MaxSize;
        size_t Size;
        const char LineDelimiter;
    };

    class TInputBuffer : public TBufferBase {
    public:
        TInputBuffer(const std::string& filename, size_t maxSize, char lineDelimiter)
            : TBufferBase(filename, std::ios::in | std::ios::binary, maxSize, lineDelimiter)
        {
            Refill();
        }

        const std::string& GetRecord() const {
            return *CurrentRecord;
        }

        bool NextRecord() {
            if (++CurrentRecord == Records.end()) {
                return Refill();
            }
            return true;
        }

    private:
        bool Refill() {
            Clear();
            std::string record;
            while (std::getline(Stream, record, LineDelimiter) && AddRecord(record)) {}
            CurrentRecord = Records.begin();
            return (bool)Stream;
        }

        TRecords::iterator CurrentRecord;
    };

    class TOutputBuffer : public TBufferBase {
    public:
        TOutputBuffer(const std::string& filename, size_t maxSize, char lineDelimiter)
            : TBufferBase(filename, std::ios::out | std::ios::binary, maxSize, lineDelimiter)
        {}

        bool AddRecord(const std::string& record) {
            if (TBufferBase::AddRecord(record)) {
                return true;
            } else {
                Flush();
                return false;
            }
        }

    private:
        void Flush() {
            static bool firstRecord = true;
            for (const auto& record : Records) {
                if (firstRecord) {
                    firstRecord = false;
                } else {
                    Stream << LineDelimiter;
                }
                Stream << record;
            }
            Clear();
        }

        size_t Size;
    };

    struct TInputBufferCompare {
        bool operator()(TInputBuffer* lhs, TInputBuffer* rhs) const {
            return lhs->GetRecord() > rhs->GetRecord();
        }
    };
} // unnamed namespace

TExternalSort::TExternalSort(size_t maxMemorySize, char lineDelimiter)
    : MaxMemorySize(maxMemorySize)
    , LineDelimiter(lineDelimiter)
{}

void TExternalSort::Sort(const std::string& inputFilename, const std::string& outputFilename) {
    InMemorySort(inputFilename);
    Merge(outputFilename);
    CleanUp();
}

void TExternalSort::InMemorySort(const std::string& filename) {
    std::ifstream input(filename, std::ios::binary);
    std::string record;
    for (size_t pass = 0; input; ++pass) {
        std::vector<std::string> records;
        size_t memoryUsed = 0;
        while (std::getline(input, record, LineDelimiter)) {
            memoryUsed += record.size();
            records.push_back(std::move(record));
            if (memoryUsed >= MaxMemorySize) {
                break;
            }
        }
        if (records.empty()) {
            break;
        }
        std::sort(records.begin(), records.end());
        TmpFilenames.push_back(GenerateTmpFilename(filename, pass));
        std::ofstream output(TmpFilenames.back(), std::ios::binary);
        for (size_t i = 0; i < records.size(); ++i) {
            if (i) {
                output << LineDelimiter;
            }
            output << records[i];
        }
    }
}

void TExternalSort::Merge(const std::string& filename) {
    std::vector<TInputBuffer> inputBuffers;
    const size_t maxBufferSize = MaxMemorySize / (TmpFilenames.size() + 1);
    for (const auto& filename : TmpFilenames) {
        inputBuffers.emplace_back(filename, maxBufferSize, LineDelimiter);
    }

    std::priority_queue<TInputBuffer*, std::vector<TInputBuffer*>, TInputBufferCompare> heap;
    for (auto& buffer : inputBuffers) {
        heap.push(&buffer);
    }

    TOutputBuffer outputBuffer(filename, maxBufferSize, LineDelimiter);
    while (!heap.empty()) {
        TInputBuffer* buffer = heap.top();
        outputBuffer.AddRecord(buffer->GetRecord());
        heap.pop();
        if (buffer->NextRecord()) {
            heap.push(buffer);
        }
    }
}

void TExternalSort::CleanUp() {
    for (const auto& filename : TmpFilenames) {
        std::remove(filename.c_str());
    }
}