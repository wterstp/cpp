#include "head/Search_server.h"
#include <codecvt> // For std::codecvt_utf8
#include <fstream>
#include <locale> // For std::wstring_convert
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

std::vector<std::string> split(const std::string &s, char delimiter) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(s);
  while (getline(tokenStream, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}

bool isChinese(const std::string &word) { //判断是否为中文
  std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
  std::wstring wstr = conv.from_bytes(word);
  for (wchar_t wc : wstr) {
    if ((wc >= 0x4E00 && wc <= 0x9FFF) || (wc >= 0x3400 && wc <= 0x4DBF) ||
        (wc >= 0x20000 && wc <= 0x2A6DF)) {
      return true;
    }
  }
  return false;
}

bool isEnglish(const std::string &word) { //判断是否为英文
  for (char ch : word) {
    if (!((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))) {
      return false;
    }
  }
  return true;
}

using namespace std;
SearchServer::~SearchServer() { delete cuttor; }

void SearchServer::loadDict(
    const std::string &enfilename = "yuliao/dict/english_dict.txt",
    const std::string &chfilename = "yuliao/dict/chinese_dict.txt") {
  std::ifstream input(enfilename);
  std::string line;
  while (std::getline(input, line)) {
    auto tokens = split(line, ' ');
    if (tokens.size() == 2) {
      endict.push_back({tokens[0], std::stoi(tokens[1])});
    }
  }
  input.close();

  std::ifstream input2(chfilename);
  while (std::getline(input2, line)) {
    auto tokens = split(line, ' ');
    if (tokens.size() == 2) {
      chdict.push_back({tokens[0], std::stoi(tokens[1])});
    }
  }
  input2.close();
}

void SearchServer::loadIndex(
    const std::string &filename = "yuliao/dict/dict_index.txt",
    const std::string &filename2 = "yuliao/dict/chdict_index.txt") {
  //处理英文词典
  std::ifstream input(filename);
  std::string line;
  while (std::getline(input, line)) {
    std::istringstream iss(line);
    std::string token;
    std::set<int> weights; // 定义权重向量，用于存放每行的权重
    std::string key;       // 定义关键词变量

    if (!(iss >> key))
      continue; // 首先读取关键词，失败则跳过该行

    // 读取所有权重，存入weights向量
    while (iss >> token) {
      try {
        weights.insert(std::stoi(token));
      } catch (const std::invalid_argument &ia) {
        // 处理转换失败的情况，忽略该权重
        continue;
      }
    }

    // 检查权重列表是否非空，然后更新索引
    if (!weights.empty()) {

      index[key] = std::move(weights); // 使用move将权重向量存储到索引中
    } else {
      index[key] = std::move(weights); // 新建条目
    }
  }
  input.close();
  //处理中文词典
  std::ifstream input2(filename2);
  while (std::getline(input2, line)) {
    std::istringstream iss(line);
    std::string token;
    std::set<int> ids;
    std::string key;

    if (!(iss >> key))
      continue; // 读取关键词，如果失败则跳过该行

    while (iss >> token) { // 读取ID
      try {
        ids.insert(std::stoi(token)); // 直接插入到set中
      } catch (const std::invalid_argument &ia) {
        // 忽略无法转换的字符串
        continue;
      }
    }

    // 只有当set不为空时才合并或更新index
    if (!ids.empty()) {
      if (chindex.find(key) != chindex.end()) {
        index[key].insert(ids.begin(), ids.end()); // 合并到现有的set中
      } else {
        chindex[key] = std::move(ids); // 新建条目
      }
    }
  }
  input2.close();
}

void SearchServer::loadAll() {
  loadDict();
  loadIndex();
}

int minEditDist(const std::string &s1, const std::string &s2) { //最小编辑距离
  int m = s1.size(), n = s2.size();
  std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

  for (int i = 0; i <= m; i++)
    dp[i][0] = i;
  for (int j = 0; j <= n; j++)
    dp[0][j] = j;

  for (int i = 1; i <= m; i++) {
    for (int j = 1; j <= n; j++) {
      if (s1[i - 1] == s2[j - 1])
        dp[i][j] = dp[i - 1][j - 1];
      else
        dp[i][j] = std::min(
            {dp[i - 1][j] + 1, dp[i][j - 1] + 1, dp[i - 1][j - 1] + 1});
    }
  }
  return dp[m][n];
}

std::vector<std::string>
SearchServer::getSuggestions(const std::string &input) { //
  // 如果输入是中文
  if (isChinese(input)) { // 使用codecvt_utf8转换input从char32_t到std::wstring
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    std::u32string unicode_input = converter.from_bytes(input);

    // 初始化suggestions和commonIds
    std::vector<std::pair<std::string, int>> suggestions;
    std::set<int> commonIds;
    bool firstChar = true;

    // 遍历unicode_input中的每一个字符
    for (char32_t ch : unicode_input) {
      // 将字符转换为std::string
      std::string key = converter.to_bytes(ch);
      // 如果chindex中存在该字符
      if (chindex.find(key) != chindex.end()) {
        // 如果firstChar为true，则将chindex中该字符对应的id集合赋值给commonIds
        if (firstChar) {
          commonIds = chindex[key];
          firstChar = false;
          // 否则，将commonIds和chindex中该字符对应的id集合取交集
        } else {
          std::set<int> temp;
          std::set_intersection(commonIds.begin(), commonIds.end(),
                                chindex[key].begin(), chindex[key].end(),
                                std::inserter(temp, temp.begin()));
          commonIds = std::move(temp);
        }
        // 如果commonIds为空，则跳出循环
        if (commonIds.empty())
          break;
      }
    }
    // 遍历commonIds中的每一个id
    for (int idx : commonIds) {
      // 获取chdict中该id对应的entry
      auto &entry = chdict[idx];
      // 计算input和entry.first的最小编辑距离
      int dist = minEditDist(input, entry.first);
      // 如果dist小于等于4，则将entry加入suggestions
      if (dist <= 4) {
        suggestions.push_back(entry);
      }
    }

    // 对suggestions中的元素进行排序
    std::sort(suggestions.begin(), suggestions.end(),
              [](const std::pair<std::string, int> &a,
                 const std::pair<std::string, int> &b) {
                //如果词频相等,则按字母表顺序排列
                return a.second > b.second ||
                       (a.second == b.second && a.first < b.first);
              });

    // 定义一个空的vector
    std::vector<std::string> result;
    // 定义一个计数器
    int count = 0;
    // 遍历suggestions中的元素
    for (auto &[word, _] : suggestions) {
      // 将word添加到result中
      result.push_back(word);
      // 如果计数器等于5，跳出循环
      if (++count == 5)
        break;
    }
    // 返回result
    return result;
  }

  //如果输入英文
  else {
    std::vector<std::pair<std::string, int>> suggestions;

    std::set<int> commonIds;
    bool firstChar = true;
    for (char ch : input) {
      std::string key = std::string(1, ch);
      if (index.find(key) != index.end()) {
        if (firstChar) {
          commonIds = index[key];
          firstChar = false;
        } else {
          std::set<int> temp;
          std::set_intersection(commonIds.begin(), commonIds.end(),
                                index[key].begin(), index[key].end(),
                                std::inserter(temp, temp.begin()));
          commonIds = std::move(temp);
        }
        if (commonIds.empty())
          break;
      }
    }
    for (int idx : commonIds) {
      auto &entry = endict[idx];
      int dist = minEditDist(input, entry.first);
      if (dist <= 4) {
        suggestions.push_back(entry);
      }
    }
    std::sort(suggestions.begin(), suggestions.end(),
              [](const std::pair<std::string, int> &a,
                 const std::pair<std::string, int> &b) {
                return a.second > b.second ||
                       (a.second == b.second && a.first < b.first);
              });

    std::vector<std::string> result;
    int count = 0;
    for (auto &[word, _] : suggestions) {
      result.push_back(word);
      if (++count == 5)
        break;
    }
    return result;
  }
}

int main() {
  SplitTool *jieba = new SplitTool();
  SearchServer s(jieba);
  s.loadAll();

  std::string input;
  while (true) {
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
