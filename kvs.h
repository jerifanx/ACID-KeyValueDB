#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <mutex>
#include <memory>
#include <chrono>
#include <thread>
#include <atomic>
#include "transaction.h"

using namespace std;

class Transaction;
class KeyValueStore {
    private:
        unordered_map<string, string> data;
        mutex m;
        ofstream logFile;
        void addToLog(const string& operation) {
            if (logFile.is_open()) {
                logFile << operation << endl;
                logFile.flush();
            } 
        }
    
    public:
        KeyValueStore() {
            logFile.open("kv_store.log", ios::app);
            if (!logFile.is_open()) {
                cerr << "Log file cannot be opened, durability at risk." << endl;
            }
        }

        ~KeyValueStore() {
            if (logFile.is_open()) {
                logFile.close();
            }
        }

        bool put(const string& key, const string& value) {
            lock_guard<mutex> lock(m);
            data[key] = value;
            addToLog("PUT " + key + " " + value);
            return true;
        }

        bool get(const string& key, string& value){
            lock_guard<mutex> lock(m);
            auto it = data.find(key);
            if (it != data.end()) {
                value = it->second;
                return true;
            }
            return false;
        }
        
        bool remove(const string& key) {
            lock_guard<mutex> lock(m);
            auto it = data.find(key);
            if (it != data.end()) {
                data.erase(it);
                addToLog("DELETE " + key);
                return true;
            }
            return false;
        }

        shared_ptr<Transaction> beginTrasaction();
        bool applyTransaction(const vector<pair<string, string>>& puts, const vector<string>& deletes);

        void printAll() {
            lock_guard<mutex> lock(m);
            cout << "KeyValueStore contents:" << endl;
            for (const auto& pair:data) {
                cout << pair.first << " => " << pair.second << endl;
            }
            cout << endl;
        }
};