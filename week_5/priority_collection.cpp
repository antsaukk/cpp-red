#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <queue>
#include <map>
#include <set>
#include <cmath>
#include <utility>
#include <vector>

using namespace std;

template <typename T>
class PriorityCollection {
public:
  using Id = size_t;/* identificator type */

  PriorityCollection() : maxVal(make_pair(0,0u)) {} //def constr


  struct PriorityComparator {
    bool operator()(const pair<int, int>& a, const pair<int, int>& b) const {
        if (a.first == b.first)
          return a.second < b.second;
        else
          return a.first < b.first;
    }
  };

  void UpdateMax(Id id, int rating) {
    if (maxVal.first < rating || (maxVal.first == rating && maxVal.second < id)) {
      maxVal.first = rating; 
      maxVal.second = id;
    } 
  }

  void ClearMax() {
    maxVal.first = 0; 
    maxVal.second = 0u;
  }

  void NewMax() {
    ClearMax();
    //Display();
    /*for (auto iter = priority.begin(); iter != priority.end(); ++iter) {
        UpdateMax(iter->second, iter->first);
    }*/
    if (priority.empty()) return;
    auto submax = prev(priority.end());
    UpdateMax(submax->second, submax->first);
  }

  // Add object with zero priority
  // using move semantics and return id
  Id Add(T object) {
    Id newId = collection.size(); //generate id

    collection.push_back(move(object)); //move obect into collection
    status.push_back(ALIVE); //update status
    priority.insert((make_pair(0, newId))); //update set

    UpdateMax(newId, ZEROP);

    return newId;
  }

  // Добавить все элементы диапазона [range_begin, range_end)
  // с помощью перемещения, записав выданные им идентификаторы
  // в диапазон [ids_begin, ...)
  template <typename ObjInputIt, typename IdOutputIt>
  void Add(ObjInputIt range_begin, ObjInputIt range_end, IdOutputIt ids_begin) {
    for(auto it = range_begin; it != range_end; it++) {
      Id nId = Add(move(*it));
      *ids_begin = nId;
      ids_begin++;
    }
  }

  // Определить, принадлежит ли идентификатор какому-либо
  // хранящемуся в контейнере объекту
  bool IsValid(Id id) const {
    return id > (status.size()-1) ? 0 : (status[id] > (-1));
  }

  // Получить объект по идентификатору
  const T& Get(Id id) const {
    return collection[id];
  }

  // Увеличить приоритет объекта на 1
  void Promote(Id id) {
    priority.erase(make_pair(status[id], id));
    priority.insert(make_pair(++status[id], id));
    UpdateMax(id, status[id]);
  }

  // Получить объект с максимальным приоритетом и его приоритет
  pair<const T&, int> GetMax() const {
    return {collection[maxVal.second], maxVal.first};
  }

  // Аналогично GetMax, но удаляет элемент из контейнера
  pair<T, int> PopMax() {
    pair<int, Id> current_max = make_pair(maxVal.first, maxVal.second);

    //cout << "DELETING: " << collection[current_max.second] << endl;

    priority.erase(prev(priority.end())); // erase last element
    status[current_max.second] = DEAD;

    NewMax();

    return make_pair(move(collection[current_max.second]), current_max.first);
  }

  void Display(){
    for(const auto& it: priority)
      cout << collection[it.second] << ": " << it.first << "\n";
  }

private:
  // Приватные поля и методы
  int ALIVE = 0;
  int DEAD = -1;
  int ZEROP = 0;

  //map<int, int> priority; // first index is identificator of max and second is priority

  set<pair<int, Id>, PriorityComparator> priority; // first index is identificator of max and second is priority

  vector<T> collection; //identificator points to the respective index of element in the array
  vector<int> status; //1 object with index id is alive 0 otherwise

  pair<int, Id> maxVal;  //store identificator and max value to fetch it quickly
};


class StringNonCopyable : public string {
public:
  using string::string;  // Позволяет использовать конструкторы строки
  StringNonCopyable(const StringNonCopyable&) = delete;
  StringNonCopyable(StringNonCopyable&&) = default;
  StringNonCopyable& operator=(const StringNonCopyable&) = delete;
  StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
  PriorityCollection<StringNonCopyable> strings;
  const auto white_id = strings.Add("white");
  const auto yellow_id = strings.Add("yellow");
  const auto red_id = strings.Add("red");

  strings.Promote(yellow_id);
  for (int i = 0; i < 2; ++i) {
    strings.Promote(red_id);
  }
  strings.Promote(yellow_id);
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "red");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "yellow");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "white");
    ASSERT_EQUAL(item.second, 0);
  }

  cout << white_id << endl;
}

void TestOrder() {
  PriorityCollection<StringNonCopyable> strings;
  const auto white_id = strings.Add("white");
  const auto yellow_id = strings.Add("yellow");
  const auto red_id = strings.Add("red");

  cout << "\n############### T E S T ###############\n\n";

  for (int i = 0; i < 2; ++i) {
    strings.Promote(red_id);
  }
  for (int i = 0; i < 4; ++i) {
    strings.Promote(white_id);
  }
  for (int i = 0; i < 6; ++i) {
    strings.Promote(yellow_id);
  }

  strings.Display();

  cout << "\n############### E N D ################\n";

  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "yellow");
    ASSERT_EQUAL(item.second, 6);
  }

  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "white");
    ASSERT_EQUAL(item.second, 4);
  }

  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "red");
    ASSERT_EQUAL(item.second, 2);
  }
  
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestNoCopy);
  RUN_TEST(tr, TestOrder);
  return 0;
}
