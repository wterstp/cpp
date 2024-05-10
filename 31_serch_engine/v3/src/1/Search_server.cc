#include "../head/Search_server.h"
#include <codecvt> // For std::codecvt_utf8
#include <fstream>
#include <locale> // For std::wstring_convert
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

std::vector<std::string> split(const std::string &s, char delimiter)
{

  
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(s);
  while (getline(tokenStream, token, delimiter))
  {
    tokens.push_back(token);
  }
  return tokens;
}




using namespace std;
SearchServer::~SearchServer() { delete cuttor; }

void SearchServer::loadDict(
    const std::string &enfilename = "../yuliao/dict/english_dict.txt",
    const std::string &chfilename = "../yuliao/dict/chinese_dict.txt")
{
  std::ifstream input(enfilename);
  std::string line;
  while (std::getline(input, line))
  {
    auto tokens = split(line, ' ');
    if (tokens.size() == 2)
    {
      endict.push_back({tokens[0], std::stoi(tokens[1])});
    }
  }
  input.close();

  std::ifstream input2(chfilename);
  while (std::getline(input2, line))
  {
    auto tokens = split(line, ' ');
    if (tokens.size() == 2)
    {
      chdict.push_back({tokens[0], std::stoi(tokens[1])});
    }
  }
  input2.close();
}

void SearchServer::loadIndex(
    const std::string &filename = "../yuliao/dict/dict_index.txt",
    const std::string &filename2 = "../yuliao/dict/chdict_index.txt")
{
  // 处理英文词典
  std::ifstream input(filename);
  std::string line;
  while (std::getline(input, line))
  {
    std::istringstream iss(line);
    std::string token;
    std::set<int> weights; // 定义权重向量，用于存放每行的权重
    std::string key;       // 定义关键词变量

    if (!(iss >> key))
      continue; // 首先读取关键词，失败则跳过该行

    // 读取所有权重，存入weights向量
    while (iss >> token)
    {
      try
      {
        weights.insert(std::stoi(token));
      }
      catch (const std::invalid_argument &ia)
      {
        // 处理转换失败的情况，忽略该权重
        continue;
      }
    }

    // 检查权重列表是否非空，然后更新索引
    if (!weights.empty())
    {

      index[key] = std::move(weights); // 使用move将权重向量存储到索引中
    }
    else
    {
      index[key] = std::move(weights); // 新建条目
    }
  }
  input.close();
  // 处理中文词典
  std::ifstream input2(filename2);
  while (std::getline(input2, line))
  {
    std::istringstream iss(line);
    std::string token;
    std::set<int> ids;
    std::string key;

    if (!(iss >> key))
      continue; // 读取关键词，如果失败则跳过该行

    while (iss >> token)
    { // 读取ID
      try
      {
        ids.insert(std::stoi(token)); // 直接插入到set中
      }
      catch (const std::invalid_argument &ia)
      {
        // 忽略无法转换的字符串
        continue;
      }
    }

    // 只有当set不为空时才合并或更新index
    if (!ids.empty())
    {
      if (chindex.find(key) != chindex.end())
      {
        index[key].insert(ids.begin(), ids.end()); // 合并到现有的set中
      }
      else
      {
        chindex[key] = std::move(ids); // 新建条目
      }
    }
  }
  input2.close();
}

void SearchServer::loadAll()
{
  loadDict();
  loadIndex();
}

int minEditDist(const std::string& s1, const std::string& s2) {
    // 使用 codecvt_utf8 转换 s1 和 s2 到 Unicode 码点序列
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    std::u32string u32s1 = converter.from_bytes(s1);
    std::u32string u32s2 = converter.from_bytes(s2);

    int m = u32s1.size(), n = u32s2.size();
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

    for (int i = 0; i <= m; i++)
        dp[i][0] = i;
    for (int j = 0; j <= n; j++)
        dp[0][j] = j;

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (u32s1[i - 1] == u32s2[j - 1])
                dp[i][j] = dp[i - 1][j - 1];
            else
                dp[i][j] = std::min({dp[i - 1][j] + 1, dp[i][j - 1] + 1, dp[i - 1][j - 1] + 1});
        }
    }
    return dp[m][n];
}
std::vector<std::string>
SearchServer::getSuggestions(const std::string &input)
{
  std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
  std::u32string unicode_input = converter.from_bytes(input);
  std::vector<std::pair<std::string, int>> suggestions;
  std::set<int> chCommonIds, enCommonIds;
  bool chFirstChar = true, enFirstChar = true;
  string chin;
  string en;

  for (char32_t ch : unicode_input)
  {
    std::string key = converter.to_bytes(ch);
    if (isChinese(ch))
    { // 中文处理
      chin += key;
      if (chindex.find(key) != chindex.end())
      {
        
        if (chFirstChar)
        {
          chCommonIds = chindex[key];
          chFirstChar = false;
        }
        else
        {
          std::set<int> temp;
          std::set_intersection(chCommonIds.begin(), chCommonIds.end(),
                                chindex[key].begin(), chindex[key].end(),
                                std::inserter(temp, temp.begin()));
          chCommonIds = std::move(temp);
        }
      }
    }
    else
    { // 英文处理
      en+=key;
      if (index.find(key) != index.end())
      {
        
        if (enFirstChar)
        {
          enCommonIds = index[key];
          enFirstChar = false;
        }
        else
        {
          std::set<int> temp;
          std::set_intersection(enCommonIds.begin(), enCommonIds.end(),
                                index[key].begin(), index[key].end(),
                                std::inserter(temp, temp.begin()));
          enCommonIds = std::move(temp);
        }
      }
    }
  }

  std::map<std::string, int> miniEditword;
  // 处理中文结果
  for (auto idx : chCommonIds)
  {
    auto &entry = chdict[idx];
    int dist = (minEditDist(chin, entry.first));
    miniEditword.insert({entry.first, dist});
    if (dist <= 4)
    {
      suggestions.push_back(entry);
    }
  }

  // 处理英文结果
  for (auto idx : enCommonIds){
    auto &entry = endict[idx];
    int dist = minEditDist(en, entry.first);
    miniEditword.insert({entry.first, dist});
    if (dist <= 4)
    {
      suggestions.push_back(entry);
    }
  }

  // 排序
std::sort(suggestions.begin(), suggestions.end(),
    [&miniEditword](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
        int distA = miniEditword[a.first];
        int distB = miniEditword[b.first];
        
        if (distA != distB) {
            return distA < distB;  // 按编辑距离升序排序
        }
        if (a.second != b.second) {
            return a.second > b.second;  // 按词频降序排序
        }
        return a.first < b.first;  // 按字母表顺序升序排序
    });


  //只取前五个结果
  std::vector<std::string> result;
  for(int i = 0; i < 5 && i < suggestions.size(); i++){
      result.push_back(suggestions[i].first);
  }
  return result;
}

int main()
{
  SplitTool *jieba = new SplitTool();
  SearchServer s(jieba);
  s.loadAll();

  std::string input;
  while (true)
  {
    std::cout << "Enter prefix: ";
    std::cin >> input;
    if (input == "exit")
      break;

    auto suggestions = s.getSuggestions(input);
    for (const auto &word : suggestions)
      std::cout << "Suggestion: " << word << std::endl;
  }
  return 0;
}
