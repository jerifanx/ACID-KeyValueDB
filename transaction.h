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
#include "kvs.h"

using namespace std;

class Transaction {
    private:
        KeyValueStore& store;
        vector<pair<string,string>> pendingPuts;
        vector<string> pendingDeletes;
        bool isActive;
};
