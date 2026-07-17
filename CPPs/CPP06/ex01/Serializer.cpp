#include "Serializer.hpp"

Serializer::Serializer() {}
Serializer::~Serializer() {}
Serializer::Serializer(const Serializer& other)
{
	*this = other;
}
Serializer& Serializer::operator=(const Serializer& other)
{
	(void)other;
	return *this;
}

/**
 * Using 'reinterpret_cast' to a 'uintptr_t' to perform a bit-wise conversion 
 * of a pointer to an integer.
 * This is highly efficient because it doesn't change the underlying bit 
 * pattern of the address; it simply tells the compiler to treat that memory 
 * address as a numeric value.
 * 
 * uintptr_t is an unsigned integer type capable of storing a pointer value.
 */
uintptr_t Serializer::serialize(Data* ptr)
{
	return reinterpret_cast<uintptr_t>(ptr);
}

Data* Serializer::deserialize(uintptr_t raw)
{
	return reinterpret_cast<Data*>(raw);
}

