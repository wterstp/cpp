#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <set>
#include "SplitTool.h"

class SearchServer {
    private:
    std::vector<std::pair<std::string, int>> endict;
    std::vector<std::pair<std::string, int>> chdict;
    std::unordered_map<std::string, std::set<int>> index;
    std::unordered_map<std::string, std::set<int>> chindex;
    SplitTool *cuttor;

    public:
    SearchServer(SplitTool* tool) : cuttor(tool){};
    ~SearchServer();
    void loadDict(const std::string &enfilename,const std::string &chfilename);//加载词典
    void loadIndex(const std::string &filename,const std::string &filename2);//加载索引
    void loadAll();
    std::vector<std::string> getSuggestions(const std::string& input);
        
    



};