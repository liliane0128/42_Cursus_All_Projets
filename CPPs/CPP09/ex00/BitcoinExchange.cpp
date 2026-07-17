#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::~BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other):data(other.data) {}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other)
{
    if (this != &other)
        this->data = other.data;
    return *this;
}

bool    BitcoinExchange::loadDatabase(const std::string& filename)
{
    std::ifstream file(filename.c_str());
	if (!file.is_open())
	{
        std::cerr << "Error: could not open file\n";
        return false;
    }

    std::string line;
    std::getline(file, line); //skip header
    while (std::getline(file, line))
    {
        if (line.empty())
            continue;
        
        size_t pos = line.find(",");
        if (pos == std::string::npos)
            continue;
        
        std::string date = line.substr(0, pos);
        std::string rateStr = line.substr(pos + 1);

		std::stringstream ss(rateStr);
		float rate;
		ss >> rate;
		if (ss.fail() || !ss.eof())
		{
			std::cerr << "Error: invalid number\n";
			continue;
		}
        this->data[date] = rate;
    }

    return true;
}

bool    BitcoinExchange::isValidDate(const std::string& date) const
{
	if (date.size() != 10)
		return false;
	if (date[4] != '-' || date[7] != '-')
		return false;

	for (int i = 0; i < 10; i++)
	{
		if (i == 4 || i == 7)
			continue;
		if (!std::isdigit(date[i]))
			return false;
	}
	
	int year = std::atoi(date.substr(0, 4).c_str());
	int month = std::atoi(date.substr(5, 2).c_str());
	int day = std::atoi(date.substr(8, 2).c_str());
	if (month < 1 || month > 12)
		return false;
	if (month == 2)
	{
		if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
		{
			if (day < 1 || day > 29)
				return false;
		}
		else 
		{
			if (day < 1 || day > 28)
				return false;
		}
	}
	else if (month == 4 || month == 6 || month == 9 || month == 11)
	{
		if (day < 1 || day > 30)
			return false;
	}
	else
	{
		if (day < 1 || day > 31)
			return false;
	}
	return true;
}

bool    BitcoinExchange::isValidValue(float& value) const
{
	if (value < 0)
	{
		std::cerr << "Error: not a positive number.\n";
		return false;
	}
	if (value > 1000)
	{
		std::cerr << "Error: too large a number.\n";
		return false;
	}
	return true;
}

bool    BitcoinExchange::processInput(const std::string& filename)
{
	std::ifstream file(filename.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error: could not open file\n";
		return false;
	}

	std::string line;
	std::getline(file, line);

	while (std::getline(file, line))
	{
		if (line.empty())
			continue;
		size_t pos = line.find("|");
		if (pos == std::string::npos)
		{
			std::cerr << "Error: bad input => " << line << "\n";
			continue;
		}
		std::string date = line.substr(0, pos);
		date.erase(date.find_last_not_of(" \t") + 1);
		date.erase(0, date.find_first_not_of(" \t"));
		if (!isValidDate(date))
		{
			std::cerr << "Error: bad input => " << date << "\n";
			continue;
		}
		
		std::string amountStr = line.substr(pos + 1);
		std::stringstream ss(amountStr);
		float amount;
		ss >> amount;
		if (ss.fail() || !ss.eof())
		{
			std::cerr << "Error: invalid number\n";
			continue;
		}
		
		//float amount = static_cast<float>(std::atof(amountStr.c_str()));
		if (!isValidValue(amount))
			continue;

		std::map<std::string, float>::iterator it = data.find(date);
		if (it == data.end())
		{
			it = data.lower_bound(date);
			if (it == data.begin())
			{
				std::cerr << "Error: date too early.\n";
				continue;
			}
			--it;
		}
		float price = it->second;
		std::cout << date << " => " << amount << " = " << (amount * price) << "\n";
	}
	return true;
}