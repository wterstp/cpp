#include <functional>
#include <iostream>
#include <memory>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
using namespace std;
using func = function<void()>;
class Thread {
public:
  Thread(func &&cb) : thid(0), is_runing(false), _cb(std::move(cb)){};
  ~Thread() {
    if (is_runing) {
      pthread_join(thid, nullptr);
    }
    cout << "~Thread" << endl;
  }

  void start() {
    int ret = pthread_create(&thid, nullptr, threadFunc, this);
    if (ret) {
      perror("pthread_create");
      return;
    }
    is_runing = true;
  };
  void stop() {
    if (is_runing) {
      int ret = pthread_join(thid, nullptr);
      if (ret) {
        perror("pthraed_join");
        return;
      }
      is_runing = false;
    }
  };

private:
  static void *threadFunc(void *arg) {
    Thread *td = static_cast<Thread *>(arg);
    td->_cb();
    return nullptr;
  }

private:
  pthread_t thid;
  bool is_runing;
  function<void()> _cb;
};

void fun(int x) {
  int ct = 0;
  while (ct < 3) {
    cout << "线程" << x << endl;
    sleep(1);
    ct++;
  }
}

class Mytask {
public:
  void process(int x) {
    int ct = 0;
    while (ct < 3) {
      cout << "线程" << x << endl;
      sleep(1);
      ct++;
    }
  }
};

void test0() {
  Thread t(move(bind(fun, 5)));
  t.start();
  t.stop();
};

void test1() {
  Mytask t;
  Thread td(move(bind(&Mytask::process, &t, 3)));
  td.start();
  td.stop();
}
int main() {
  test1();

  return 0;
}
