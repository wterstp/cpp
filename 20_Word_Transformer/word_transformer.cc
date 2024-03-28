#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
using namespace std;
class Word_transformer {
public:
  void read(const string filename);
  void store(const string filename);
  void display() {
    for (auto &it : _transformer) {
      cout << it.first << " " << it.second << endl;
    }
  }

private:
  map<string, string> _transformer;
};

void Word_transformer::read(const string filename) {
  ifstream map_file(filename);
  if (!map_file.is_open()) {
    cerr << "Error: " << filename << endl;
    return;
  }
  string line, rule, old_str, new_str;

  while (getline(map_file, line)) {
    istringstream stream(line);
    stream >> old_str;
    getline(stream >> ws, new_str);
    _transformer[old_str] = new_str;
  }

  map_file.close();
}

void Word_transformer::store(const string filename) {
  ifstream is(filename);
  string line;
  string new_line;
  vector<string> text;
  while (getline(is, line)) {
    istringstream iss(line);
    string word;
    new_line.clear();
    while (iss >> word) {
      auto it = _transformer.find(word);
      if (it != _transformer.end()) {
        word = it->second;
      }
      if (new_line.empty()) {
        new_line = word;
      } else {
        new_line += " " + word;
      }
    }
    text.push_back(new_line);
  }
  is.close();
  ofstream os(filename);
  for (auto &line : text) {
    os << line << endl;
  }
  os.close();
}

void test0() {
  Word_transformer file;
  file.read("map.txt");
  file.store("file.txt");
  file.display();
}

int main() {
  test0();

  return 0;
}
