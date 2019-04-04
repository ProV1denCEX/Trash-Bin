#pragma once
#include <ctime>
#include <string>
#include <Windows.h>
#include <iostream>
#include <iomanip>

class Order
{
public:
	static int global_ordernum;

	// Constructor: Only need code, volume, price, and direction
	// Not need for time, time set as sys time
	Order(std::string, int, double, bool);

	// can set time
	Order(std::string, std::string, int, double, bool);

	~Order();

	// Getters
	time_t get_timeSec() const;
	std::string get_timeStr() const;
	std::string get_code() const;
	int get_volume() const;
	double get_price() const;
	bool get_type() const;
	int get_ordernum() const;

	// Setters
	void set_time(time_t);
	void set_time(std::string);
	void set_code(std::string);
	void set_volume(int);
	void set_price(double);
	void set_type(int);

	// Overload << function
	friend std::ostream &operator<<(std::ostream &output, const Order &order);

private:
	/*
	 * t_time: order's time, Saved as time_t
	 * s_code: order's code
	 * n_volume: order's volume
	 * n_price: order's price
	 * n_type: order's direction: false->short, true-> long
	 */
	time_t t_time;
	std::string s_code;
	int n_volume;
	double n_price;
	bool n_type;
	int n_ordernum;
};

