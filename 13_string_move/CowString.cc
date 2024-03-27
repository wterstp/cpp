#include <iostream>
#include <string.h>
using std::cout;
using std::endl;
class String {
public:
  String() : _str(new char[1]()) {}
  /* String(){} */

  String(const char *pstr) : _str(new char[strlen(pstr) + 1]()) {
    cout << "String(const char *)" << endl;
    strcpy(_str, pstr);
  }

  //拷贝构造
  // String(const String &rhs) : _str(new char[strlen(rhs.c_str()) + 1]()) {
  //   cout << "String(const String &)" << endl;
  //   strcpy(_str, rhs.c_str());
  // }

  // 移动构造
  String(String &&rhs) : _str(rhs._str) {
    cout << "String(String&&)" << endl;
    rhs._str = nullptr;
  }

  //移动赋值函数
  String &operator=(String &&rhs) {
    cout << "String & operator=(String&&)" << endl;
    if (this != &rhs) {
      delete[] _str;
      //浅拷贝
      _str = rhs._str;
      /* _str = new char[1](); */
      rhs._str = nullptr;
    }
    return *this;
  }

  //默认拷贝构造
  /* String(const String & rhs) */
  /* : _str(rhs._str) */
  /* {} */

  //赋值运算符函数
  String &operator=(const String &rhs) {
    if (this != &rhs) {
      delete[] _str;
      _str = new char[strlen(rhs.c_str()) + 1];
      strcpy(_str, rhs.c_str());
      cout << "String& operator=(const String &)" << endl;
    }
    return *this;
  }

  ~String() {
    if (_str) {
      delete[] _str;
      _str = nullptr;
    }
    cout << "~String()" << endl;
  }

  void print() { cout << _str << endl; }

  size_t length() const { return strlen(_str); }

  const char *c_str() const { return _str; }

private:
  char *_str;
};

void test0() {
  String s1 = "hello";
  String s2 = std::move(s1);
  String s3;
  s3 = String("wangdao");
}

int main(void) {
  test0();
  return 0;
}
