#include <iostream>
#include <vector>
using namespace std;
template <typename T, typename Compare = std::less<T>> class MyQsort {
public:
  MyQsort(T *arr, size_t size, Compare com = std::less<T>())
      : _vec(arr, arr + size) {
    quick(0, size - 1, com);
  }
  void quick(int left, int right, Compare &com) {
    if (left >= right) {
      return;
    }
    int idx = partition(left, right, com);
    quick(left, idx - 1, com);
    quick(idx + 1, right, com);
  }
  int partition(int left, int right, Compare &com) {
    T pivot = _vec[left];
    int low = left;
    int high = right;
    while (low < high) {
      while (low < high && _vec[high] >= pivot) {
        --high;
      }
      _vec[low] = _vec[high];
      while (low < high && _vec[low] < pivot) {
        ++low;
      }
      _vec[high] = _vec[low];
    }

    _vec[low] = pivot;
    return low;
  }
  void print() {
    if (!_vec.empty()) {
      for (auto &num : _vec) {
        cout << num << " ";
      }
      cout << endl;
    }
  }

private:
  vector<T> _vec;
};
void test0() {
  int arr[5] = {2, 5, 1, 3, 4};
  size_t sz = sizeof(arr) / sizeof(arr[0]);
  MyQsort<int> vec(arr, sz);
  vec.print();
};

int main() {
  test0();

  return 0;
}
