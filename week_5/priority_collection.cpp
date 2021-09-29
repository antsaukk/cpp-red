#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <map>
#include <cmath>
#include <utility>
#include <vector>

using namespace std;

template <typename T>
class PriorityCollection {
public:
  using Id = size_t;/* identificator type */
  //using MaxNode = pair<int, Id>;/* identificator type */

  PriorityCollection() : maxVal(make_pair(0u,0)) {} //def constr


  void UpdateMax(Id id, int rating) {
    if (maxVal.second < rating || (maxVal.second == rating && maxVal.first < id)) { 
      maxVal.first = id; 
      maxVal.second = rating;
    } 
  }

  void ClearMax() {
    maxVal.first = 0u; 
    maxVal.second = 0;
  }

  void NewMax() {
    ClearMax();
    for (auto iter = priority.begin(); iter != priority.end(); ++iter) {
        UpdateMax(iter->first, iter->second); 
    }
  }

  // Add object with zero priority
  // using move semantics and return id
  Id Add(T object) {
    Id newId = collection.size(); //generate id

    collection.push_back(move(object)); //move obect into collection
    status.push_back(ALIVE); //update status
    priority.insert((make_pair(newId, 0))); //update map

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
    return id > (status.size()-1) ? 0 : status[id];
  }

  // Получить объект по идентификатору
  const T& Get(Id id) const {
    return collection[id];
  }

  // Увеличить приоритет объекта на 1
  void Promote(Id id) {
    UpdateMax(id, ++priority.at(id));
  }

  // Получить объект с максимальным приоритетом и его приоритет
  pair<const T&, int> GetMax() const {
    //return make_pair(collection[maxVal.first], maxVal.second);
    return {collection[maxVal.first], maxVal.second};
  }

  // Аналогично GetMax, но удаляет элемент из контейнера
  pair<T, int> PopMax() {
    pair<Id, int> current_max = make_pair(maxVal.first, maxVal.second);

    priority.erase(current_max.first);
    status[current_max.first] = DEAD;

    NewMax();

    return make_pair(move(collection[current_max.first]), current_max.second);;
  }

private:
  // Приватные поля и методы
  int ALIVE = 1;
  int DEAD = 0;
  int ZEROP = 0;

  map<int, int> priority; // first index is identificator and second is priority
  vector<T> collection; //identificator points to the respective index of element in the array
  vector<int> status; //1 object with index id is alive 0 otherwise

  pair<Id, int> maxVal;  //store identificator and max value to fetch it quickly 
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

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestNoCopy);
  return 0;
}
