#include "Order.h"

Order::Order(const std::string code, const int volume, const double price, const bool direction)
{
	this->t_time = time(nullptr);
	this->set_code(code);
	this->set_volume(volume);
	this->set_price(price);
	this->set_type(direction);
	this->n_ordernum = global_ordernum;
	global_ordernum++;
}

Order::Order(const std::string timestamp, const std::string code, const int volume, const double price, const bool direction)
{
	this->set_time(timestamp);
	this->set_code(code);
	this->set_volume(volume);
	this->set_price(price);
	this->set_type(direction);
	this->n_ordernum = global_ordernum;
	global_ordernum++;
}


Order::~Order()
{
}

time_t Order::get_timeSec() const
{
	return this->t_time;
}

std::string Order::get_timeStr() const
{
	char time_temp[CHAR_MAX];
	//ctime_s(time_temp, CHAR_MAX, &this->t_time);
	tm tm_temp;
	localtime_s(&tm_temp, &this->t_time);
	strftime(time_temp, CHAR_MAX, "%Y-%m-%d %H:%M:%S", &tm_temp);
	return time_temp;
}

std::string Order::get_code() const
{
	return this->s_code;
}

int Order::get_volume() const
{
	return  this->n_volume;
}

double Order::get_price() const
{
	return  this->n_price;
}

bool Order::get_type() const
{
	return this->n_type;
}

int Order::get_ordernum() const
{
	return this->n_ordernum;
}

/*
 * Setting time from time_t
 *		User Cannot trace back its order to the time earlier than system time,
 *		If the user trace back too early, the time will set to current system time
 */
void Order::set_time(const time_t time)
{
	if (::time(nullptr) <= time)
	{
		this->t_time = time;
	}
	else
	{
		std::cerr << "Unfortunately, Yesterday No longer more" << std::endl;
		std::cout << "	But we can hold on now :)" << std::endl;
		this->t_time = ::time(nullptr);
	}
}

/*
 * Setting time from input string
 *		Str format: yyyy/mm/dd hh:MM:ss
 *
 *	If format check fail, refuse to set time
 */
void Order::set_time(const std::string time)
{
	// input check for time
	if (time.size() == 19)
	{
		for (auto istr = 0; istr < time.size(); ++istr)
		{
			switch (istr)
			{
			case 4:
				if (time[istr] != '/')
				{
					std::cerr << "Illegal input of order time !" << std::endl;
					return;
				}
				break;

			case 7:
				if (time[istr] != '/')
				{
					std::cerr << "Illegal input of order time !" << std::endl;
					return;
				}
				break;

			case 10:
				if (time[istr] != ' ') 
				{
					std::cerr << "Illegal input of order time !" << std::endl;
					return;
				}
				break;

			case 13:
				if (time[istr] != ':') 
				{
					std::cerr << "Illegal input of order time !" << std::endl;
					return;
				}
				break;

			case 16:
				if (time[istr] != ':') 
				{
					std::cerr << "Illegal input of order time !" << std::endl;
					return;
				}
				break;

			default:
				if (!isalnum(time[istr])) 
				{
					std::cerr << "Illegal input of order time !" << std::endl;
					return;
				}
				break;
			}
		}
	}

	// Convert tm to time_t
	struct tm temp;
	time_t tt;
	localtime_s(&temp, &tt);
	
	sscanf_s(time.c_str(), "%d/%d/%d %d:%d:%d",
		&temp.tm_year, &temp.tm_mon, &temp.tm_mday, &temp.tm_hour, &temp.tm_min, &temp.tm_sec);
	temp.tm_mon--;
	temp.tm_year -= 1900;
	// change time
	tt = mktime(&temp);
	this->set_time(tt);
}


/*
 * Obey the securities naming rule of China and USA:
 *	    1. Only upper case and nums allowed
 *	    2. only support the first '.' signal, other '.' ignored
 *
 *	legal input:
 *		20asdas -> 20ASDAS
 *		56sdq.sh -> 56SDQ.SH
 *		600001.sz -> 600001.SZ
 *
 *	Illegal input part will be ignored:
 *		20aas.aa.ss -> 20AAS.AA
 */
void Order::set_code(std::string code)
{
	auto istr = 0;
	auto isdot = false;
	while (code[istr])
	{
		if (IsCharAlphaNumericA(code[istr]))
		{
			if (isalpha(code[istr]))
			{
				code[istr] = toupper(code[istr]);
			}
			istr++;
		}
		else if (code[istr] == '.' && !isdot)
		{
			isdot = true;
			istr++;
		}
		else
		{
			code.erase(istr, 1);
		}
	}
	this->s_code = code;
}

void Order::set_volume(const int vol)
{
	// it is meaningful to set vol negative
	this->n_volume = vol;
}

void Order::set_price(const double price)
{
	if (price >= 0)
	{
		this->n_price = price;
	}
	else
	{
		this->n_price = 0;
	}
}

void Order::set_type(const int direction)
{
	if (direction)
	{
		this->n_type = true;
	}
	else
	{
		this->n_type = false;
	}
}

std::ostream &operator<<(std::ostream& output, const Order& order)
{
	std::string s_str;
	if (order.get_type())
	{
		s_str = "Buy";
	}
	else
	{
		s_str = "Sell";
	}

	output << std::left << std::setw(20) << order.get_timeStr()
		<< std::left << std::setw(10) << order.get_code()
		<< std::left << std::setw(10) << order.get_price()
		<< std::left << std::setw(10) << s_str
		<< std::left << std::setw(10) << order.get_ordernum();
	return output;
}
