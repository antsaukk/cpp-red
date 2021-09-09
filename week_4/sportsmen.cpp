#include <iostream>
#include <vector>
#include <list>

using namespace std;

int main() {
 int n; 
 cin >> n; 

 list<int> sportsmen; 
 vector<list<int>::iterator> iterators(100001);
 vector<int> itermap(100001, -1);

 int num, que; 
 
 for(int i = 0; i < n; i++) {
  cin >> num >> que; 
  if (itermap[que] == -1) {
  	sportsmen.push_back(num);
  	itermap[num] = 1; 
  	iterators[num] = prev(sportsmen.end());
  } else {
  	sportsmen.insert(iterators[que], num);
  	itermap[num] = 1; 
  	iterators[num] = prev(iterators[que]);
  }
 }

 for(auto val = sportsmen.begin(); val != sportsmen.end(); val = next(val)) {
 	cout << *val << endl;

 }
 return 0;
}
