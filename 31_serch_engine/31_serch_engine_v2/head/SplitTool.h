#include <vector>
#include "../cppjieba-5.0.3/include/cppjieba/Jieba.hpp"


class SplitTool{
      public:
       
        SplitTool() ;
        
        std::vector<std::string> cut(const std::string& sentence);
       
     private:
       cppjieba:: Jieba _jieba;
};