#include "Consumer.h"
#include "Producer.h"
#include "TaskQueue.h"
#include "Thread.h"
#include <iostream>
#include <memory>
using namespace std;

void test0() {
  TaskQueue taskQue(10);

  unique_ptr<Thread> pro(new Producer(taskQue));
  unique_ptr<Thread> con(new Consumer(taskQue));

  pro->start();
  con->start();

  pro->stop();
  con->stop();
}
int main() {
  test0();

  return 0;
}
