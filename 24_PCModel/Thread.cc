#include "Thread.h"
#include <iostream>
#include <stdio.h>
using namespace std;

Thread::Thread() : _thid(0), _isRunning(false) {}

Thread::~Thread() {}

void Thread::start() {
  int ret = pthread_create(&_thid, nullptr, threadFunc, this);
  if (ret) {
    perror("pthread_create");
    return;
  }
  _isRunning = true;
}

void Thread::stop() {
  if (_isRunning) {
    int ret = pthread_join(_thid, nullptr);
    if (ret) {
      perror("pthread_join");
      return;
    }
    _isRunning = false;
  }
}

void *Thread::threadFunc(void *arg) {
  Thread *pth = static_cast<Thread *>(arg);
  if (pth) {
    pth->run();
  } else {
    cout << "nullptr==pth" << endl;
  }
  pthread_exit(nullptr);
}
