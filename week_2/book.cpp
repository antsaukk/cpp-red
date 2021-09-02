#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>
#include <map>

using namespace std;

//custom comparison by value for std::map
struct compareByValue {
  bool operator()(pair<int,int> &a, pair<int,int> &b) const {
    return a.second > b.second;
  }
};

class ReadingManager {
public:
  ReadingManager()
      : user_page_counts_(MAX_USER_COUNT_ + 1, 0),
        sorted_users_(),
        user_positions_(MAX_USER_COUNT_ + 1, -1), //ratings(),
        ratings(1000, 0) {}

  void Read(int user_id, int page_count) { //O(pages)
    int prev_page_count = 0;
    if (user_page_counts_[user_id] == 0) {
      AddUser(user_id);
    } else {
      prev_page_count = user_page_counts_[user_id]; //rating of the page before user read further
      ratings[prev_page_count]--;
    }
    //ratings[page_count]++;

    user_page_counts_[user_id] = page_count;
    
    int& position = user_positions_[user_id];
    //if position is not zero and current user has read more pages than next user
    //swap their positions 
    while (position > 0 && page_count > user_page_counts_[sorted_users_[position - 1]]) {
      //cout << "position before:" << position << endl;
      SwapUsers(position, position - 1); //N
      //cout << "position after:" << position << endl;
      //PrintV();
    }
    /*int step = 0;
    for(int i = prev_page_count; i < page_count; i++) {
      if (ratings[i] > 0) step += ratings[i];
    }
    cout << step << endl;
    if (position > 0 && page_count > user_page_counts_[sorted_users_[position - step]]) { //bug
      SwapUsers(position, position - step);
    }*/
    ratings[page_count]++;
    //PrintV();
  }

  double Cheer(int user_id) const { //O(1)
    if (user_page_counts_[user_id] == 0) {
      return 0;
    }
    const int user_count = GetUserCount();
    if (user_count == 1) {
      return 1;
    }
    const int page_count = user_page_counts_[user_id];
    /*int position = user_positions_[user_id];
    while (position < user_count && user_page_counts_[sorted_users_[position]] == page_count) {
      //cout << "position before:" << position << endl;
      ++position; //N
      //cout << "position after:" << position << endl;
    }*/

    int balance = 0;
    for (int i = 0; i < page_count; i++) { //page_count <= 1000; O(1000)
      balance += ratings[i];
    }
    /*if (position == user_count) {
        return 0;
    }*/

    if (balance == 0) {
        return 0;
    }
    // По умолчанию деление целочисленное, поэтому
    // нужно привести числитель к типу double.
    // Простой способ сделать это — умножить его на 1.0.
    return balance * 1.0 / (user_count - 1);
  }

  void PrintV() {
    cout << "sorted user IDs: ";
    for (size_t i = 0; i < sorted_users_.size(); i++) {
      cout << sorted_users_[i] << " ";
    }
    cout << endl;
    cout << "user's pages: ";
    for (size_t i = 0; i < sorted_users_.size(); i++) {
      cout << user_page_counts_[sorted_users_[i]] << " ";
    }
    cout << endl;
    cout << "user's positions: ";
    for (size_t i = 0; i < sorted_users_.size(); i++) {
      cout << user_positions_[sorted_users_[i]] << " ";
    }
    cout << endl;
    cout << "page's rating: ";
    for (size_t i = 0; i < ratings.size(); i++) {
      if (ratings[i]!=0) cout << "page: " << i << ", rating: " << ratings[i] << endl;
    }
    cout << endl;
  }



private:
  // Статическое поле не принадлежит какому-то конкретному
  // объекту класса. По сути это глобальная переменная,
  // в данном случае константная.
  // Будь она публичной, к ней можно было бы обратиться снаружи
  // следующим образом: ReadingManager::MAX_USER_COUNT.
  static const int MAX_USER_COUNT_ = 100'000;

  vector<int> user_page_counts_;
  vector<int> sorted_users_;   // отсортированы по убыванию количества страниц
  vector<int> user_positions_; // позиции в векторе sorted_users_
  vector<int> ratings; //ind - page number, val - amount of readers

  int GetUserCount() const {
    return sorted_users_.size();
  }
  void AddUser(int user_id) {
    sorted_users_.push_back(user_id);
    user_positions_[user_id] = sorted_users_.size() - 1;
  }
  void SwapUsers(int lhs_position, int rhs_position) {
    const int lhs_id = sorted_users_[lhs_position];
    const int rhs_id = sorted_users_[rhs_position];
    swap(sorted_users_[lhs_position], sorted_users_[rhs_position]);
    swap(user_positions_[lhs_id], user_positions_[rhs_id]);
  }
};


int main() {
  // Для ускорения чтения данных отключается синхронизация
  // cin и cout с stdio,
  // а также выполняется отвязка cin от cout

  //ios::sync_with_stdio(false);
  //cin.tie(nullptr);

  ReadingManager manager;

  int query_count;
  cin >> query_count;

  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    cin >> query_type;
    int user_id;
    cin >> user_id;

    if (query_type == "READ") {
      int page_count;
      cin >> page_count;
      manager.Read(user_id, page_count);
    } else if (query_type == "CHEER") {
      cout << setprecision(6) << manager.Cheer(user_id) << "\n";
    } else if (query_type == "P") manager.PrintV();
  }

  //manager.PrintM();
  return 0;
}