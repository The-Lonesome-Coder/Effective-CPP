#include <memory>
/**
 * Think carefully about copying behavior in resource-managing classes.
 *
 * This item is still valid; however, using the "std::shared_ptr" in <memory> library is simpler.
*/

// Suppose you are using a C API to manipulate mutual exclusion (mutex) object, offering lock() and unlock() function
class Mutex { };

void lock(Mutex *p_m);
void unlock(Mutex *p_m);

// To make sure you never forget to unlock a Mutex you've locked, you'd like to create a class to manage locks
class Lock
{
    public:
        explicit Lock(Mutex *p_m)
            : m_mutexPtr { p_m }
        {
            lock(m_mutexPtr);   // Acquire resources
        }

        ~Lock()
        {
            unlock(m_mutexPtr);   // Release resources
        }

    private:
        Mutex* m_mutexPtr;
};


// The client use Lock in the conventional RAII function - fine
Mutex m;   // Define the mutex you need to use

int main()
{
    /*...*/

    Lock ml(&m);   // Lock the mutex

    /*...*/   // Perform some operations
}   // Automatically unlock mutex at end of block


// Something weird will happen if someone try to do this
Lock ml1(&m);
Lock ml2(ml1);


/**
 * Most of the time, you can choose one of the following possibilities:
*/
/**
 * 1. Prohibiting copying (see Item 06).
 *
 * In many cases, it makes no sense to allow RAIIobjects to be copied.
 * This is likely to be true for a class like Lock,
 * because it rarely makes sense to have “copies” of synchronization primitives.
 * When copying makes no sense for an RAII class, you should prohibit it.
*/
class Lock2
{
    public:
        /**
         * Same as before ...
        */

        Lock2(Lock2& rhs) = delete;
        Lock2& operator = (Lock2& rhs) = delete;
};


/**
 * 2. Reference-count the underlying resource.
 *
 * Sometimes it’s desirable to hold on to a resource until the last object using it has been destroyed.
 * Fortunately, std::shared_ptr allows specification of a “deleter” — a function or function object to be called
 * when the reference count goes to zero.
 *
 * In this example, notice how the Lock class no longer declares a destructor.
 * mutexPtr’s destructor will automatically call the tr1::shared_ptr’s deleter — unlock,
 * in this case — when the mutex’s reference count goes to zero.
*/
class Lock3
{
    public:
        explicit Lock3(Mutex *p_m)
            // Initialize std::shared_ptr with the Mutex to point to and unlock function as deleter
        {
            lock(p_m);
            m_mutexPtr.reset(p_m, unlock);
        }

    private:
        std::shared_ptr<Mutex> m_mutexPtr;   // Use std::shared_ptr instead of raw pointer
};


/**
 * 3. Copy the underlying resource.
 *
 * Sometimes you can have as many copies of a resource as you like, and the only reason you need a
 * resource-managing class is to make sure that each copy is released when you’re done with it.
 * In that case, copying the resource-managing object should also copy the resource it wraps.
 * That is, copying a resource-managing object performs a “deep copy.”
*/


/**
 * 4. Transfer ownership of the underlying resource.
 *
 * On rare occasion, you may wish to make sure that only one RAII object refers to a raw resource
 * and that when the RAII object is copied, ownership of the resource is transferred from the copied object
 * to the copying object.
 *
 * A "std::unique_ptr" can accomplish this, see https://en.cppreference.com/w/cpp/header/memory.
*/