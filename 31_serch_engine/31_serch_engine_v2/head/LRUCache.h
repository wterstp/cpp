#include <list>
#include <unordered_map>
#include <string>
#include <vector>
#include <mutex>
#include <locale>
#include <codecvt>
#include "Speech_recognition.h"
#include<set>
class LRUCache {
private:struct CacheItem {
        std::string key;
         std::vector<std::string> value; 
        CacheItem(const std::string& key, std::vector<std::string> value) : key(key), value(value) {};
    };

    std::list<CacheItem> cacheList;
    std::unordered_map<std::string, std::list<CacheItem>::iterator> cacheMap;
    std::list<CacheItem> _pendingUpdateList;
    size_t capacity;
    mutable std::mutex cacheMutex;

public:
    LRUCache (size_t capacity=1000);

    void put(const std::string& key, std::vector<std::string>& value);
     void addPendingUpdate(const std::string& key, std::vector<std::string>& value);
    std::vector<std::string> get(const std::string& key);
    void readFromFile(const std::string& word);
    void processUpdates();
    void loadIndexesFromFile(const std::string& filePath, const std::string& key, std::set<int>& indexSet);

};