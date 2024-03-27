#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::string;

class HeapOnlyClass {
public:
  HeapOnlyClass(const HeapOnlyClass &) = delete;
  HeapOnlyClass &operator=(const HeapOnlyClass &) = delete;
  static HeapOnlyClass *Create() {
    cout << "create!" << endl;

    return new HeapOnlyClass();
  };
  ~HeapOnlyClass() { cout << "delete!" << endl; }

private:
  HeapOnlyClass(){};
};

void test0() {
  HeapOnlyClass *heapobj = HeapOnlyClass::Create();
  delete heapobj;
};

int main() {
  test0();

  return 0;
}
