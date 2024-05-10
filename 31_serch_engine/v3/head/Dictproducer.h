#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>
#include <map>
#include <set>
#include "SplitTool.h"
#include<mutex>
class DictProducer {
private:
    static DictProducer *instance; // 静态实例指针
    static std::mutex mutex;       // 互斥锁，用于线程安全

    std::vector<std::string> enfiles;
    std::vector<std::string> chfiles;
    std::vector<std::pair<std::string, int>> endict;
    std::vector<std::pair<std::string, int>> chdict;
    std::unordered_map<std::string, std::set<int>> index;
    std::unordered_map<std::string, std::set<int>> chindex;
    SplitTool *cuttor;

    // 私有构造函数
    DictProducer(const std::string &dir, SplitTool *tool);
    // 防止外部删除实例
    ~DictProducer() ;

public:
    // 删除拷贝构造函数和赋值操作符
    DictProducer(const DictProducer &) = delete;
    DictProducer &operator=(const DictProducer &) = delete;

    // 静态方法获取实例
    static DictProducer *getInstance(const std::string &dir, SplitTool *tool)
    {
        if (instance == nullptr)
        {
            std::lock_guard<std::mutex> lock(mutex);
            if (instance == nullptr)
            {
                instance = new DictProducer(dir, tool);
            }
        }
        return instance;
    }
    
// 构造词典以及索引(不对外提供)
private:
    void buildEnDict();
    void buildCnDict();
    void createIndex();
    void store();
    
public:
    void start();
        std::vector<std::pair<std::string, int>> getchDict()
    {
        return chdict;
    }

    std::vector<std::pair<std::string, int>> &getenDict()
    {
        return endict;
    }

    std::unordered_map<std::string, std::set<int>> &getChIndex()
    {
        return chindex;
    }

    std::unordered_map<std::string, std::set<int>> &getEnIndex()
    {
        return index;
    }

    std::string doQuery(std::string word);

};

// 初始化静态成员变量
DictProducer* DictProducer::instance = nullptr;
std::mutex DictProducer::mutex;

