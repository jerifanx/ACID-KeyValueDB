#ifndef _TRANSACTION_H_
#define _TRANSACTION_H_
#include <string>
#include <vector>
#include <stdexcept>

class KeyValueStore;

class Transaction {
    private:
        KeyValueStore& store;
        std::vector<std::pair<std::string, std::string>> pendingPuts;
        std::vector<std::string> pendingDeletes;
        bool isActive;

    public:
        explicit Transaction(KeyValueStore& store);
        
        void put(const std::string& key, const std::string& value);
        
        void remove(const std::string& key);
        
        bool commit();
        
        void rollback();
        
        bool isTransactionActive() const;
};

#endif
