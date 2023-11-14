#include <memory>
/**
 * Use objects to manage resources.
*/

/**
 * In Effective C++, it suggests using "auto_ptr" and "std::tr1::shared_ptr".
 * The former has been deprecated in C++17, and the latter has been moved into <memory> header.
 * Therefore, the core point in this item is:
 *
 * 1. Resources are acquired and immediately turned over to resource-managing objects.
 *    The idea of using objects to manage resources is often called Resource Acquisition Is Initialization (RAII),
 *    because it’s so common to acquire a resource and initialize a resource-managing object in the same statement.
 *
 * 2. Resource-managing objects use their destructors to ensure that resources are released.
 *    Because destructors are called automatically when an object is destroyed (e.g., when an object goes out of scope),
 *    resources are correctly released, regardless of how control leaves a block.
 *
 * The suggestion of using smart pointer is still valid. See: https://en.cppreference.com/w/cpp/header/memory
*/
class Example
{
    /*...*/
};

// With raw pointer, it will lead to potential memory leak.
Example* ex;

// Using smart pointer can prevent such incident.
std::shared_ptr<Example> ex2;