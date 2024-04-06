#include "TaskQueue.h"

TaskQueue::TaskQueue(size_t quesize)
    : _quesize(quesize), _que(), _mutex(), _notFull(_mutex),
      _notempty(_mutex){};

TaskQueue::~TaskQueue() {}

void TaskQueue::push(int &value) {
  MutexLockGuard autolock(_mutex);
  while (full()) {
    _notFull.wait();
  }
  _que.push(value);
  _notempty.notify();
}

int TaskQueue::pop() {
  MutexLockGuard autolock(_mutex);
  while (empty()) {
    _notempty.wait();
  }
  int tmp = _que.front();
  _que.pop();
  _notFull.notify();

  return tmp;
}

bool TaskQueue::empty() const { return _que.size() == 0; }

bool TaskQueue::full() const { return _que.size() == _quesize; }
