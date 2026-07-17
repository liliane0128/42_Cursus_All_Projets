#include "iter.hpp"

template <typename T>
void    printElement(const T& x)
{
    std::cout << x << " ";
}

void increment(int& n)
{
    n++;
}

int main()
{
    int intArray[] = {1, 2, 3, 4, 5};
    iter(intArray, 5, printElement<int>);
    std::cout << std::endl;

    iter(intArray, 5, increment);
    iter(intArray, 5, printElement<int>);
    std::cout << std::endl;

    std::string strArray[] = {"hello", "world", "how", "are", "you", "?"};
    iter(strArray, 6, printElement<std::string>);


    return 0;
}