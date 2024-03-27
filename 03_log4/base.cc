#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::string;

class Base {
public:
  Base(int value) : val(value) {}

  Base operator+(const Base &other) { return Base(other.val - val); }
  int getVal() const { return val; }

private:
  int val;
};

void test0() {
  int i = 2;
  int j = 7;

  Base x(i);
  Base y(j);
  cout << ((x + y).getVal() == j - i) << endl;
};

int main() {
  test0();

  return 0;
}
