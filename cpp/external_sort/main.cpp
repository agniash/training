#include "file_generator.h"
#include "external_sort.h"

int main()
{
    std::string filename = "sample";

    TFileGenerator fg;
    fg.Generate(filename, 105, 10);

    TExternalSort es(106);
    es.Sort(filename, filename + "_sorted");

    return 0;
}

