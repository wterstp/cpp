#include <algorithm> // 包含std::uninitialized_copy
#include <iostream>
#include <memory> // 包含std::allocator
using namespace std;
template <class T> class Vector {
public:
  Vector() : _start(nullptr), _finish(nullptr), _end(nullptr){};
  ~Vector() {
    cout << "~Vector()" << endl;
    clear();
    _alloc.deallocate(_start, capacity());
  }

  void push_back(const T &value) {
    if (_finish == _end) {
      reallocate();
    }
    _alloc.construct(_finish++, value);
  }

  void pop_back() {
    if (_start != _finish) {
      _alloc.destroy(--_finish);
    }
  }

  int size() const { return _finish - _start; }
  int capacity() const { return _end - _start; }
  // 添加：支持迭代器
  T *begin() const { return _start; }
  T *end() const { return _finish; }

private:
  void clear() {
    while (_finish != _start) {
      _alloc.destroy(--_finish);
    }
  }

  void reallocate() {
    size_t oldSize = size();
    size_t newSize = oldSize ? 2 * oldSize : 1;
    T *newStart = _alloc.allocate(newSize);
    uninitialized_copy(_start, _finish, newStart);
    clear();
    _alloc.deallocate(_start, oldSize);
    _start = newStart;
    _finish = _start + oldSize;
    _end = _start + newSize;
  }

  T *_start;
  T *_finish;
  T *_end;
  static allocator<T> _alloc;
};

template <class T> allocator<T> Vector<T>::_alloc;

void test0() {
  Vector<int> arr;
  arr.push_back(1);
  arr.push_back(2);
  arr.push_back(3);
  arr.push_back(4);
  for (auto &num : arr) {
    cout << num << " ";
  }
  cout << endl;
  arr.pop_back();
  arr.pop_back();
  for (auto &num : arr) {
    cout << num << " ";
  }
  cout << endl;
};

int main() {
  test0();

  return 0;
}
