#include "Serializer.hpp"

int main()
{
	Data myData;
	myData.id = 42;
	myData.name = "Lucas";
	myData.value = 3.14f;

	Data* originalPtr = &myData;

	uintptr_t raw = Serializer::serialize(originalPtr);
	Data* deserializedPtr = Serializer::deserialize(raw);
	std::cout << "Original Pointer:     " << originalPtr << std::endl;
    std::cout << "Deserialized Pointer: " << deserializedPtr << std::endl;

    if (deserializedPtr == originalPtr) {
        std::cout << "Success: Both pointers are equal!" << std::endl;
        std::cout << "Data content: " << deserializedPtr->name << " (ID: " << deserializedPtr->id << "," 
		<< " Value: " << deserializedPtr->value << ")"<< std::endl;
    } else {
        std::cout << "Error: Pointers are different!" << std::endl;
    }

	return 0;
}