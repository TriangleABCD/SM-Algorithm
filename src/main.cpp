#include <omp.h>
#include "sm4.h"
#include "sm3.h"

int main(int argc, char** argv) {
    SM3 sm3("abc");
    auto hash = sm3.getHash();
    int k = 0;
    for(auto& c: hash) {
        cout << c;
        ++k;
        if(k != 0 && k % 32 == 0) {
            std::cout << "\n";
        }
    }
    std::cout << "#\n";
    return 0;
}
