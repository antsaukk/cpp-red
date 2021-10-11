#include "test_runner.h"
#include "profile.h"

#include <map>
#include <string>
#include <algorithm>
#include <iterator>
#include <future>
#include <functional>
#include <utility>
using namespace std;

struct Stats {
  map<string, int> word_frequences;

  void operator += (const Stats& other) {
    for(const auto& word : other.word_frequences) {
      word_frequences[word.first]+=word.second;
    }
  }
};

Stats ExploreKeyWordsSingleThread(const set<string>& key_words, vector<string>& words) {
  Stats result;
  for(const auto& str : words){
    if (key_words.count(str) != 0) {
      result.word_frequences[str]++;
    }
  }
  return result;
}

void DisplayRange(size_t start, size_t end) {
  cout << "[" << start << ", " << end << ")\n";
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
  vector<string> words;
  string s; 
  while (input >> s) words.push_back(s);

  constexpr size_t cores = 8;
  size_t N = words.size();
  size_t workload = N > cores ? (N / cores) + 1 : N;

  vector<vector<string>> spl_words;

  for (size_t start = 0; start < N; start += workload) {
    size_t end = min(N, start + workload);
    vector<string> temp(end-start);
    for(size_t i = start; i < end; i++) {
      temp.push_back(words[i]); 
    }
    spl_words.push_back(temp); 
  }

  vector<future<Stats>> futures; 
  for (auto& range : spl_words) {
    futures.push_back(async(ExploreKeyWordsSingleThread, ref(key_words), ref(range)));
  }

  Stats result;
  for (auto& f : futures) {
    result += f.get();
  }
  return result; 
}

void TestBasic() {
  const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

  stringstream ss;
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

  const auto stats = ExploreKeyWords(key_words, ss);
  const map<string, int> expected = {
    {"yangle", 6},
    {"rocks", 2},
    {"sucks", 1}
  };
  ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestBasic);
}
