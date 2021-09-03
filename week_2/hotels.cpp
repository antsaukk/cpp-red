#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>
#include <deque>
#include <map>

using namespace std;

class BookingSystem {
public:
	BookingSystem() : 
	booking_times_(SECONDS_PER_DAY_) 
	{}

	long long int ComputeLowerTimeBound(current_time) {
		return current_time - SECONDS_PER_DAY_ > 0 ? current_time - SECONDS_PER_DAY_: 0;
	}

	void RemoveOldBookings(long long int time) {
		while(booking_times_.front() < time) {
			//collect data to remove from 1-day storage
			int key = hid_.front()
			int rooms_to_remove_ = rooms_per_booking_.front();
			int c_id_to_remove_ = customers_per_booking_.front();

			//remove booked rooms and customer
			rooms_per_hotels_[key] -= rooms_to_remove_; 
			customers_per_hotels_[key].erase(c_id_to_remove_); //log(N)

			//remove 1-day storage data
			booking_times_.pop_front();
			hid_.pop_front();
			rooms_per_booking_.pop_front();
			customers_per_booking_.pop_front();
		}
	}

	void Book(long long int time, string hotel_name, int client_id, int room_count){
		//check if hotel already exist
		int hotel_id_key = 0;
		if (hotel_ids_.count(hotel_name) == 0) { //Log(N)
			hotel_id_key = hotel_ids_.size() + 1;
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

	int Customers(string hotel_name) {
		//check that hotel id exist
		if(hotel_ids_.count(hotel_name) == 0) { return 0; }
		int hotel_id_key = hotel_ids_.at(hotel_name);

		return customers_per_hotels_[hotel_id_key];
	}

	int Rooms(string hotel_name) {
		//check that hotel id exist
		if(hotel_ids_.count(hotel_name) == 0) { return 0; }
		int hotel_id_key = hotel_ids_.at(hotel_name);

		return rooms_per_hotels_[hotel_id_key];
	}

private:
	static const int SECONDS_PER_DAY_ = 86400;

	map<string, int> hotel_ids_;
	vector<int> rooms_per_hotels_;
	vector<set<int>> customers_per_hotels_; 
	deque<long long int> booking_times_;
	deque<int> hid_;
	deque<int> rooms_per_booking_;
	deque<int> customers_per_booking_;
};
