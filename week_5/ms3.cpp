#include "test_runner.h"
#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
  size_t N = range_end - range_begin;
  if (N < 2) return;
  else {
    vector<typename RandomIt::value_type> elements(make_move_iterator(range_begin), make_move_iterator(range_end));
    size_t partition = N / 3;

    const RandomIt firstMid = elements.begin() + partition;
    const RandomIt secondMid = firstMid + partition;

    MergeSort(elements.begin(), firstMid); 
    MergeSort(firstMid, secondMid);
    MergeSort(secondMid, elements.end());

    vector<typename RandomIt::value_type> temp; 
    merge(make_move_iterator(elements.begin()), make_move_iterator(firstMid), make_move_iterator(firstMid), make_move_iterator(secondMid), back_inserter(temp));
    merge(make_move_iterator(temp.begin()), make_move_iterator(temp.end()), make_move_iterator(secondMid), make_move_iterator(elements.end()), range_begin);
  }
}

void TestIntVector() {
  vector<int> numbers = {6, 1, 3, 9, 1, 9, 8, 12, 1};
  MergeSort(begin(numbers), end(numbers));
  ASSERT(is_sorted(begin(numbers), end(numbers)));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestIntVector);
  return 0;
}
