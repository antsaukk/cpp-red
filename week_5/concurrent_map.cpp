#include "test_runner.h"
#include "profile.h"

#include <algorithm>
#include <iostream>
#include <future>
#include <map>
#include <vector>
#include <string>
#include <random>
#include <mutex>

using namespace std;

template <typename T>
T Abs(T x) {
  return x < 0 ? -x : x;
}

template <typename K, typename V>
class ConcurrentMap {
public:
  static_assert(is_integral_v<K>, "ConcurrentMap supports only integer keys");

  struct Access {
    //constuctor has receives the reference map and only then locks mutex for value
    //if constructor reveives the value straight away and then lock mutex, race condition happens
    Access(const K& key, mutex& m, map<K,V>& MP) : 
    guard(m), 
    ref_to_value(MP[key]) 
    {}

    lock_guard<mutex> guard;
    V& ref_to_value;
  };

  explicit ConcurrentMap(size_t bucket_count) : 
  buckets(bucket_count), 
  map_collection(bucket_count),
  mutexes(bucket_count) {}

  Access operator[](const K& key) {
    size_t indx = Abs(key) % buckets;
    return {key, mutexes[indx], map_collection[indx]}; //copy elision 
  }

  map<K, V> BuildOrdinaryMap() {
    map<K, V> result;
    for(size_t i = 0; i < buckets; i++){
      lock_guard<mutex> lock_guard(mutexes[i]);
      result.merge(map_collection[i]);
    }
    return result;
  }

private:
  size_t buckets;
  vector<map<K,V>> map_collection;
  vector<mutex> mutexes;
};

void RunConcurrentUpdates(ConcurrentMap<int, int>& cm, size_t thread_count, int key_count) {
  auto kernel = [&cm, key_count](int seed) {
    vector<int> updates(key_count);
    iota(begin(updates), end(updates), -key_count / 2);
    shuffle(begin(updates), end(updates), default_random_engine(seed));

    for (int i = 0; i < 2; ++i) {
      for (auto key : updates) {
        cm[key].ref_to_value++;
      }
    }
  };

  vector<future<void>> futures;
  for (size_t i = 0; i < thread_count; ++i) {
    futures.push_back(async(kernel, i));
  }
}

void TestConcurrentUpdate() {
  const size_t thread_count = 3;
  const size_t key_count = 50000;

  ConcurrentMap<int, int> cm(thread_count);
  RunConcurrentUpdates(cm, thread_count, key_count);

  const auto result = cm.BuildOrdinaryMap();
  ASSERT_EQUAL(result.size(), key_count);
  for (auto& [k, v] : result) {
    AssertEqual(v, 6, "Key = " + to_string(k));
  }
}

void TestReadAndWrite() {
  ConcurrentMap<size_t, string> cm(5);

  auto updater = [&cm] {
    for (size_t i = 0; i < 50000; ++i) {
      cm[i].ref_to_value += 'a';
    }
  };
  auto reader = [&cm] {
    vector<string> result(50000);
    for (size_t i = 0; i < result.size(); ++i) {
      result[i] = cm[i].ref_to_value;
    }
    return result;
  };

  auto u1 = async(updater);
  auto r1 = async(reader);
  auto u2 = async(updater);
  auto r2 = async(reader);

  u1.get();
  u2.get();

  for (auto f : {&r1, &r2}) {
    auto result = f->get();
    ASSERT(all_of(result.begin(), result.end(), [](const string& s) {
      return s.empty() || s == "a" || s == "aa";
    }));
  }
}

void TestSpeedup() {
  {
    ConcurrentMap<int, int> single_lock(1);

    LOG_DURATION("Single lock");
    RunConcurrentUpdates(single_lock, 4, 50000);
  }
  {
    ConcurrentMap<int, int> many_locks(100);

    LOG_DURATION("100 locks");
    RunConcurrentUpdates(many_locks, 4, 50000);
  }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestConcurrentUpdate);
  RUN_TEST(tr, TestReadAndWrite);
  RUN_TEST(tr, TestSpeedup);
}
