#include "kvs.h"
#include "transaction.h"
#include <iostream>
#include <thread>
#include <chrono>

// Simple test
void testKeyValueStore() {
    KeyValueStore store;
    
    // Basic operations
    store.put("name", "Jeri");
    store.put("age", "20");
    
    std::string value;
    if (store.get("name", value)) {
        std::cout << "Retrieved: name = " << value << std::endl;
    }
    
    // Test transaction
    auto transaction = store.beginTransaction();
    transaction->put("city", "Waterloo");
    transaction->put("country", "Canada");
    transaction->remove("age");
    
    // Before commit
    std::cout << "Before commit:" << std::endl;
    store.printAll();
    
    // Commit transaction
    transaction->commit();
    
    // After commit
    std::cout << "After commit:" << std::endl;
    store.printAll();
}

// Test concurrent access
void concurrencyTest() {
    KeyValueStore store;
    
    // Initialize with some data
    store.put("counter", "0");
    
    // Function to increment counter
    auto incrementCounter = [&store](int id, int iterations) {
        for (int i = 0; i < iterations; i++) {
            std::string value;
            
            // Begin a transaction
            auto tx = store.beginTransaction();
            
            // Get current value
            if (store.get("counter", value)) {
                int count = std::stoi(value);
                count++;
                
                // Simulate some processing time
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                
                // Update the value
                tx->put("counter", std::to_string(count));
                tx->commit();
                
                std::cout << "Thread " << id << " incremented counter to " << count << std::endl;
            }
        }
    };
    
    // Create multiple threads
    std::thread t1(incrementCounter, 1, 5);
    std::thread t2(incrementCounter, 2, 5);
    
    t1.join();
    t2.join();
    
    std::string finalValue;
    store.get("counter", finalValue);
    std::cout << "Final counter value: " << finalValue << std::endl;
}