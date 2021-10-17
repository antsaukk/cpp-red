#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>

vector<string> SplitIntoWords(const string& line) {
  istringstream words_input(line);
  return {istream_iterator<string>(words_input), istream_iterator<string>()};
}

SearchServer::SearchServer(istream& document_input) {
  UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
  InvertedIndex new_index;

  for (string current_document; getline(document_input, current_document); ) {
    new_index.Add(move(current_document));
  }

  index = move(new_index);
}

void SearchServer::AddQueriesStream(istream& query_input, ostream& search_results_output) {
  vector<pair<size_t, size_t>> docid_count(50'000);

  for (string current_query; getline(query_input, current_query); ) { //can iterate over 500 000
    const auto words = SplitIntoWords(current_query); // 1 <= sizeof(words) <= 10

    for (const auto& word : words) { // O(size of the query) <= 10
      auto indx_lookup = index.Lookup(word); //O(Log(index size))
      for (const size_t docid : indx_lookup) { //O(size of list inside map index) <= 50 000
        docid_count[docid] = {docid, ++docid_count[docid].second};
      }
    }

    partial_sort(begin(docid_count), begin(docid_count)+5, end(docid_count),
      [](pair<size_t, size_t> lhs, pair<size_t, size_t> rhs) {
        int64_t lhs_docid = lhs.first;
        auto lhs_hit_count = lhs.second;
        int64_t rhs_docid = rhs.first;
        auto rhs_hit_count = rhs.second;
        return make_pair(lhs_hit_count, -lhs_docid) > make_pair(rhs_hit_count, -rhs_docid);
      }
    );

    auto head = Head(docid_count, 5);

    search_results_output << current_query << ':';
    for (auto [docid, hitcount] : head) {
      if (hitcount == 0) break; //mine line
      search_results_output << " {"
        << "docid: " << docid << ", "
        << "hitcount: " << hitcount << '}';
    }
    search_results_output << endl;

    for(size_t i = 0; i < 50'000; i++)
      docid_count[i] = {0,0};
  }
}

/*Inverted index functions*/

void InvertedIndex::Add(const string& document) {
  docs.push_back(move(document));

  const size_t docid = docs.size() - 1;
  auto doc_split = SplitIntoWords(document);
  for (const auto& word : doc_split) {
    index[word].push_back(docid);
  }
}

vector<size_t> InvertedIndex::Lookup(const string& word) const {
  if (auto it = index.find(word); it != index.end()) {
    return it->second;
  } else {
    return {};
  }
}
