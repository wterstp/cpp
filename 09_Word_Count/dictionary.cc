#include <cctype> // For std::isalpha
#include <fstream>
#include <iostream>
#include <map>

using namespace std;

class Dictionary {
public:
  void read(const string filename);
  void store(const string filename);

private:
  map<string, int> _dict;
};

void Dictionary::read(const string filename) {
  ifstream file(filename);
  if (!file.is_open()) {
    cerr << "Error: Could not open " << filename << endl;
    return;
  }
  string word;
  while (file >> word) {
    string clean_word;
    for (char c : word) {
      if (isalpha(c)) {
        clean_word += tolower(c);
      }
    }
    if (!clean_word.empty()) {
      ++_dict[clean_word];
    }
  }
  file.close();
}

void Dictionary::store(const string filename) {
  ofstream file(filename);
  if (!file.is_open()) {
    cerr << "Error: Could not open " << filename << endl;
    return;
  }
  for (auto &pair : _dict) {
    file << pair.first << " " << pair.second << "\n";
  }
  file.close();
}

void test0() {
  Dictionary dict;
  string input_file = "The_Holy_Bible.txt";
  string output_file = "dict.txt";
  dict.read(input_file);
  dict.store(output_file);
};

int main() {
  time_t start = time(NULL);
  test0();
  time_t end = time(NULL) - start;
  cout << end << "s" << endl;
  return 0;
}
