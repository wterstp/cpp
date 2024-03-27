#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>

using namespace std;

class TextQuery {
public:
  void readFile(const string &filename);
  void query(const string &word);

private:
  std::vector<std::string> _lines;
  std::map<string, set<int>> _wordNumbers;
  std::map<string, int> _dict;
};

void TextQuery::readFile(const string &filename) {
  ifstream file(filename);
  if (!file.is_open()) {
    cerr << "Error opening file: " << filename << std::endl;
    return;
  }
  string line;
  int linenumber = 0;
  while (getline(file, line)) {
    _lines.push_back(line);
    ++linenumber;

    istringstream iss(line);
    string word;
    while (iss >> word) {
      transform(word.begin(), word.end(), word.begin(), ::tolower);
      ++_dict[word];
      _wordNumbers[word].insert(linenumber);
    }
  }
  file.close();
}

void TextQuery::query(const string &word) {
  string wordLower = word;
  transform(wordLower.begin(), wordLower.end(), wordLower.begin(), ::tolower);
  auto it = _wordNumbers.find(wordLower);
  if (it != _wordNumbers.end()) {
    cout << word << "出现:" << _dict[wordLower] << "次" << endl;
    for (int linenumber : it->second) {
      cout << "(line " << linenumber << ")"
           << " " << _lines[linenumber - 1] << endl;
    }

  } else {
    cout << "Word not foud" << endl;
  }
}

void test0() {
  string word;
  cout << "请输入要查询的单词(-1结束): ";
  while (cin >> word && word != "-1") {
    TextQuery query;
    query.readFile("china_daily.txt");
    query.query(word);
    cout << "请输入要查询的单词(-1结束): ";
  }
}

int main(int argc, char *argv[]) {
  test0();
  return 0;
}
