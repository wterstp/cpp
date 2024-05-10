#include "head/SplitTool.h"
#include <string>
using std::string;

const char * const DICT_PATH = "./cppjieba-5.0.3/include/cppjieba/dict/jieba.dict.utf8";
const char * const HMM_PATH = "./cppjieba-5.0.3/include/cppjieba/dict/hmm_model.utf8";
const char * const USER_DICT_PATH = "./cppjieba-5.0.3/include/cppjieba/dict/user.dict.utf8";
const char * const IDF_PATH = "./cppjieba-5.0.3/include/cppjieba/dict/idf.utf8";
const char * const STOP_WORD_PATH = "./cppjieba-5.0.3/include/cppjieba/dict/stop_words.utf8";

 SplitTool :: SplitTool() : _jieba(DICT_PATH,
        HMM_PATH,
        USER_DICT_PATH,
        IDF_PATH,
        STOP_WORD_PATH)
 {
 }

 std::vector<std::string> SplitTool:: cut(const std::string& sentence)  {
        std::vector<string> out;
        _jieba.Cut(sentence, out, true);
        return out;
}