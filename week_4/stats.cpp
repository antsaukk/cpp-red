#include "stats.h"

void Stats::AddMethod(string_view method) {
 if (method_stats.count(method) == 0) { method_stats.at("UNKNOWN")++; } 
 else { method_stats.at(method)++; }
}

void Stats::AddUri(string_view uri) {
 if (uri_stats.count(uri) == 0) { uri_stats.at("unknown")++; } 
 else { uri_stats.at(uri)++; }
}

string_view Cut(string_view& line) {
  size_t space = line.find(' ');
  string_view str = line.substr(0, space);
  line.remove_prefix(space + 1);
  return str;
}

HttpRequest ParseRequest(string_view line) {
  HttpRequest res;

  while(line.find(' ') == 0) { line.remove_prefix(1); }

  res.method = Cut(line);
  res.uri = Cut(line);
  res.protocol = line;

  return res;
}