#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>
using namespace std;

template <class T>
class ObjectPool {
public:
  T* Allocate() {
    T* obj;
    if (!freed.empty()) {
      obj = freed.front(); //pop()
      freed.pop();
    } else {
      obj = new T;
    }
    allocated.insert(obj);
    return obj;
  }

  T* TryAllocate() {
    if (!freed.empty()) {
      Allocate();
    } else {
      return nullptr;
    }
  }

  void Deallocate(T* object){
    if (allocated.count(object) != 0) {
      freed.push(object);
      allocated.erase(object);
    } else {
      throw invalid_argument("Object does not exist");
    }
  }

  ~ObjectPool() {
    while(!freed.empty()) {
      T* obj = freed.front();
      freed.pop(); 
      delete obj;
    }
    for(auto obj : allocated) {
      delete obj;
    }
  }

  void DisplayAllocated() {
    for(auto obj : allocated) {
      cout << *obj << " ";
    } cout << endl;
  }

private:
  set<T*> allocated; 
  queue<T*> freed;
};

void TestObjectPool() {
  ObjectPool<string> pool;

  auto p1 = pool.Allocate();
  auto p2 = pool.Allocate();
  auto p3 = pool.Allocate();

  *p1 = "first";
  *p2 = "second";
  *p3 = "third";

  pool.Deallocate(p2);

  //pool.DisplayAllocated(); // first, third

  ASSERT_EQUAL(*pool.Allocate(), "second");

  //pool.DisplayAllocated(); // first, second, third

  pool.Deallocate(p3);

  //pool.DisplayAllocated(); // first, second

  pool.Deallocate(p1);

  //pool.DisplayAllocated(); //second

  ASSERT_EQUAL(*pool.Allocate(), "third");

  ASSERT_EQUAL(*pool.Allocate(), "first");

  pool.Deallocate(p1);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestObjectPool);
  return 0;
}
