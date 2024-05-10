#include "../head/LRUCache.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <set>
#include <algorithm>

using std::string;
using std::cout;


LRUCache::LRUCache (size_t capacity):capacity(capacity) {
}

std::vector<std::string> LRUCache::get(const std::string &key)
{
    cout << "loadIndexesFromFile" << std::endl;
    std::lock_guard<std::mutex> lock(cacheMutex);

    auto it = cacheMap.find(key);

    if (it == cacheMap.end())
    {

        std::vector<string> empty = readFromFile(key);
        put(key, empty);
        return empty;
    }

    return it->second->value;
}
void LRUCache::put (const std::string& key, std::vector<std::string>& value) {
    
    auto it=cacheMap.find(key);
    if(it!=cacheMap.end()){
         it->second->value = value;
         cacheList.splice(cacheList.begin(),cacheList,it->second);
         return;
    }
    if(cacheMap.size()==capacity){
        cacheMap.erase(cacheList.back().key);
        cacheList.pop_back();
    }
    cacheList.push_front({key,value});
    cacheMap[key]=cacheList.begin();
    
}


void LRUCache:: addPendingUpdate(const std::string& key,std::vector<std::string>& value){
     _pendingUpdateList.emplace_back(key, value);
}


void   LRUCache:: processUpdates(){
    for(auto& update:_pendingUpdateList){
        put(update.key,update.value);
    }
        _pendingUpdateList.clear();
}







//!读取文件用于将对应的索引添加到set中
void LRUCache:: loadIndexesFromFile(const std::string& filePath, const std::string& key, std::set<int>& indexSet) {
     std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    std::u32string u32word = converter.from_bytes(key);
    std::ifstream fileStream(filePath);
    std::string line;
    std::set<int> commonIds;
    bool firstchat=true;
    if (!fileStream) {
        std::cerr << "Unable to open file: " << filePath << std::endl;
        return;
    }
 for (char32_t ch : u32word) {
        std::string key = converter.to_bytes(ch);
    while (getline(fileStream, line)) {
        std::istringstream iss(line);
        std::string indexKey;
        int indexValue;

        iss >> indexKey; // First, read the keyword
        if (indexKey == key) { // If the keyword matches
            if(firstchat)
            {while (iss >> indexValue) { // Read all the index numbers that follow
                commonIds.insert(indexValue);
            }}
            else{
                std::set<int> currentIds;
                std::set<int> temp;
                while (iss >> indexValue) { // Read all the index numbers that follow
                currentIds.insert(indexValue);
                std::set_intersection(commonIds.begin(), commonIds.end(),
                                currentIds.begin(), currentIds.end(),
                                std::inserter(temp, temp.begin()));
                commonIds = std::move(temp);
                }
            }
            break; // Break the loop if a match is found
        }
    }
}
    indexSet=std::move(commonIds);
}



//! 读取文件内容,最终拿到词频
bool getLineData(const std::string& filename, int lineNumber, std::string& word, int& frequency) {
    std::ifstream file(filename);
    std::string line;
    int currentLine = 0;
    
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return false;
    }
    
    while (getline(file, line)) {
        ++currentLine;
        if (currentLine == lineNumber) {
            std::istringstream iss(line);
            if (!(iss >> word >> frequency)) {
                return false;  // 处理读取或格式错误
            }
            return true;
        }
    }
    return false;  // 没有找到指定的行
}



//! 如缓存没有此关键词,会执行此函数去文件查找
std::vector<std::string> LRUCache::readFromFile(const std::string &word)
{
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    std::u32string u32word = converter.from_bytes(word);
    std::vector<std::pair<std::string, int>> suggestions;
    std::set<int> chCommonIds, enCommonIds;
    std::string chinese;
    std::string english;

    for (char32_t ch : u32word)
    {
        std::string key = converter.to_bytes(ch);
        if (isChinese(ch))
        {
            chinese += key;
        }
        else if (isEnglish(key))
        {
            english += key;
        }
    }

    if (!chinese.empty())
    {
        loadIndexesFromFile("../../yuliao/dict/chdict_index.txt", chinese, chCommonIds);
    }
    if (!english.empty())
    {
        loadIndexesFromFile("../../yuliao/dict/dict_index.txt", english, enCommonIds);
    }
    std::unordered_map<std::string, int> Word_chFrequency;
    std::unordered_map<std::string, int> Word_enFrequency;
    for (auto &line : chCommonIds)
    {
        std::string word;
        int frequency;
        getLineData("../../yuliao/dict/chinese_dict.txt", line + 1, word, frequency);
        Word_chFrequency[word] = frequency;
    }

    for (auto &line : enCommonIds)
    {

        std::string word;
        int frequency;
        getLineData("../../yuliao/dict/english_dict.txt", line + 1, word, frequency);
        Word_enFrequency[word] = frequency;
    }

    std::unordered_map<std::string, int> miniEditword;
    // 处理中文结果
    for (auto &word : Word_chFrequency)
    {

        int dist = (minEditDist(chinese, word.first));
        miniEditword.insert({word.first, dist});
        if (dist <= 4)
        {
            suggestions.push_back(word);
        }
    }

    // 处理英文结果
    for (auto &word : Word_enFrequency)
    {

        int dist = (minEditDist(english, word.first));
        miniEditword.insert({word.first, dist});
        if (dist <= 4)
        {
            suggestions.push_back(word);
        }
    }
    // 排序
    std::sort(suggestions.begin(), suggestions.end(),
              [&miniEditword](const std::pair<std::string, int> &a, const std::pair<std::string, int> &b)
              {
                  int distA = miniEditword[a.first];
                  int distB = miniEditword[b.first];

                  if (distA != distB)
                  {
                      return distA < distB; // 按编辑距离升序排序
                  }
                  if (a.second != b.second)
                  {
                      return a.second > b.second; // 按词频降序排序
                  }
                  return a.first < b.first; // 按字母表顺序升序排序
              });

    std::vector<std::string> result;
    for (int i = 0; i < 5 && i < suggestions.size(); i++)
    {
        result.push_back(suggestions[i].first);
    }
    return result;
}
