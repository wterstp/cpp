#include <cstring>
#include <iostream>

class String {
public:
  String() : _pstr(nullptr) {
    _pstr = new char[1];
    *_pstr = '\0';
  }

  String(const char *pstr) {
    if (pstr != nullptr) {
      _pstr = new char[strlen(pstr) + 1];
      strcpy(_pstr, pstr);
    } else {
      _pstr = new char[1];
      *_pstr = '\0';
    }
  }

  String(const String &rhs) {
    _pstr = new char[strlen(rhs._pstr) + 1];
    strcpy(_pstr, rhs._pstr);
  }

  String &operator=(const String &rhs) {
    if (this != &rhs) {
      delete[] _pstr;
      _pstr = new char[strlen(rhs._pstr) + 1];
      strcpy(_pstr, rhs._pstr);
    }
    return *this;
  }

  ~String() { delete[] _pstr; }

  void print() {
    if (_pstr) {
      std::cout << _pstr << std::endl;
    }
  }

  size_t length() const { return strlen(_pstr); }

  const char *c_str() const { return _pstr; }

private:
  char *_pstr;
};

int main() {
  String str1;
  str1.print();

  String str2 = "Hello,world";
  String str3("wangdao");

  str2.print();
  str3.print();

  String str4 = str3;
  str4.print();

  str4 = str2;
  str4.print();

  return 0;
}
