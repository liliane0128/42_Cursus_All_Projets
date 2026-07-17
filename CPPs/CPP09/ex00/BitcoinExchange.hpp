#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

#include <map>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>

class BitcoinExchange 
{
private:
    std::map<std::string, float> data;

	bool isValidDate(const std::string& date) const;
    bool isValidValue(float& value) const;

public:
    BitcoinExchange();
    ~BitcoinExchange();
    BitcoinExchange(const BitcoinExchange& other);
    BitcoinExchange& operator=(const BitcoinExchange& other);

    bool    loadDatabase(const std::string& filename);
    bool    processInput(const std::string& filename);

};

#endif