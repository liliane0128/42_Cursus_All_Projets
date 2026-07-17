#include "Account.hpp"
#include <ctime>
#include <iostream>

/**
 * Brief Understanding Account Class Variables / Functions 
 * * 1. STATIC MEMBERS (Class-level / Global Bank State):
 * Variables: Prefixed with 'static' in the header. Only ONE copy exists 
 * in memory, shared by all instances. Tracks bank-wide data (e.g., total funds).
 * - Functions: Can be called without an object (e.g., Account::getNbAccounts()).
 * NOTE: Static functions can ONLY access static variables. They have 
 * no 'this' pointer and cannot see individual account data.
 * 
 * * 2. INSTANCE MEMBERS (Object-level / Personal Account State):
 * - Variables: Not prefixed with 'static'. Every object (instance) gets its 
 * own unique copy. Tracks individual data (e.g., your specific balance).
 * - Functions: Must be called by a specific object (e.g., myAcc.displayStatus()).
 * These functions can access BOTH instance variables (their own data) 
 * AND static variables (the bank's global data).
 */

/** Static variables must be initialized at the begining of the program.
 * Requires the Type and Scope Resolution Operator (e.g., int Account::_nbAccounts).
 * These are initialized only ONCE for the entire duration of the program.
 */
int Account::_nbAccounts = 0;
int Account::_totalAmount = 0;
int Account::_totalNbDeposits = 0;
int Account::_totalNbWithdrawals = 0;

/**
 * Displays current system time in the format: [YYYYMMDD_HHMMSS] 
 * 1. std::time(NULL) gets the current time as seconds since the Epoch.
 * 2. std::localtime() converts seconds into a structured tm format (year, month, etc.).
 * 3. std::strftime() formats the tm struct into the specific string required by the log.
 * Note: We use a char buffer and strftime to ensure strict C++98 compliance.
 */
void Account::_displayTimestamp( void )
{
	std::time_t now = std::time(NULL);
	struct tm *t = std::localtime(&now);
	char buf[20];

	std::strftime(buf, sizeof(buf), "[%Y%m%d_%H%M%S] ", t);
	std::cout << buf;
}

int	Account::getNbAccounts( void )
{
	return _nbAccounts;
}

int	Account::getTotalAmount( void )
{
	return _totalAmount;
}

int	Account::getNbDeposits( void )
{
	return _totalNbDeposits;
}

int	Account::getNbWithdrawals( void )
{
	return _totalNbWithdrawals;
}+

void	Account::displayAccountsInfos( void )
{
	_displayTimestamp();
	std::cout << "accounts:" << getNbAccounts() << ";"
			  << "total:" << getTotalAmount() << ";"
			  << "deposits:" << getNbDeposits() << ";"
			  << "withdrawals:" << getNbWithdrawals() << std::endl;
}

//Constructor : using Initializer List (use of ":" and "," before "{") 
Account::Account( int initial_deposit ) : 
    _accountIndex(_nbAccounts),
    _amount(initial_deposit),
    _nbDeposits(0),
    _nbWithdrawals(0)
{
    _nbAccounts++;
    _totalAmount += _amount;

    _displayTimestamp();
    std::cout << "index:" << _accountIndex << ";"
              << "amount:" << _amount << ";"
              << "created" << std::endl;
}

void	Account::makeDeposit( int deposit )
{
	int p_amount = _amount;

	_amount += deposit;
	_nbDeposits++;
	_totalAmount += deposit;
	_totalNbDeposits++;

	_displayTimestamp();
	std::cout << "index:" << _accountIndex << ";"
			  << "p_amount:" << p_amount << ";"
			  << "deposit:" << deposit << ";"
			  << "amount:" << _amount << ";"
			  << "nb_deposits:" << _nbDeposits << std::endl;
}

bool	Account::makeWithdrawal( int withdrawal )
{
	int p_amount = _amount;

	_displayTimestamp();
	std::cout << "index:" << _accountIndex << ";"
			  << "p_amount:" << p_amount << ";";

	if (withdrawal > _amount)
	{
		std::cout << "withdrawal:refused" << std::endl;
		return false;
	}
	
	_amount -= withdrawal;
	_nbWithdrawals++;
	_totalAmount -= withdrawal;
	_totalNbWithdrawals++;

	std::cout << "withdrawal:" << withdrawal << ";"
			  << "amount:" << _amount << ";"
			  << "nb_withdrawals:" << _nbWithdrawals << std::endl;
	return true;
}


int		Account::checkAmount( void ) const
{
	return _amount;
}

void	Account::displayStatus( void ) const
{
	_displayTimestamp();
	std::cout << "index:" << _accountIndex << ";"
			  << "amount:" << _amount << ";"
			  << "deposits:" << _nbDeposits << ";"
			  << "withdrawals:" << _nbWithdrawals << std::endl;
}

//Destructor 
Account::~Account( void )
{
	_displayTimestamp();

	std::cout << "index:" << _accountIndex << ";"
			  << "amount:" << _amount << ";"
			  << "closed" << std::endl;

	// Decrement the global count of active accounts
	_nbAccounts--;
	_totalAmount -= _amount;
}
