#include <iostream>
#include <vector>
using namespace std;
template <typename T, typename Compare = std::less<T>> class HeapSort {
public:
  HeapSort(T *arr, size_t size) : _vec(arr, arr + size) {
    size_t last_idx = (size - 2) >> 1;
    for (int i = last_idx; i >= 0; i--) {
      heapAdjust(size, i);
    }
  }
  void heapAdjust(size_t len, size_t idx) {
    while (1) {
      size_t max_idx = idx;

      size_t left_idx = (idx << 1) + 1;
      size_t right_idx = (idx << 1) + 2;
      if (left_idx < len && _cmp(_vec[max_idx], _vec[left_idx])) {
        max_idx = left_idx;
      }

      if (right_idx < len && _cmp(_vec[max_idx], _vec[right_idx])) {
        max_idx = right_idx;
      }
      if (max_idx != idx) {
        swap(_vec[max_idx], _vec[idx]);
        idx = max_idx;
      } else {
        break;
      }
    }
  }
  void sort() {
    size_t heap_len = _vec.size();
    while (heap_len > 1) {
      swap(_vec[0], _vec[heap_len - 1]);
      --heap_len;

      heapAdjust(heap_len, 0);
    }
  }
  void print() {
    if (!_vec.empty()) {
      for (const T &num : _vec) {
        cout << num << " ";
      }
      cout << endl;
    }
  };

private:
  vector<T> _vec;
  Compare _cmp;
};
void test0() {
  int arr[5] = {4, 10, 3, 5, 1};
  HeapSort<int> heap(arr, sizeof(arr) / sizeof(int));
  heap.print();
  heap.sort();
  heap.print();
};

int main() {
  test0();

  return 0;
}
