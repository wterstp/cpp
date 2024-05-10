#ifndef SEARCH_SERVER_H
#define SEARCH_SERVER_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <set>
#include "SplitTool.h"
#include"LRUCache.h"
class SearchServer {
    private:
    std::vector<std::pair<std::string, int>> endict;  // 用于英文存储词典
    std::vector<std::pair<std::string, int>> chdict;  // 用于中文存储词典
    std::unordered_map<std::string, std::set<int>> index;  // 用于英文存储索引
    std::unordered_map<std::string, std::set<int>> chindex;   // 用于中文存储索引
    SplitTool *cuttor;    // 分词器
    LRUCache cache;     // 缓存

    public:
    SearchServer(SplitTool* tool) : cuttor(tool),cache(1000){};
    // SearchServer(SplitTool* tool) : cuttor(tool){};
    ~SearchServer();
    void loadDict(const std::string &enfilename,const std::string &chfilename);//加载词典
    void loadIndex(const std::string &filename,const std::string &filename2);//加载索引
    void loadAll();  // 加载词典和索引
    std::vector<std::string> getSuggestions(const std::string& input);   
    

    



};

#endif