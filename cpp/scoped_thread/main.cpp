#include "scoped_thread.h"

#include <iostream>
#include <functional>

void f(int& local) {
    local = 10;
}

void g() {
    int local = 1;
    std::cout << "before: " << local << std::endl;
    {
        TScopedThread t(std::thread(f, std::ref(local)));
    }
    std::cout << "after: " << local << std::endl;
}

int main(int argc, char** argv) {
    g();
    return 0;
}
