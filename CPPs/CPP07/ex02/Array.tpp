#include <stdexcept>

template <typename T>
Array<T>::Array():_elements(NULL), _size(0) {}

/**
 * @brief Value-Initialization vs. Default-Initialization.
 * * - Using 'new T[n]' (without parentheses) performs default-initialization. 
 * For primitive types (int, float, etc.), memory remains uninitialized, 
 * containing "garbage" values.
 * 
 * - Using 'new T[n]()' (with parentheses) performs value-initialization. 
 * This ensures that primitive types are zero-initialized (e.g., int becomes 0), 
 * preventing undefined behavior from uninitialized data.
 */
template <typename T>
Array<T>::Array(unsigned int n): _size(n)
{
	_elements = new T[n]();
}

template <typename T>
Array<T>::~Array()
{
	delete[] this->_elements;
}

/**
 * @note Why initialize to NULL first?
 * * The Copy Constructor reuses 'operator=', which starts by 
 * calling 'delete[] _elements'.
 * * If _elements is not initialized to NULL, it contains random 
 * garbage. Deleting a garbage pointer causes a crash.
 * * By setting it to NULL in the initializer list, we ensure 
 * the 'delete[]' call inside the assignment operator is safe 
 * and defined.
 */
template <typename T>
Array<T>::Array(const Array& other):_elements(NULL), _size(0)
{
	*this = other;
}

/**
 * @note Return Type in Template Implementation:
 * * Use 'Array<T>&' outside the class body, 'Array' is just a template 
 * name. You must specify the template arguments <T> to refer to 
 * the actual class type.
 */
template <typename T>
Array<T>& Array<T>::operator=(const Array& other)
{
	if (this == &other)
		return *this;

	delete[] this->_elements;
	
	this->_size = other._size;
	if (this->_size > 0)
	{
		this->_elements = new T[this->_size]();
		for (unsigned int i = 0; i < this->_size; i++)
			this->_elements[i] = other._elements[i];
	}
	else
		this->_elements = NULL;
	return *this;
}

/**
 * @note About std::out_of_range:
 * * It is a standard exception defined in <stdexcept>.
 * * It inherits from std::logic_error, signaling that the code 
 * attempted to access an element outside the valid range.
 */
template <typename T>
T& Array<T>::operator[](unsigned int index)
{
	if (index >= _size)
		throw std::out_of_range("Index out of range");
	return this->_elements[index];
}

template <typename T>
const T& Array<T>::operator[](unsigned int index) const
{
	if (index >= _size)
		throw std::out_of_range("Index out of range");
	return this->_elements[index];
}

template <typename T>
unsigned int	Array<T>::size() const
{
	return _size;
}

