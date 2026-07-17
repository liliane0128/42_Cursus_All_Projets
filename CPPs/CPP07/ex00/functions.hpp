#ifndef FUNCTIONS_HPP
# define FUNCTIONS_HPP

/**
 * @brief A template is a blueprint for creating functions or classes.
 * * Instead of manually overloading functions for 'int', 'float', or 'std::string', 
 * this single generic version handles any type.
 * * Instantiation:
 * At compile-time, the compiler uses 'Type Inference' to identify the type of 'T' 
 * based on the arguments passed, then generates the specific machine code for 
 * that type.
 * * @param T: Placeholder (formal type parameter) replaced by the actual type.
 */
template <typename T> void    swap(T& a, T& b);
template <typename T> const T&  min(const T& a, const T& b);
template <typename T> const T&  max(const T& a, const T& b);

template <typename T>
void    swap(T& a, T& b)
{
    T  tmp = a;
    a = b;
    b = tmp;
}

template <typename T>
const T&  min(const T& a, const T& b)
{
    if (b <= a)
        return b;
    return a;
}

template <typename T>
const T&  max(const T& a, const T& b)
{
    if (b >= a)
        return b;
    return a;
}

#endif