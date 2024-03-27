#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::string;

class FunctionObject {
public:
  void operator()() { cout << "void operator()()" << endl; }
  void operator()(int x, int y) { cout << "x=" << x << ",y=" << y << endl; };
  void printf(int x) { cout << "printf(" << x << ")" << endl; };
};

typedef void (FunctionObject::*M)(int);
void test0() {
  FunctionObject fo;
  fo();
  fo(1, 2);
}

void test1() {
  M p = &FunctionObject::printf;
  FunctionObject f;
  (f.*p)(10);
}

int main() {
  test1();

  return 0;
}
