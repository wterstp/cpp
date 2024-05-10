#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>
#include <map>
#include <set>
#include"SplitTool.h"











class DictProducer {
private:

    std::vector<std::string> enfiles;
    std::vector<std::string> chfiles;
    std::vector<std::pair<std::string, int>> endict;
    std::vector<std::pair<std::string, int>> chdict;
    std::unordered_map<std::string, std::set<int>> index;
    std::unordered_map<std::string, std::set<int>> chindex;
    SplitTool *cuttor;

public:
    DictProducer(const std::string& dir, SplitTool* tool);
    ~DictProducer();

    void buildEnDict() ;
    void buildCnDict() ;
    void createIndex() ;

    void store() ;
};

