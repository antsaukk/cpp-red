#pragma once

#include <cstdlib>
#include <utility>
#include <memory>
#include <algorithm>

template <typename T>
class SimpleVector {
public:
  SimpleVector() : //default constructor
  data(nullptr),
  capacity_(0),
  size_(0)
  {}
  
  explicit SimpleVector(size_t size) : //non-empty constructor
    data(new T[size]),
    capacity_(size),
    size_(size) 
    {}

  SimpleVector(const SimpleVector<T>& other) : //copy constructor
    data(new T[other.capacity_]),
    capacity_(other.capacity_),
    size_(other.size_) 
    {
      std::copy(other.begin(), other.end(), begin()); //subtitute by own analogy
    }

  void operator=(const SimpleVector& other) { //copy assignment 
    if (other.size_ <= capacity_) {
      std::copy(other.begin(), other.end(), begin());
      size_ = other.size_;
    } else {
      SimpleVector<T> temp(other);
      std::swap(temp.data, data); //subtitute by own analogy
      std::swap(temp.size_, size_); //subtitute by own analogy
      std::swap(temp.capacity_, capacity_); //subtitute by own analogy
    } 
  }


  ~SimpleVector() { delete[] data; }

  T& operator[](size_t index) { return *(data + index); }

  T* begin() { return data; }
  T* end() { return data + size_; }

  const T* begin() const { return data; }
  const T* end() const { return data + size_; }

  size_t Size() const { return size_; }

  size_t Capacity() const { return capacity_; }

  void PushBack(const T& value) {
    if (!Capacity()) {
      data = new T[SINGLE_EXP]; 
      *data = value;
      capacity_++;
      size_++;
    } else if (Size() == Capacity()) {
      size_t size_temp = Capacity() * DOUBLING_CONST;
      T* data_temp = new T[size_temp];
      for (size_t i = 0; i < Size(); i++) {
        *(data_temp + i) = *(data + i);
      }
      delete[] data;
      data = data_temp;
      capacity_ = size_temp;

      *(data + capacity_ / DOUBLING_CONST) = value; 
      size_++;
    } else {
      *(data + Size()) = value;
      size_++;
    } 
  }

private:
  static const int SINGLE_EXP = 1;
  static const int DOUBLING_CONST = 2;

  T* data;
  size_t capacity_;
  size_t size_;
};
