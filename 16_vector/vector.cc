#include <iostream>
#include <vector>

using namespace std;

void display(vector<int> &arr) {
  if (arr.size() != 0) {
    for (auto &num : arr)
      cout << num << " ";
  }
  cout << endl;
}

void test0() {
  vector<int> numbers(10);
  display(numbers);

  vector<int> num2(10, 5);
  display(num2);
  int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
  vector<int> num3(arr, arr + 10);
  display(num3);
  vector<int> num4{0, 9, 8, 7, 6, 5, 4, 3, 2, 1};
  display(num4);
};

int main() {
  test0();

  return 0;
}
