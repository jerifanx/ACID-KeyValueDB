#include "kvs.h"
#include "transaction.h"
#include "tests.h"
#include <iostream>

int main() {
    std::cout << "Running basic test: " << std::endl;
    testKeyValueStore();
    
    std::cout << "Running concurrency test: " << std::endl;
    concurrencyTest();
    
    return 0;
}
