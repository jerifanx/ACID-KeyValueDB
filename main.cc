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

        }

        bool get(){

        }
        
        bool remove() {

        }
};