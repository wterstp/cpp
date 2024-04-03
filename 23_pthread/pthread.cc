#include <iostream>
#include <memory>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
using namespace std;
class Thread {
public:
  Thread() : thid(0){};
  virtual ~Thread() { cout << "~Thread" << endl; }

  void start() {
    int ret = pthread_create(&thid, nullptr, threadFunc, this);
    if (ret) {
      perror("pthread_create");
      return;
    }
    is_runing = true;
  };
  void stop() {
    int ret = pthread_join(thid, nullptr);
    if (ret) {
      perror("pthraed_join");
      return;
    }
    is_runing = false;
  };

private:
  static void *threadFunc(void *arg);
  virtual void run() = 0;

private:
  pthread_t thid;
  bool is_runing;
};
void *Thread::threadFunc(void *arg) {
  Thread *pth = static_cast<Thread *>(arg);
  if (pth) {
    pth->run();
  } else {
    cout << "nullptr==pth" << endl;
  }
  return nullptr;
}

class MyThread : public Thread {
public:
  void run() override {
    int num = 0;
    while (num < 5) {
      cout << "MyThread is running!!!" << endl;
      sleep(1);
      num++;
    }
  }

  ~MyThread() { cout << "~MyThread()" << endl; }
};

void test0() {
  MyThread myThread;
  myThread.start();
  myThread.stop();
};
void test1() {
  unique_ptr<Thread> myThread(new MyThread);
  myThread->start();
  myThread->stop();
}

int main() {
  test1();

  return 0;
}
