#include "Producer.h"
#include "TaskQueue.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
using namespace std;

Producer::Producer(TaskQueue &taskQue) : _taskQue(taskQue) {}

Producer::~Producer() {}

void Producer::run() {
  srand(clock());
  int cnt = 20;
  while (cnt--) {
    int number = rand() % 100;
    _taskQue.push(number);
    cout << "Producer:" << number << endl;
    sleep(1);
  }
}
