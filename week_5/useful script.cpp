//split stream in to string with separator
/*for (string line; getline(input, line); ) {
    string::const_iterator start = line.begin();
    string::const_iterator end = line.end();
    string::const_iterator next = find(start, end, ' ');
    while (next != end) {
      words.push_back(string(start, next));
      start = next + 1;
      next = find(start, end, ' ');
    }
    words.push_back(string(start, next));
  }*/ 