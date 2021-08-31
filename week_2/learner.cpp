#include "test_runner.h"
#include "profile.h"

#include <algorithm>
#include <string>
#include <vector>
#include <set>

using namespace std;

class Learner {
 private:
  set<string> setdict;

 public:
  int Learn(const vector<string>& words) {
    int newWords = 0;
    for (const auto& word : words) {
      if (setdict.insert(word).second) { ++newWords; }
    }
    return newWords;
  }

  vector<string> KnownWords() const {
    return {setdict.begin(), setdict.end()};
  }
};

int main() {
  Learner learner;
  string line;
  while (getline(cin, line)) {
    vector<string> words;
    stringstream ss(line);
    string word;
    while (ss >> word) {
      words.push_back(word);
    }
    cout << learner.Learn(words) << "\n";
  }
  cout << "=== known words ===\n";
  for (auto word : learner.KnownWords()) {
    cout << word << "\n";
  }
}