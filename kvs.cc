#include "kvs.h"
#include "transaction.h"
#include <iostream>

KeyValueStore::KeyValueStore() {
    logFile.open("kv_store.log", std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Log file cannot be opened, durability at risk." << std::endl;
    }
}

KeyValueStore::~KeyValueStore() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void KeyValueStore::addToLog(const std::string& operation) {
    if (logFile.is_open()) {
        logFile << operation << std::endl;
        logFile.flush();
    } 
}

bool KeyValueStore::put(const std::string& key, const std::string& value) {
    std::lock_guard<std::mutex> lock(m);
    data[key] = value;
    addToLog("PUT " + key + " " + value);
    return true;
}

bool KeyValueStore::get(const std::string& key, std::string& value) {
    std::lock_guard<std::mutex> lock(m);
    auto it = data.find(key);
    if (it != data.end()) {
        value = it->second;
        return true;
    }
    return false;
}

bool KeyValueStore::remove(const std::string& key) {
    std::lock_guard<std::mutex> lock(m);
    auto it = data.find(key);
    if (it != data.end()) {
        data.erase(it);
        addToLog("DELETE " + key);
        return true;
    }
    return false;
}

std::shared_ptr<Transaction> KeyValueStore::beginTransaction() {
    return std::make_shared<Transaction>(*this);
}

bool KeyValueStore::applyTransaction(const std::vector<std::pair<std::string, std::string>>& puts,
                                    const std::vector<std::string>& deletes) {
    std::lock_guard<std::mutex> lock(m);
    
    addToLog("Begin transaction");
    
    for (const auto& pair : puts) {
        data[pair.first] = pair.second;
        addToLog("PUT " + pair.first + " " + pair.second);
    }
    
    for (const auto& key : deletes) {
        data.erase(key);
        addToLog("DELETE " + key);
    }
    
    addToLog("Commit transaction");
    
    return true;
}

void KeyValueStore::printAll() {
    std::lock_guard<std::mutex> lock(m);
    std::cout << "KeyValueStore contents:" << std::endl;
    for (const auto& pair:data) {
        std::cout << pair.first << " => " << pair.second << std::endl;
    }
    std::cout << std::endl;
}