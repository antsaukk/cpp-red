#include "test_runner.h"
#include <vector>
#include <numeric>
#include <cmath>
#include <future>
#include <cstdint>
using namespace std;

int64_t SumSingleThread(const vector<vector<int>>& matrix, size_t start, size_t end) {
  int64_t sum = 0; 
  for(size_t i = start; i < end; i++)
    sum += accumulate(matrix[i].begin(), matrix[i].end(), 0);
  return sum;
}


int64_t ComputeResult(vector<future<int64_t>>& futures) {
  int64_t result = 0;
  for(auto& f : futures)
    result += f.get();
  return result;
}

void DisplayRange(size_t start, size_t end) {
  cout << "[" << start << ", " << end << ")\n";
}

int64_t CalculateMatrixSum(const vector<vector<int>>& matrix) {
  constexpr size_t cores = 4;
  size_t N = matrix.size();
  int step = N > cores ? (N / cores) + 1 : N;

  vector<future<int64_t>> futures;
  for(size_t start = 0; start < N; start+=step){
    size_t end = min(start+step, N);
    futures.push_back(async([matrix, start, end]{ return SumSingleThread(matrix, start, end); }));
  }

  return ComputeResult(futures);
}

void TestCalculateMatrixSum() {
  const vector<vector<int>> matrix = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12},
    {13, 14, 15, 16}
  };
  ASSERT_EQUAL(CalculateMatrixSum(matrix), 136);
}

void TestNonDivisible() {
  const vector<vector<int>> matrix = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},

    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},

    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
  };
  ASSERT_EQUAL(CalculateMatrixSum(matrix), 121);
}

void TestNonDivisible2() {
  const vector<vector<int>> matrix = {
    {1, 1, 1},
    {1, 1, 1},
    {1, 1, 1}
  };
  ASSERT_EQUAL(CalculateMatrixSum(matrix), 9);
}

void TestNonDivisible3() {
  const vector<vector<int>> matrix = {
    {1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1}
  };
  ASSERT_EQUAL(CalculateMatrixSum(matrix), 25);
}

void TestNonDivisible4() {
  const vector<vector<int>> matrix = {
    {1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1}
  };
  ASSERT_EQUAL(CalculateMatrixSum(matrix), 36);
}

void TestNonDivisible5() {
  const vector<vector<int>> matrix = {
    {1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1}
  };
  ASSERT_EQUAL(CalculateMatrixSum(matrix), 49);
}

void TestNonDivisible6() {
  size_t N = 1000; 
  vector<vector<int>> matrix; 
  for(size_t i = 0; i < N; i++){
    vector<int> row(N);
    for(size_t j = 0; j < N; j++){
      row[j] = 1; 
    }
    matrix.push_back(row);
  }
  ASSERT_EQUAL(CalculateMatrixSum(matrix), N*N);
}

void TestNonDivisible7() {
  size_t N = 1001; 
  vector<vector<int>> matrix; 
  for(size_t i = 0; i < N; i++){
    vector<int> row(N);
    for(size_t j = 0; j < N; j++){
      row[j] = 1; 
    }
    matrix.push_back(row);
  }
  ASSERT_EQUAL(CalculateMatrixSum(matrix), N*N);
}

void TestNonDivisible8() {
  size_t N = 9999; 
  vector<vector<int>> matrix; 
  for(size_t i = 0; i < N; i++){
    vector<int> row(N);
    for(size_t j = 0; j < N; j++){
      row[j] = 1; 
    }
    matrix.push_back(row);
  }
  ASSERT_EQUAL(CalculateMatrixSum(matrix), N*N);
}

void TestNonDivisible9() {
  const vector<vector<int>> matrix = {
    {1, 1},
    {1, 1}
  };
  ASSERT_EQUAL(CalculateMatrixSum(matrix), 4);
}

void TestNonDivisible10() {
  const vector<vector<int>> matrix = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},

    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},

    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
  };
  ASSERT_EQUAL(CalculateMatrixSum(matrix), 100);
}

void TestNonDivisible11() {
  size_t N = 8235; 
  vector<vector<int>> matrix; 
  for(size_t i = 0; i < N; i++){
    vector<int> row(N);
    for(size_t j = 0; j < N; j++){
      row[j] = 2; 
    }
    matrix.push_back(row);
  }
  ASSERT_EQUAL(CalculateMatrixSum(matrix), N*N*2);
}

void TestNonDivisible12() {
  size_t N = 9000; 
  vector<vector<int>> matrix; 
  for(size_t i = 0; i < N; i++){
    vector<int> row(N);
    for(size_t j = 0; j < N; j++){
      row[j] = 3; 
    }
    matrix.push_back(row);
  }
  ASSERT_EQUAL(CalculateMatrixSum(matrix), N*N*3);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestCalculateMatrixSum);
  RUN_TEST(tr, TestNonDivisible);
  RUN_TEST(tr, TestNonDivisible2);
  RUN_TEST(tr, TestNonDivisible3);
  RUN_TEST(tr, TestNonDivisible4);
  RUN_TEST(tr, TestNonDivisible5);
  RUN_TEST(tr, TestNonDivisible6);
  RUN_TEST(tr, TestNonDivisible7);
  RUN_TEST(tr, TestNonDivisible8);
  RUN_TEST(tr, TestNonDivisible9);
  RUN_TEST(tr, TestNonDivisible10);
  RUN_TEST(tr, TestNonDivisible11);
  RUN_TEST(tr, TestNonDivisible12);
  return 0;
}
