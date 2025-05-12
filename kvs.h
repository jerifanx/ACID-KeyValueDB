#ifndef _KVS_H_
#define _KVS_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <mutex>
#include <memory>

class Transaction;

class KeyValueStore {
    private:
        std::unordered_map<std::string, std::string> data;
        std::mutex m;
        std::ofstream logFile;
        void addToLog(const std::string& operation);
    
    public:
        KeyValueStore();
        ~KeyValueStore();

        bool put(const std::string& key, const std::string& value);

        bool get(const std::string& key, std::string& value);
        
        bool remove(const std::string& key);

        std::shared_ptr<Transaction> beginTransaction();
        bool applyTransaction(const std::vector<std::pair<std::string, std::string>>& puts, const std::vector<std::string>& deletes);

        void printAll();
};

#endif