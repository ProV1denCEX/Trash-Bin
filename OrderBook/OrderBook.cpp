#include "Order.h"
#include <vector>
#include <map>

// local function
void update_orderbook(Order new_order,
	std::map<std::string, std::tuple<std::vector<Order>, std::vector<Order>>>* order_book);
void insert_orderbook(bool, std::vector<Order>*, Order);
void interact_orderbooks(std::vector<Order>*, std::vector<Order>*);
void modify_order(Order *);
void que_order(std::map<std::string, std::tuple<std::vector<Order>, std::vector<Order>>>);
int main(int argc, char* argv[]);

// Global
int Order::global_ordernum{ 0 };

int main(int argc, char* argv[])
{
	// Long and short orders list
	std::map<std::string, std::tuple<std::vector<Order>, std::vector<Order>>> order_books;

	auto n_mode = 0;
	auto n_amount = 0;
	double n_price = 0;
	auto n_direction = 0;
	auto n_order = 0;
	std::string s_code;
	char s_time[CHAR_MAX];
	bool is_mod;
	// messege loop
	while (true)
	{
		std::cout << "Please Input Number to Choose: \n";
		std::cout << "	1. New order\n";
		std::cout << "	2. View current orders\n";
		std::cout << "	3. Query orders\n";
		std::cout << "	4. Modify your order\n";
		std::cout << "	5. Quit\n" << std::endl;
		std::cin >> n_mode;
		std::cout << "**********************************************" << std::endl;

		switch (n_mode)
		{
		case 1:
			// Create new order & Insert to order book
			std::cout << "\nPlease Enter the company's code:" << std::endl;
			std::cin >> s_code;

			std::cout << "\nPlease Enter the amount you want to trade:" << std::endl;
			std::cin >> n_amount;

			std::cout << "\nPlease Enter the price you want to trade\n";
			std::cout << "	If you enter 0, the system will place your order with market price." << std::endl;
			std::cin >> n_price;

			std::cout << "\nPlease Enter the direction you want to trade\n";
			std::cout << "	If you enter 0, means you want to sell.\n";
			std::cout << "	If you enter 1, means you want to buy.\n" << std::endl;
			std::cin >> n_direction;

			std::cout << "\nPlease Enter the time you want to trade\n";
			std::cout << "	If you enter 0 or incorrect time format, means you don't want to modify.\n";
			std::cout << "	If you enter correct format, means you want to trade at and after that time.\n";
			std::cout << "	Correct time format: yyyy/mm/dd hh:MM:ss" << std::endl;
			std::cin.ignore();
			std::cin.getline(s_time, CHAR_MAX);
			std::cin.clear();

			if (s_time == "0")
			{
				update_orderbook(Order(s_code, n_amount, n_price, n_direction), &order_books);
			}
			else
			{
				update_orderbook(Order(s_time, s_code, n_amount, n_price, n_direction), &order_books);
			}
			break;

		case 2:
			std::cout << std::left << std::setw(20) << "Time"
				<< std::left << std::setw(10) << "Code"
				<< std::left << std::setw(10) << "Price"
				<< std::left << std::setw(10) << "Direction"
				<< std::left << std::setw(10) << "Order#" << std::endl;

			for (auto key : order_books)
			{
				for (auto element : std::get<0>(key.second))
				{
					std::cout << element << std::endl;
				}

				for (auto element : std::get<1>(key.second))
				{
					std::cout << element << std::endl;
				}
			}
			break;

		case 3:
			// Query
			que_order(order_books);

			break;

		case 4:
			std::cout << "Please enter your order# to modify your order" << std::endl;
			std::cin >> n_order;

			is_mod = false;
			for (auto key : order_books)
			{
				for (auto element : std::get<0>(key.second))
				{
					if (element.get_ordernum() == n_order)
					{
						modify_order(&element);
						is_mod = true;

						// check trade
						interact_orderbooks(&std::get<0>(order_books.at(element.get_code())),
							&std::get<1>(order_books.at(element.get_code())));
						break;
					}
				}

				for (auto element : std::get<1>(key.second))
				{
					if (element.get_ordernum() == n_order)
					{
						modify_order(&element);
						is_mod = true;

						// check trade
						interact_orderbooks(&std::get<0>(order_books.at(element.get_code())),
							&std::get<1>(order_books.at(element.get_code())));
						break;
					}
				}

				if (is_mod) { break; }
			}

			if (!is_mod)
			{
				std::cout << "Cannot find your order! Make sure you enter correct order# !" << std::endl;
			}
			break;

		case 5:
			std::cout << "Cheerio~" << std::endl;
			return 0;

		default:
			break;
		}
	}
}

/*
 * To balance the code performance and data structure
 *		we think the search speed is less important than trade speed
 */
void que_order(std::map<std::string, std::tuple<std::vector<Order>, std::vector<Order>>> order_book)
{
	// Generate Query Temp
	std::vector<Order> total_orders;
	for (auto key : order_book)
	{
		total_orders.insert(total_orders.end(), std::get<0>(key.second).begin(), std::get<0>(key.second).end());
		total_orders.insert(total_orders.end(), std::get<1>(key.second).begin(), std::get<1>(key.second).end());
	}
	std::vector<bool> chosen_orders(total_orders.size(), false);

	int n_filter_type;
	int n_mode;
	auto is_new = true;
	std::string s_filter;
	char s_time[CHAR_MAX];
	int n_filter;
	double n_price;

	// Get search filters
	while (true)
	{
		// Mode Choosing
		if (!is_new)
		{
			/*
			 * 3 Modes:
			*		0: OR Mode
			*		1: AND Mode
			*		66: Clear all search options and restart search
			*/
			std::cout << "\nPlease Choose Search Mode :" << std::endl;
			std::cout << "    Type 0: OR Mode" << std::endl;
			std::cout << "    Type 1: AND Mode" << std::endl;
			std::cout << "    Type 66: Clear Search Options" << std::endl;
			std::cin >> n_mode;
		}
		else
		{
			// if it is first time to search, default mode is OR
			n_mode = 0;
		}

		// Clear Search Options
		// No longer need to input others args
		if (n_mode == 66)
		{
			// reset to new search
			is_new = true;
			// Clear result
			for (auto i = 0; i < chosen_orders.size(); ++i)
			{
				chosen_orders[i] = false;
			}
			continue;
		}

		// Input filter type
		std::cout << "\nPlease Input Search Filter Type :" << std::endl;
		std::cout << "    Type 1 : Time" << std::endl;
		std::cout << "    Type 2 : Code" << std::endl;
		std::cout << "    Type 3 : Price" << std::endl;
		std::cout << "    Type 4 : Direction" << std::endl;
		std::cout << "    Type 5 : Volume" << std::endl;
		std::cout << "    Type 6 : Order#" << std::endl;
		std::cout << "    Type 99 : Quit Search" << std::endl;
		std::cin >> n_filter_type;

		switch (n_filter_type)
		{
		case 1:
			std::cout << "Please enter the filter content: " << std::endl;
			std::cout << "	Please enter correct time format\n";
			std::cout << "	Correct time format: yyyy/mm/dd hh:MM:ss" << std::endl;
			std::cin.ignore();
			std::cin.getline(s_time, CHAR_MAX);
			std::cin.clear();
			std::cin.ignore();

			// find all matches
			for (auto icar = 0; icar < total_orders.size(); ++icar)
			{
				const auto is_match = total_orders[icar].get_timeStr() == s_filter;
				switch (n_mode)
				{
					// OR Mode
				case 0:
					chosen_orders[icar] = is_match || chosen_orders[icar];
					break;

					// AND Mode
				case 1:
					// it is impossible that is_new is true (see the n_mode choosing process above).
					chosen_orders[icar] = chosen_orders[icar] && is_match;
					break;
				default:;
				}
			}

			break;

		case 2:
			std::cout << "Please enter the filter content: " << std::endl;
			std::cin >> s_filter;

			for (auto icar = 0; icar < total_orders.size(); ++icar)
			{
				const auto is_match = total_orders[icar].get_code() == s_filter;
				switch (n_mode)
				{
				case 0:
					chosen_orders[icar] = is_match || chosen_orders[icar];
					break;

				case 1:
					chosen_orders[icar] = chosen_orders[icar] && is_match;
					break;
				default:;
				}
			}
			break;

		case 3:
			std::cout << "Please enter the filter content: " << std::endl;
			std::cin >> n_price;

			for (auto icar = 0; icar < total_orders.size(); ++icar)
			{
				const auto is_match = total_orders[icar].get_price() == n_price;
				switch (n_mode)
				{
				case 0:
					chosen_orders[icar] = is_match || chosen_orders[icar];
					break;

				case 1:
					chosen_orders[icar] = chosen_orders[icar] && is_match;
					break;
				default:;
				}
			}
			break;

		case 4:
			std::cout << "Please enter the filter content: \n" << "	1. Buy    2. Sell" << std::endl;
			std::cin >> n_filter;

			for (auto icar = 0; icar < total_orders.size(); ++icar)
			{
				const auto is_match = total_orders[icar].get_type() == n_filter;
				switch (n_mode)
				{
				case 0:
					chosen_orders[icar] = is_match || chosen_orders[icar];
					break;

				case 1:
					chosen_orders[icar] = chosen_orders[icar] && is_match;
					break;
				default:;
				}
			}
			break;

		case 5:
			std::cout << "Please enter the filter content: " << std::endl;
			std::cin >> n_filter;

			for (auto icar = 0; icar < total_orders.size(); ++icar)
			{
				const auto is_match = total_orders[icar].get_volume() == n_filter;
				switch (n_mode)
				{
				case 0:
					chosen_orders[icar] = is_match || chosen_orders[icar];
					break;

				case 1:
					chosen_orders[icar] = chosen_orders[icar] && is_match;
					break;
				default:;
				}
			}
			break;

		case 6:
			std::cout << "Please enter the filter content: " << std::endl;
			std::cin >> n_filter;

			for (auto icar = 0; icar < total_orders.size(); ++icar)
			{
				const auto is_match = total_orders[icar].get_ordernum() == n_filter;
				switch (n_mode)
				{
				case 0:
					chosen_orders[icar] = is_match || chosen_orders[icar];
					break;

				case 1:
					chosen_orders[icar] = chosen_orders[icar] && is_match;
					break;
				default:;
				}
			}

		case 99:
			std::cout << "Search mode exited." << std::endl;
			return;

		default:
			break;
		}

		// print all matches
		std::cout << "\nSearch Result :" << std::endl;
		auto is_show = false;
		for (auto i = 0; i < total_orders.size(); ++i)
		{
			if (chosen_orders[i])
			{
				std::cout << total_orders[i] << std::endl;
				is_show = true;
			}
		}

		// Check if cars match
		if (!is_show)
		{
			std::cout << "No orders matches your filters ! The filters will all reset and please input new options" << std::endl;
			is_new = true;
			// Clear result
			for (auto i = 0; i < chosen_orders.size(); ++i)
			{
				chosen_orders[i] = false;
			}
		}
		else
		{
			// no longer new search
			is_new = false;
		}
		std::cout << "\nYour Search will Continue, the filters input before still works\n" << std::endl;
	}
}

/*
 * Modify the order by users
 */
void modify_order(Order *order)
{
	auto n_mode = 0;
	auto n_value = 0;
	double n_price = 0;
	std::string s_value;
	char s_time[CHAR_MAX];
	while (true)
	{
		std::cout << "Please enter number to choose order's info: \n"
			<< "		0. Cancel this order\n" << "		1. Time\n" << "	2. Code\n"
			<< "	3. Price\n" << "	4. Direction\n" << "		5. Amount\n"
			<< "	6. Quit Modification" << std::endl;
		std::cin >> n_mode;

		switch (n_mode)
		{
		case 0:
			order->set_volume(-999);
			return;

		case 1:
			std::cout << "Please enter new Value: " << std::endl;
			std::cout << "	Please enter correct format\n";
			std::cout << "	Correct time format: yyyy/mm/dd hh:MM:ss" << std::endl;
			std::cin.ignore();
			std::cin.getline(s_time, CHAR_MAX);
			std::cin.clear();

			order->set_time(s_time);
			break;

		case 2:
			std::cout << "Please enter new Value: " << std::endl;
			std::cin >> s_value;

			order->set_code(s_time);
			break;

		case 3:
			std::cout << "Please enter new Value: " << std::endl;
			std::cin >> n_price;

			order->set_price(n_price);
			break;

		case 4:
			std::cout << "Please enter new Value: \n" << "	1. Buy    2. Sell" << std::endl;
			std::cin >> n_value;

			order->set_type(n_value);
			break;

		case 5:
			std::cout << "Please enter new Value: " << std::endl;
			std::cin >> n_value;

			order->set_volume(n_value);
			break;
		case 6:
			std::cout << "Modification exited " << std::endl;

			return;
		default:
			break;
		}
	}
}

/*
 * update order book:
 *			1. insert new order to current order book
 *			2. check if there are deals to make
 *			3. check for market price order
 */
void update_orderbook(Order new_order,
	std::map<std::string, std::tuple<std::vector<Order>, std::vector<Order>>>* order_book)
{
	// check if exist
	if (!order_book->count(new_order.get_code()))
	{
		const std::tuple<std::vector<Order>, std::vector<Order>> temp;
		(*order_book)[new_order.get_code()] = temp;
	}

	// insert
	if (new_order.get_type())
	{
		if (new_order.get_price() == 0)
		{
			double n_price;
			if (std::get<1>(order_book->at(new_order.get_code())).empty())
			{
				if (std::get<0>(order_book->at(new_order.get_code())).empty())
				{
					std::cerr << "No Market price for this security now, please re-enter the price!\n"
						<< "If you still enter price not larger than 0, your order will be executed in 0! "
						<< std::endl;
					std::cin >> n_price;
				}
				else
				{
					n_price = std::get<0>(order_book->at(new_order.get_code())).at(0).get_price();
				}
			}
			else
			{
				n_price = std::get<1>(order_book->at(new_order.get_code())).at(0).get_price();
			}
			new_order.set_price(n_price);
		}
		insert_orderbook(new_order.get_type(), &std::get<0>(order_book->at(new_order.get_code())), new_order);
	}
	else
	{
		if (new_order.get_price() == 0)
		{
			double n_price;
			if (std::get<0>(order_book->at(new_order.get_code())).empty())
			{
				if (std::get<1>(order_book->at(new_order.get_code())).empty())
				{
					std::cerr << "No Market price for this security now, please re-enter the price!\n"
						<< "If you still enter price not larger than 0, your order will be executed in 0! "
						<< std::endl;
					std::cin >> n_price;
				}
				else
				{
					n_price = std::get<1>(order_book->at(new_order.get_code())).at(0).get_price();
				}
			}
			else
			{
				n_price = std::get<0>(order_book->at(new_order.get_code())).at(0).get_price();
			}
			new_order.set_price(n_price);
		}
		insert_orderbook(new_order.get_type(), &std::get<1>(order_book->at(new_order.get_code())), new_order);
	}

	// check trade
	interact_orderbooks(&std::get<0>(order_book->at(new_order.get_code())), &std::get<1>(order_book->at(new_order.get_code())));
}


/**
 * \brief
 * \param n_direction: 1->buy 0-> Sell
 * \param order_book: orderbook 2 insert
 * \param new_order: newly created order
 * \return inserted order
 */
void insert_orderbook(const bool n_direction, std::vector<Order>* order_book, Order new_order)
{
	if (!order_book->empty())
	{
		if (n_direction)
			// Insert BUY side orderbook
		{
			for (auto iorder = 0; iorder < order_book->size(); ++iorder)
			{
				// If new order's price is higher than the target order, 
				//		it means that new order has priority than target order,
				//		and should be placed before target order.
				if (order_book->at(iorder).get_price() < new_order.get_price())
				{
					order_book->insert(order_book->begin() + iorder, new_order);
					break;
				}
				if (order_book->at(iorder).get_price() == new_order.get_price())
				{
					// if new order's price is equal to target order,
					//		we shall compare the trading time,
					//		the earlier one has priority
					if (order_book->at(iorder).get_timeSec() > new_order.get_timeSec())
					{
						order_book->insert(order_book->begin() + iorder, new_order);
						break;
					}
				}
			}
		}
		else
			// Insert SELL side orderbook
		{
			for (auto iorder = 0; iorder < order_book->size(); ++iorder)
			{
				// If new order's price is lower than the target order, 
				//		it means that new order has priority than target order,
				//		and should be placed before target order.
				if (order_book->at(iorder).get_price() > new_order.get_price())
				{
					order_book->insert(order_book->begin() + iorder, new_order);
					break;
				}
				if (order_book->at(iorder).get_price() == new_order.get_price())
				{
					// if new order's price is equal to target order,
					//		we shall compare the trading time,
					//		the earlier one has priority
					if (order_book->at(iorder).get_timeSec() > new_order.get_timeSec())
					{
						order_book->insert(order_book->begin() + iorder, new_order);
						break;
					}
				}
			}
		}
	}
	else
	{
		order_book->push_back(new_order);
	}
}

/*
 * interact buy orders and sell orders to find if there are some trades
 * Then delete orders with non positive trading volume
 * Can use structured binding or pointers
 */
void interact_orderbooks(std::vector<Order>* d_buy_orderbook, std::vector<Order>* d_sell_orderbook)
{
	for (auto iorder = 0; iorder < d_buy_orderbook->size(); ++iorder)
	{
		for (auto jorder = 0; jorder < d_sell_orderbook->size(); ++jorder)
		{
			/*
			 * There are three requirements to make a deal:
			 *		1. The buyer order still has vol to trade
			 *		2. The buyer's price is not less than seller's price
			 *		3. The Two order aim at the same contract
			 *		4. Enough trade vol
			 */
			if (d_buy_orderbook->at(iorder).get_volume() > 0
				&& d_sell_orderbook->at(jorder).get_volume() > 0
				&& d_buy_orderbook->at(iorder).get_price() >= d_sell_orderbook->at(jorder).get_price())
			{
				// generate a deal
				const auto n_vol = min(d_buy_orderbook->at(iorder).get_volume(), d_sell_orderbook->at(jorder).get_volume());
				d_buy_orderbook->at(iorder).set_volume(d_buy_orderbook->at(jorder).get_volume() - n_vol);
				d_sell_orderbook->at(jorder).set_volume(d_sell_orderbook->at(jorder).get_volume() - n_vol);

				// Output this deal
				std::cout << "There is a deal: \n";
				std::cout << d_buy_orderbook->at(iorder) << std::endl;
				std::cout << d_sell_orderbook->at(jorder) << std::endl;
			}
		}
	}

	// Delete executed orders
	auto counter = 0;
	while (counter < d_buy_orderbook->size())
	{
		if (d_buy_orderbook->at(counter).get_volume() <= 0)
		{
			d_buy_orderbook->erase(d_buy_orderbook->begin() + counter);
		}
		else
		{
			++counter;
		}
	}

	counter = 0;
	while (counter < d_sell_orderbook->size())
	{
		if (d_sell_orderbook->at(counter).get_volume() <= 0)
		{
			d_sell_orderbook->erase(d_sell_orderbook->begin() + counter);
		}
		else
		{
			++counter;
		}
	}
}