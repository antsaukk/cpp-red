#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>
#include <deque>
#include <map>
#include <set>

using namespace std;

class BookingSystem {
public:
	BookingSystem() :
	rooms_per_hotels_(HOTELS_UPPER_BOUND_ + 1, 0),
	customers_per_hotels_(HOTELS_UPPER_BOUND_ + 1)
	{}

	long long int ComputeLowerTimeBound(long long int current_time) {
		return current_time - SECONDS_PER_DAY_;
	}

	void RemoveOldBookings(long long int time) {
		while(booking_times_.front() <= time && !booking_times_.empty()) {
			//collect data to remove from 1-day storage
			int key = hid_.front();
			int rooms_to_remove_ = rooms_per_booking_.front();
			int c_id_to_remove = customers_per_booking_.front();

			//remove booked rooms and customer
			rooms_per_hotels_[key] -= rooms_to_remove_; 
			customers_per_hotels_[key].erase(c_id_to_remove); //log(N)

			//remove 1-day storage data
			booking_times_.pop_front();
			hid_.pop_front();
			rooms_per_booking_.pop_front();
			customers_per_booking_.pop_front();
		}
	}

	void Book(long long int time, const string& hotel_name, int client_id, int room_count){
		//check if hotel already exist
		int hotel_id_key = 0;
		if (hotel_ids_.count(hotel_name) == 0) { //Log(N)
			hotel_id_key = hotel_ids_.size() + 1; // ?
			hotel_ids_[hotel_name] = hotel_id_key; //Log(N)
		} else {
			hotel_id_key = hotel_ids_.at(hotel_name); //Log(N)
		}
		auto check = ComputeLowerTimeBound(time); //set inside removeOldBookings
		// remove information about bookings that are older than one day and update global state
		RemoveOldBookings(check); 

		//record booking data for 1-day storage
		booking_times_.push_back(time); 
		hid_.push_back(hotel_id_key); 
		rooms_per_booking_.push_back(room_count);
		customers_per_booking_.push_back(client_id);

		//update bookings
		rooms_per_hotels_[hotel_id_key] += room_count; //increase number of booked rooms
		customers_per_hotels_[hotel_id_key].insert(client_id); //record customer 

	}

	int Customers(string hotel_name) const {
		//check that hotel id exist
		if(hotel_ids_.count(hotel_name) == 0) { return 0; }
		int hotel_id_key = hotel_ids_.at(hotel_name);

		return customers_per_hotels_[hotel_id_key].size();
	}

	int Rooms(string hotel_name) const {
		//check that hotel id exist
		if(hotel_ids_.count(hotel_name) == 0) { return 0; }
		int hotel_id_key = hotel_ids_.at(hotel_name);

		return rooms_per_hotels_[hotel_id_key];
	}

private:
	static const int SECONDS_PER_DAY_ = 86400;
	static const int HOTELS_UPPER_BOUND_ = 100000;

	map<string, int> hotel_ids_;
	vector<int> rooms_per_hotels_;
	vector<set<int>> customers_per_hotels_; 
	deque<long long int> booking_times_;
	deque<int> hid_;
	deque<int> rooms_per_booking_;
	deque<int> customers_per_booking_;
};


int main() {

	// for furtherr optimization untie&unsync stdin and stdout 
  //ios::sync_with_stdio(false);
  //cin.tie(nullptr);

  BookingSystem system;

  int query_count;
  cin >> query_count;

  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    cin >> query_type;

    if (query_type == "BOOK") {
      long long int time;
      cin >> time;
      string hotel_name;
      cin >> hotel_name;
      int client_id;
      cin >> client_id;
      int room_count; 
      cin >> room_count;

      system.Book(time, hotel_name, client_id, room_count);
    } else if (query_type == "CLIENTS") {
    	string hotel_name;
    	cin >> hotel_name;

      	cout << system.Customers(hotel_name) << "\n";
    } else if (query_type == "ROOMS") {
    	string hotel_name; 
    	cin >> hotel_name;

    	cout << system.Rooms(hotel_name) << "\n";
    }
  }
  return 0;
}