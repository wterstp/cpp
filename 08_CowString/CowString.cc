#include <iostream>
#include <string.h>
using std::cout;
using std::endl;
using std::ostream;

class CowString {
public:
  CowString();
  ~CowString();
  CowString(const CowString &rhs);
  CowString(const char *pstr);
  CowString &operator=(const CowString &rhs);

  const char *c_str() const { return _pstr; }
  size_t size() const { return strlen(_pstr); }
  int use_count() { return *(int *)(_pstr - kRefcountLength); }
  friend ostream &operator<<(ostream &os, const CowString &rhs);

  class CharProxy {
  public:
    CharProxy(CowString &str, size_t idx) : _str(str), _idx(idx){};
    CharProxy &operator=(char c) {
      cout << "CharProxy=" << endl;
      if (_idx < _str.size()) {
        if (_str.use_count() > 1) {
          cout << "大于1" << endl;
          char *newStr = _str.malloc(_str._pstr);
          strcpy(newStr, _str._pstr);
          _str.release();
          _str._pstr = newStr;
          _str.initRefcount();
        }
        _str._pstr[_idx] = c;
        return *this;
      }
      return *this;
    }
    friend ostream &operator<<(ostream &os, const CowString::CharProxy &rhs);

  private:
    CowString &_str;
    size_t _idx;
  };

public:
  CharProxy operator[](size_t idx);
  friend ostream &operator<<(ostream &os, const CowString::CharProxy &rhs);

private:
  char *malloc(const char *pstr = nullptr) {
    if (pstr) {
      return new char[strlen(pstr) + kRefcountLength + 1]() + kRefcountLength;
    } else {
      return new char[1 + kRefcountLength]() + kRefcountLength;
    }
  }

  void release() {
    decreaseRefcount();
    if (use_count() == 0) {
      delete[](_pstr - kRefcountLength);
      _pstr = nullptr;
      cout << "delete heap" << endl;
    }
  }

  void initRefcount() { *(int *)(_pstr - kRefcountLength) = 1; }

  void increaseRefcount() { ++*(int *)(_pstr - kRefcountLength); }

  void decreaseRefcount() { --*(int *)(_pstr - kRefcountLength); }

  static const int kRefcountLength = 4;
  char *_pstr;
};

ostream &operator<<(ostream &os, const CowString &rhs) {
  os << rhs._pstr;
  return os;
}

CowString::CowString() : _pstr(malloc()) {
  cout << "CowString()" << endl;
  initRefcount();
}

CowString::~CowString() { release(); }

CowString::CowString(const CowString &rhs)
    : _pstr(rhs._pstr) //浅拷贝
{
  increaseRefcount();
  cout << "CowString(const CowString&)" << endl;
}

CowString::CowString(const char *pstr) : _pstr(malloc(pstr)) {
  cout << "CowString(const char *)" << endl;
  strcpy(_pstr, pstr);
  initRefcount();
}

CowString &CowString::operator=(const CowString &rhs) {
  if (this != &rhs) { // 1.还需要考虑自复制
    cout << "CowString& operator=(const CowString&)" << endl;
    release();         // 2.判断是否回收，原本空间的引用计数-1
    _pstr = rhs._pstr; // 3.浅拷贝
    increaseRefcount();
  }
  return *this;
}

CowString::CharProxy CowString::operator[](size_t idx) {
  return CharProxy(*this, idx);
}

ostream &operator<<(ostream &os, const CowString::CharProxy &rhs) {
  os << rhs._str._pstr;
  return os;
}

void test0() {
  CowString str1;
  cout << str1.use_count() << endl;
  CowString str2 = str1;
  cout << "str1:" << str1 << endl;
  cout << "str2:" << str2 << endl;
  cout << str1.use_count() << endl;
  cout << str2.use_count() << endl;

  cout << "#############" << endl;
  CowString str3("hello");
  cout << "str3:" << str3 << endl;
  cout << str3.use_count() << endl;
  CowString str4 = str3;
  cout << "str3:" << str3 << endl;
  cout << "str4:" << str4 << endl;
  cout << str3.use_count() << endl;
  cout << str4.use_count() << endl;

  cout << "#############" << endl;
  str1 = str3;
  cout << "str1:" << str1 << endl;
  cout << "str2:" << str2 << endl;
  cout << "str3:" << str3 << endl;
  cout << "str4:" << str4 << endl;
  cout << str1.use_count() << endl;
  cout << str2.use_count() << endl;
  cout << str3.use_count() << endl;
  cout << str4.use_count() << endl;

  cout << "#############" << endl;
  cout << str1[0] << endl;
  str1[0] = 'H';
  cout << str3[0] << endl;
  cout << str1[0] << endl;
  cout << str3.use_count() << endl;
  cout << str1.use_count() << endl;

  str1[0] = 'X';
  cout << str1[0] << endl;
  cout << str1.use_count() << endl;
  str1[0] = 'P';
  cout << str1[0] << endl;
  cout << str1.use_count() << endl;
}

int main(void) {
  test0();
  return 0;
}
