#include <vector>
//#include <deque>
#include <stdexcept>
#include <iostream>

template <typename T>
class Deque {
public:
	Deque() = default;

	bool Empty() const { return front_v.empty() && back_v.empty(); } 

	size_t Size() const { return front_v.size() + back_v.size(); }

	T& operator[](size_t index) {
		return front_v.size() > index ? front_v[front_v.size() - index - 1] : back_v[index - front_v.size()];
	}

	const T& operator[](size_t index) const {
		return front_v.size() > index ? front_v[front_v.size() - index - 1] : back_v[index - front_v.size()];
	}

	T& At(size_t index) {
		if (index >= Size()) { throw std::out_of_range("index out of bounds"); }
		else {
			return front_v.size() > index ? front_v[front_v.size() - index - 1] : back_v[index - front_v.size()];
		}
	}

	const T& At(size_t index) const {
		if (index >= Size()) { throw std::out_of_range("index out of bounds"); }
		else {
			return front_v.size() > index ? front_v[front_v.size() - index - 1] : back_v[index - front_v.size()];
		}
	}

	T& Front() { 
		return !front_v.empty() ? front_v.back() : back_v[0];
	}

	const T& Front() const { 
		return !front_v.empty() ? front_v.back() : back_v[0];
	}

	T& Back() { 
		return !back_v.empty() ? back_v.back() : front_v[0];
	}

	const T& Back() const { 
		return !back_v.empty() ? back_v.back() : front_v[0];
	}

	void PushFront(T element) { front_v.push_back(element); }
	void PushBack(T element) { back_v.push_back(element); }

private:
	std::vector<T> front_v;
	std::vector<T> back_v;
};


/*int main() {

	Deque<int> D;

	D.PushBack(1);
	D.PushFront(2);
	D.PushBack(3);
	D.PushFront(4);
	//4 2 1 3

	std::cout << "TESTING PUSH_FRONT/BACK AND FRONT/BACK" << std::endl;

	std::cout << "My Deque: " << D.Front() << " " << D.Back() << std::endl;

	std::deque<int> d; 

	d.push_back(1);
	d.push_front(2);
	d.push_back(3);
	d.push_front(4);
	//4 2 1 3

	std::cout << "STL Deque: " << d.front() << " " << d.back() << std::endl;

	std::cout << "TESTING []" << std::endl;

	for(int i = 0; i < 4; i++) {
		std::cout << D[i] << " ";	
	}
	std::cout << std::endl;
	for(int i = 0; i < 4; i++) {
		std::cout << d[i] << " ";
	}
	std::cout << std::endl;

	std::cout << "TESTING At()" << std::endl;

	for(int i = 0; i < 4; i++) {
		std::cout << D.At(i) << " ";	
	}
	std::cout << std::endl;
	for(int i = 0; i < 4; i++) {
		std::cout << d.at(i) << " ";
	}
	std::cout << std::endl;

	return 0; 
}*/
