#include "transaction.h"
#include "kvs.h"

Transaction::Transaction(KeyValueStore& store) : store(store), isActive(true) {}

void Transaction::put(const std::string& key, const std::string& value) {
    if (!isActive) {
        throw std::runtime_error("Transaction is no longer active");
    }
    pendingPuts.push_back({key, value});
}

void Transaction::remove(const std::string& key) {
    if (!isActive) {
        throw std::runtime_error("Transaction is no longer active");
    }
    pendingDeletes.push_back(key);
}

bool Transaction::commit() {
    if (!isActive) {
        return false;
    }
    
    bool success = store.applyTransaction(pendingPuts, pendingDeletes);
    isActive = false;
    return success;
}

void Transaction::rollback() {
    pendingPuts.clear();
    pendingDeletes.clear();
    isActive = false;
}

bool Transaction::isTransactionActive() const {
    return isActive;
}