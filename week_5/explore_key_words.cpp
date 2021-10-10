#include "test_runner.h"
#include "profile.h"

#include <map>
#include <string>
#include <algorithm>
#include <future>
#include <functional>
#include <utility>
using namespace std;

template <typename Iterator>
class IteratorRange {
public:
  IteratorRange(Iterator begin, Iterator end)
    : first(begin)
    , last(end)
    , size_(distance(first, last))
  {
  }

  Iterator begin() const {
    return first;
  }

  Iterator end() const {
    return last;
  }

  size_t size() const {
    return size_;
  }

private:
  Iterator first, last;
  size_t size_;
};

template <typename Iterator>
class Paginator {
private:
  vector<IteratorRange<Iterator>> pages;

public:
  Paginator(Iterator begin, Iterator end, size_t page_size) {
    for (size_t left = distance(begin, end); left > 0; ) {
      size_t current_page_size = min(page_size, left);
      Iterator current_page_end = next(begin, current_page_size);
      pages.push_back({begin, current_page_end});

      left -= current_page_size;
      begin = current_page_end;
    }
  }

  auto begin() const {
    return pages.begin();
  }

  auto end() const {
    return pages.end();
  }

  size_t size() const {
    return pages.size();
  }
};

template <typename C>
auto Paginate(C& c, size_t page_size) {
  return Paginator(begin(c), end(c), page_size);
}

struct Stats {
  map<string, int> word_frequences;

  void operator += (const Stats& other) {
    for(const auto& word : other.word_frequences) {
      word_frequences[word.first]+=word.second;
    }
  }
};

Stats ExploreKeyWordsSingleThread(const set<string>& key_words, vector<string>& range) {
  Stats result;
  for(const auto& str : range){
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

  /*for (auto w : words) 
    cout << w << " " << endl;*/ 

  constexpr size_t cores = 6;
  size_t N = words.size();
  size_t workload = N > cores ? (N / cores) + 1 : N;

  vector<future<Stats>> futures; 
  auto pages = Paginate(words, workload);

  for (vector<string>& range : pages) {
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
