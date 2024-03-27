#include <algorithm>
#include <cstring>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::string;

class String {
public:
  String() : _pstr(new char[1]) { _pstr[0] = '\0'; }

  String(const char *str) : _pstr(new char[std::strlen(str) + 1]) {
    std::strcpy(_pstr, str);
  }
  String(const String &str) : _pstr(new char[str.size() + 1]) {
    std::strcpy(_pstr, str._pstr);
  }
  ~String() { delete[] _pstr; };
  String &operator=(const String &str) {
    if (this != &str) {
      char *new_pstr = new char[(str.size() + 1)];
      std::strcpy(new_pstr, str._pstr);
      delete[] _pstr;
      _pstr = new_pstr;
    }
    return *this;
  }
  String &operator=(const char *str) {
    if (_pstr != str) {
      char *new_pstr = new char[std::strlen(str) + 1];
      std::strcpy(new_pstr, str);
      delete[] _pstr;
      _pstr = new_pstr;
    }
    return *this;
  }

  String &operator+=(const char *str) {
    char *new_pstr = new char[std::strlen(_pstr) + std::strlen(str) + 1];
    std::strcpy(new_pstr, _pstr);
    std::strcat(new_pstr, str);
    delete[] _pstr;
    _pstr = new_pstr;
    return *this;
  }

  String &operator+=(const String &str) { return *this += str._pstr; }

  char &operator[](std::size_t index) { return _pstr[index]; }
  const char &operator[](std::size_t index) const { return _pstr[index]; }

  std::size_t size() const { return std::strlen(_pstr); }
  const char *c_str() const { return _pstr; }

  friend bool operator==(const String &lhs, const String &rhs);
  friend bool operator!=(const String &lhs, const String &rhs);

  friend bool operator<(const String &lhs, const String &rhs);
  friend bool operator>(const String &lhs, const String &rhs);
  friend bool operator<=(const String &lhs, const String &rhs);
  friend bool operator>=(const String &lhs, const String &rhs);

  friend std::ostream &operator<<(std::ostream &os, const String &s);
  friend std::istream &operator>>(std::istream &is, String &s);

private:
  char *_pstr;
};

bool operator==(const String &lhs, const String &rhs) {
  return std::strcmp(lhs._pstr, rhs._pstr) == 0;
}

bool operator!=(const String &lhs, const String &rhs) { return !(lhs == rhs); }

bool operator<(const String &lhs, const String &rhs) {
  return std::strcmp(lhs._pstr, rhs._pstr) < 0;
}
bool operator>(const String &lhs, const String &rhs) {
  return std::strcmp(lhs._pstr, rhs._pstr) > 0;
}

bool operator<=(const String &lhs, const String &rhs) {
  return !(std::strcmp(lhs._pstr, rhs._pstr) > 0);
}

bool operator>=(const String &lhs, const String &rhs) {
  return !(std::strcmp(lhs._pstr, rhs._pstr) < 0);
}

std::ostream &operator<<(std::ostream &os, const String &s) {
  os << "str: " << s._pstr;
  return os;
}
std::istream &operator>>(std::istream &is, String &s) {
  char buf[1024];
  is >> buf;
  s = buf;
  return is;
}

String operator+(const String &lhs, const String &rhs) {
  String temp(lhs);
  temp += rhs;
  return temp;
}
String operator+(const String &lhs, const char *rhs) {
  String temp(lhs);
  temp += rhs;
  return temp;
}
String operator+(const char *lhs, const String &rhs) {
  String temp(lhs);
  temp += rhs;
  return temp;
}

void test0() {
  String s1;
  String s2("hello");
  cout << "拷贝构造测试 s2: " << s2 << endl;
  s1 = s2;
  cout << "赋值测试 s1: " << s1 << endl;
  String s3(s2);
  cout << "参数构造测试 s3: " << s3 << endl;

  String s4("world");
  s1 = s2 + "," + s4;
  cout << "重载+ " << s1 << endl;

  if (s1 > s2) {
    std::cout << "s1大于s2" << std::endl;
  }

  if (s3 == s2) {
    std::cout << "s3等于s2" << std::endl;
  }
  s2 += s4;
  cout << "s2+=s4: " << s2 << endl;
  cout << "输入:  " << endl;
  String s6;
  cin >> s6;
  cout << "输入测试 s6:" << s6 << endl;
};

int main() {
  test0();

  return 0;
}
