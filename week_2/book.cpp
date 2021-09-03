#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>

using namespace std;

class ReadingManager {
public:
  ReadingManager()
      : active_users_(0),
        user_page_counts_(MAX_USER_COUNT_ + 1, 0),
        ratings(MAX_BOOK_COUNT_ + 1, 0) 
        {}

  void Read(int user_id, int page_count) { //O(pages)
    int prev_page_count = 0;
    if (user_page_counts_[user_id] == 0) active_users_++;
    else if (user_page_counts_[user_id] > 0) {
      prev_page_count = user_page_counts_[user_id]; //rating of the page before user read further
      ratings[prev_page_count]--;
    }

    user_page_counts_[user_id] = page_count;
    ratings[page_count]++;
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

    int balance = 0;
    for (int i = 0; i < page_count; i++) { //page_count <= 1000; O(1000)
      balance += ratings[i];
    }
   

    if (balance == 0) {
        return 0;
    }
    return balance * 1.0 / (user_count - 1);
  }

private:
  static const int MAX_USER_COUNT_ = 100'000;
  static const int MAX_BOOK_COUNT_ = 1000;

  int active_users_;

  vector<int> user_page_counts_;
  vector<int> ratings; //ind - page number, val - amount of readers

  int GetUserCount() const {
    return active_users_;
  }
};


int main() {

  ios::sync_with_stdio(false);
  cin.tie(nullptr);

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
    } 
  }
  return 0;
}