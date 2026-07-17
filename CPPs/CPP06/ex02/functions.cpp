#include "functions.hpp"

Base* generate(void)
{
    int r = rand() % 3;
    if (r == 0) return new A;
    if (r == 1) return new B;
    return new C;
}

/**
 * dynamic_cast: 
 * - The Only cast checked at runtime via RTTI (Run-Time Type Information).
 * - Requirement: The Base class must be polymorphic (have at least one virtual function).
 * - Pointers: Returns nullptr on failure.
 * - References: Throws std::bad_cast exception on failure.
 * - Purpose: Safely downcast from a Base pointer/reference to a Derived one.
 */
void identify(Base* p)
{
    if (!p)
		return;

	if (dynamic_cast<A*>(p))
        std::cout << "A" << std::endl;
    else if (dynamic_cast<B*>(p))
        std::cout << "B" << std::endl;
    else if (dynamic_cast<C*>(p))
        std::cout << "C" << std::endl;
}

/**
 * Pointer vs Reference:
 * A pointer can point to nullptr, which provide a way for 'dynamic_cast'
 * to signal failure. However, a reference must always be bound to a valid
 * object, so it must use try-catch block to throw an exception upon failure.
 */
void identify(Base& p)
{
    try
    {
        A& a = dynamic_cast<A&>(p);
        (void)a;
        std::cout << "A" << std::endl;
        return;
    }
    catch(std::exception& e) {}
    
    try
    {
        B& b = dynamic_cast<B&>(p);
        (void)b;
        std::cout << "B" << std::endl;
        return;
    }
    catch(const std::exception& e) {}
        
    try
    {
        C& c = dynamic_cast<C&>(p);
        (void)c;
        std::cout << "C" << std::endl;
        return;
    }
    catch(const std::exception& e) {}
}
