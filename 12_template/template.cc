#include <iostream>
using namespace std;
double ADD() { return 0; }
template <class T, class... Args> double ADD(T x, Args... args) {
  return x + ADD(args...);
}

void test0() { cout << ADD(1.1, 2, 3, 5) << endl; };

int main() {
  test0();

  return 0;
}
