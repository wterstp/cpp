#include <iostream>
#include <filesystem> 
#include <string>
#include <vector>
#include <fstream>
#include <unordered_set>
#include"head/Dictproducer.h"
#include <sstream>
#include <nlohmann/json.hpp>
#include <locale>
#include <codecvt>

using namespace std;
namespace fs = std::filesystem;
std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

std::string cleanWord(const std::string& word) {            //转小写
    std::string cleaned;
    for (char c : word) {
        if (isalnum(c)) cleaned += tolower(c);
    }
    return cleaned;
}

bool isChinese(const std::string &word) {        //判断是否为中文
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    std::wstring wstr = conv.from_bytes(word);
    for (wchar_t wc : wstr) {
        if ((wc >= 0x4E00 && wc <= 0x9FFF) || 
            (wc >= 0x3400 && wc <= 0x4DBF) || 
            (wc >= 0x20000 && wc <= 0x2A6DF)) {
            return true;
        }
    }
    return false;
}


bool isEnglish(const std::string &word) {           //判断是否为英文
    for (char ch : word) {
        if (!((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))) {
            return false;
        }
    }
    return true;
}

unordered_set <string>loadstopWords(const string &filepath){
    std::unordered_set<std::string> stopWords; 
    std::ifstream file(filepath);
    std::string word;
    if(file.is_open()){
        while(file >> word){
            stopWords.insert(word);
        }
        file.close();
    }else{
        cout<<"Unable to open file\n";

    }
    return stopWords;

}









 DictProducer :: DictProducer(const std::string& dir, SplitTool* tool) : cuttor(tool) {
        for(const auto &entry : fs::recursive_directory_iterator(dir)){
            if(entry.is_regular_file()){
                std::string path=entry.path().string();
                
                if(path.find("english")!=std::string::npos){
                    
                    enfiles.push_back(path.c_str());
                }else if(path.find("Art")!=std::string::npos){
                    
                    chfiles.push_back(path.c_str());
                }

            }

        }


 }





void  DictProducer::buildEnDict(){
    std::unordered_set <string> stopwords=loadstopWords("./yuliao/stop/stop_words_eng.txt");
    std::map<std::string, int> frequencyMap;
        for(const auto&file:enfiles){
                std::ifstream in(file);
                std::string word;
                int lineNum=0;
                
                while(in>>word){
                    word=cleanWord(word);
                    if(stopwords.find(word)==stopwords.end()&&!word.empty()&&isEnglish(word)){
                        frequencyMap[word]++;
                    }

                }
        in.close();
    }
    
        endict.clear();
        for(const auto&pair:frequencyMap){
            endict.push_back(pair);
        }
       std::sort(endict.begin(), endict.end(),
              [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
                  return a.first < b.first;
              });
 }



void  DictProducer::buildCnDict(){
    std::unordered_set <string> stopwords=loadstopWords("./yuliao/stop/stop_words_zh.txt");
    std::map<std::string, int> frequencyMap;
    for(const auto&file:chfiles){
        std::ifstream in(file);
        std::string word;
        std::string line;
        while(std::getline(in,line)){
            std::vector<string>ch= cuttor->cut(line);
            for(const auto& word : ch){
                if(stopwords.find(word)==stopwords.end()&&!word.empty()&&isChinese(word)){
                    frequencyMap[word]++;
                }
            }
                }
                in.close();
            }
            chdict.clear();
            for(const auto &pair:frequencyMap){
                chdict.push_back(pair);
            }

        }
    

 void DictProducer::createIndex() {
    // Create index for English dictionary
    for (int i = 0; i < endict.size(); ++i) {
        int linenumber=0;
        const auto& word = endict[i].first;
        string key=word;
        for(size_t j=0;j<key.size();++j){
            if (index.find(std::string(1, key[j])) != index.end()){
                index[std::string(1, key[j])].insert(i);
            }
            else{
                index[std::string(1, key[j])].insert(i);
            }
        }
 
    }

    // Create index for Chinese dictionary
    for (int i = 0; i < chdict.size(); ++i) {
       
        const auto& word = chdict[i].first;
        std::wstring wide_string = converter.from_bytes(word);
        
        for(wchar_t wc : wide_string){
            std::string key = converter.to_bytes(wc);
            if (chindex.find(key) != chindex.end()){
                chindex[key].insert(i);
            }
            else{
                chindex[key].insert(i);
            }
        }
    }
}

 void DictProducer:: store(){
    std::ofstream out("./yuliao/dict/english_dict.txt");
    std::ofstream out2("./yuliao/dict/chinese_dict.txt");
    for(const auto&item:endict){
        out<<item.first<<" "<<item.second<<std::endl;
    }
    for(const auto&item:chdict){
        out2<<item.first<<" "<<item.second<<std::endl;
    }
    out.close();
    out2.close();

    std::ofstream out3("./yuliao/dict/chdict_index.txt");
     for(const auto&item:chindex){
        out3<<item.first<<" ";
        for(auto i:item.second){
            if(i!=*(--(item.second.end()))){
            out3<<i<<" ";
            }
            else{
                out3<<i;
            }

        }
        out3<<std::endl;
    }
    out3.close();

 std::ofstream out4("./yuliao/dict/dict_index.txt");
     for(const auto&item:index){
        out4<<item.first<<" ";
        for(auto i:item.second){
            if(i!=*(--(item.second.end()))){
            out4<<i<<" ";
            }
            else{
                out4<<i;
            }

        }
        out4<<std::endl;
    }
    out4.close();

 }

DictProducer::~DictProducer(){
    delete cuttor;
}


int main(){
    SplitTool * jieba=new SplitTool();
    const string dir="./yuliao";
    DictProducer  dp(dir,jieba);
    dp.buildCnDict();
    dp.buildEnDict();
    dp.createIndex();
    dp.store();
    
    
}