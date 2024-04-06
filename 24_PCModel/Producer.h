#ifndef __PRODUCER_H__
#define __PRODUCER_H_

#include "Thread.h"

class TaskQueue;

class Producer : public Thread {
public:
  Producer(TaskQueue &taskQue);
  ~Producer();

  void run() override;

private:
  TaskQueue &_taskQue;
};

#endif
