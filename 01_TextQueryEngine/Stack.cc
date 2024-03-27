#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::string;

class Stack {
public:
  bool empty(); //判空

  void push(int value); //元素入栈
  void pop();           //元素出栈
  int top();            //获取栈顶元素

  Stack() : stackTop(nullptr) {} //构造函数

private:
  struct Node {
    int data;
    Node *next;
    Node(int data, Node *next = nullptr) : data(data), next(next) {}
  };

  Node *stackTop;
};

bool Stack::empty() { return stackTop == nullptr; }

void Stack::push(int value) {
  Node *newnode = new Node(value, stackTop);
  stackTop = newnode;
}

void Stack::pop() {
  if (!empty()) {
    Node *temp = stackTop;
    stackTop = stackTop->next;
    delete temp;

  } else {
    std::cerr << "Stack is empty" << endl;
  }
}

int Stack::top() {
  if (!empty()) {
    return stackTop->data;
  } else {
    std::cerr << "Stack is empty" << endl;
    return -1;
  }
}

void test0() {
  Stack s;
  s.push(3);
  int top = s.top();
  cout << "top=" << top << endl;
  s.pop();
  s.top();
};

int main() {
  test0();

  return 0;
}
