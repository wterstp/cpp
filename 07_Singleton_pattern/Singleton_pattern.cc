#include <cstdlib>
#include <iostream>
#include <pthread.h>
using std::cin;
using std::cout;
using std::endl;
using std::string;

class Singleton {
  Singleton(int x, int y) : _ix(x), _iy(y) {
    cout << "Singleton(int,int)" << endl;
  }

  ~Singleton() { cout << "~Singleton()" << endl; }
  Singleton(const Singleton &rhs) = delete;
  Singleton &operator=(const Singleton &rhs) = delete;

  // class AutoRelease {
  // public:
  //   AutoRelease() { cout << "AutoRelease()" << endl; }
  //   ~AutoRelease() {
  //     cout << "AutoRelease()" << endl;
  //     if (pInstance) {
  //       delete pInstance;
  //       pInstance = nullptr;
  //     }
  //   }
  // };
  static void destroy() {
    if (pInstance) {
      delete pInstance;
      pInstance = nullptr;
    }
  }

public:
  static Singleton *getInstance() {
    pthread_once(&_once, init_r);

    return pInstance;
  }

  static void init_r() {
    pInstance = new Singleton(0, 0);
    atexit(destroy);
  }

  // static Singleton *getInstance() {
  //   if (pInstance == nullptr) {
  //     pInstance = new Singleton(1, 2);
  //   }
  //   return pInstance;
  // }

  void init(int x, int y) {
    _ix = x;
    _iy = y;
  }

  void print() {
    cout << "ix= " << _ix << "\n"
         << "iy= " << _iy << endl;
  }

private:
  int _ix;
  int _iy;
  static Singleton *pInstance;
  // static AutoRelease _ar;
  static pthread_once_t _once;
};
Singleton *Singleton::pInstance = getInstance();
// Singleton ::AutoRelease Singleton::_ar;
pthread_once_t Singleton::_once = PTHREAD_ONCE_INIT;

void test0() {
  Singleton::getInstance()->print();
  Singleton::getInstance()->init(3, 5);
  Singleton::getInstance()->print();
};

int main() {

  test0();
  return 0;
}
