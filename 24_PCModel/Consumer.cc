#include "Consumer.h"
#include "TaskQueue.h"
#include <iostream>
#include <unistd.h>
using namespace std;
Consumer::Consumer(TaskQueue &taskQue) : _taskQue(taskQue) {}
Consumer::~Consumer() {}
void Consumer::run() {
  int cnt = 20;
  while (cnt--) {
    int number = _taskQue.pop();
    cout << "Consumer:" << number << endl;
    sleep(1);
  }
}
