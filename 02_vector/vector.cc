#include <iostream>
#include <vector>
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
void test0() {
  vector<int> numbers{1, 2, 3, 4, 5, 6, 7, 8};
  for (auto &num : numbers) {

    cout << num << " ";
  }
  cout << endl;
  cout << "vector size= " << numbers.size() << endl;
  numbers.push_back(9);
  cout << "vector size= " << numbers.size() << endl;
  numbers.clear();
  for (auto &num : numbers) {

    cout << num << " ";
  }

  cout << "vector size= " << numbers.size() << endl;
};

int main() {
  test0();

  return 0;
}
