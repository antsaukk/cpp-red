#pragma once

#include "http_request.h"
#include "profile.h"

#include <iostream>
#include <sstream>
#include <string_view>
#include <map>
using namespace std;

class Stats {
public:
  Stats() {}

  void AddMethod(string_view method);

  void AddUri(string_view uri);

  const map<string_view, int>& GetMethodStats() const { return method_stats; }
  const map<string_view, int>& GetUriStats() const { return uri_stats; }
private: 
  map<string_view, int> method_stats {{"GET", 0}, {"POST", 0}, {"PUT", 0}, {"DELETE", 0}, {"UNKNOWN", 0}}; 
  map<string_view, int> uri_stats  {{"/", 0}, {"/order", 0}, {"/product", 0}, {"/basket", 0}, {"/help", 0}, {"unknown", 0}}; 
};

HttpRequest ParseRequest(string_view line);