#pragma once

#include <istream>
#include <ostream>
#include <set>
#include <vector>
#include <map>
#include <string>
using namespace std;

struct DocWordStat {
    DocWordStat(size_t did, size_t ht) : 
    docid_(did), 
    hitcount_(ht) 
    {}

    size_t get_docid() const {
      return docid_;
    }

     size_t get_hitcount() const {
      return hitcount_;
    }

    void increase_hitcount() {
      hitcount_++;
    }
  private: 
    size_t docid_;
    size_t hitcount_;
};

class InvertedIndex {
public:

  void Add(const string& document);
  //void Add(string_view document);

  const vector<DocWordStat>& Lookup(const string& word) const;
  //const vector<DocWordStat>& Lookup(string_view word) const;

  size_t Index() {
    return docs > 0 ? docs-1 : docs; 
  }

private:
  //map<string_view, vector<DocWordStat>> index;
  map<string, vector<DocWordStat>> index;
  //vector<DocWordStat> empty_;
  size_t docs=0;
};

class SearchServer {
public:
  SearchServer() = default;
  explicit SearchServer(istream& document_input);
  void UpdateDocumentBase(istream& document_input);
  void AddQueriesStream(istream& query_input, ostream& search_results_output);

private:
  InvertedIndex index;
};
