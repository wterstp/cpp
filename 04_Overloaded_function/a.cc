#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::string;

class Data {
public:
  Data() { cout << "Data" << endl; }

  Data(int x) : _data(x) { cout << "Data(int)" << endl; }

  ~Data() { cout << "Data" << endl; }

  int getData() { return _data; }

private:
  int _data = 10;
};

class MiddleLayer {
public:
  MiddleLayer(Data *data) : _data(data) { cout << "MiddleLayer" << endl; }
  int getData() { return _data->getData(); }

  Data *operator->() { return _data; }

  ~MiddleLayer() {
    delete (_data);
    _data = nullptr;
  }

private:
  Data *_data;
};

void test0() {
  MiddleLayer m1(new Data(20));
  cout << "a=" << m1->getData() << endl;
};

int main() {
  test0();

  return 0;
}
