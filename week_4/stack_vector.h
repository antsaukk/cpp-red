#pragma once

#include <stdexcept>
#include <array>
#include <iostream>
using namespace std;

template <typename T, size_t N>
class StackVector {
public:
  explicit StackVector(size_t a_size = 0) : size_(a_size) {
    if (size_ > N) {
      throw invalid_argument("Size can not exceed capacity.");
    }
  }

  T& operator[](size_t index) { return data_[index]; }
  const T& operator[](size_t index) const { return data_[index]; }

  auto begin() { return data_.begin() ; }
  auto end() { return data_.begin() + size_; }
  const auto begin() const { return data_.begin(); }
  const auto end() const { return data_.begin() + size_; }

  size_t Size() const { return size_; }
  size_t Capacity() const { return N; }

  void PushBack(const T& value) {
    if (Size() == Capacity()) throw overflow_error("StackVector is full."); 
    else {
      data_[Size()] = value; 
      size_++;
    }
  }
  T PopBack(){
    if (Size() == 0) throw underflow_error("StackVector is empty."); 
    else {
      size_--; 
      return data_[Size()];
    }
  }

private:
  array<T, N> data_;
  size_t size_ = 0;
};

