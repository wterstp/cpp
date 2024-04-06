#ifndef __TASKQUEUE_H__
#define __TASKQUEUE_H_
#include "Condition.h"
#include "MutexLock.h"

#include <queue>
using namespace std;

class TaskQueue {
public:
  TaskQueue(size_t quesize);
  ~TaskQueue();

  void push(int &value);
  int pop();

  bool full() const;
  bool empty() const;

private:
  size_t _quesize;
  queue<int> _que;
  MutexLock _mutex;
  Condition _notFull;
  Condition _notempty;
};

#endif
