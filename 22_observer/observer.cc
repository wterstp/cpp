#include <iostream>
#include <list>
using namespace std;
class Observer {
public:
  virtual void update(int status) = 0;
  virtual string getname() = 0;
};

class Subject {
private:
  list<Observer *> observers;
  int _state;

public:
  void attach(Observer *observer) { observers.push_back(observer); }

  void deattach(Observer *observer) {
    observers.remove(observer);
    cout << observer->getname() << "已退出" << endl;
  }

  void setdate(int state) {
    _state = state;
    notify();
  }

  void notify() {
    for (auto &observer : observers) {
      observer->update(_state);
    }
  }

  int get_state() { return _state; }
};

class ConcreateObserver : public Observer {
private:
  string _name;

public:
  ConcreateObserver(string name) : _name(name){};
  string getname() { return _name; }

  void update(int state) { cout << _name << "已接收:" << state << endl; }
};

void test0() {
  Subject subject;

  ConcreateObserver observer1("1号观察者");
  ConcreateObserver observer2("2号观察者");

  subject.attach(&observer1);
  subject.attach(&observer2);

  subject.setdate(10);

  subject.deattach(&observer1);
  subject.setdate(20);
};

int main() {
  test0();

  return 0;
}
