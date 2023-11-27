#include <cstddef>
#include <new>
/**
 * Adhere to convention when writing new and delete.
 *
 * operator new should contain an infinite loop trying to allocate memory, should call the new-handler if it can’t
 * satisfy a memory request, and should handle requests for zero bytes. Class-specific versions should handle requests
 * for larger blocks than expected.
 *
 * operator delete should do nothing if passed a pointer that is null.
 * Class-specific versions should handle blocks that are larger than expected.
*/

/**
 * C++ requires that operator new return a legitimate pointer even when zero bytes are requested.
 *
 * The trick of treating requests for zero bytes as if they were really requests for one byte looks slimy,
 * but it’s simple, it’s legal, it works.
 *
 * The new-handling function pointer is set to null, then promptly reset to what it was originally.
 * There is no way to get at the new-handling function pointer directly, so you have to call set_new_handler
 * to find out what it is.
 *
 * Item 49 remarks that operator new contains an infinite loop, and the pseudo-code shows that loop explicitly;
 * “while (true)” is about as infinite as it gets. The only way out of the loop is for memory to be successfully
 * allocated or for the new-handling function to do one of the things described in Item 49.
*/
// Pseudo-code
void* operator new (std::size_t size) throw(std::bad_alloc)
{
    // Handle 0-byte requests by treating them as 1-byte requests
    if (size == 0)
    {
        size = 1;
    }

    while (true)
    {
        /* Attempt to allocate size bytes */

        if (/* The allocation is unsuccessful */)
        {
            return /* A pointer to the memory */
        }

        // Allocation was unsuccessful. Find out what the current new-handling function is
        std::new_handler globalHandler = std::set_new_handler(0);
        std::set_new_handler(globalHandler);

        if (globalHandler)
        {
            (*globalHandler)();
        }
        else
        {
            throw std::bad_alloc();
        }
    }
}


/**
 * operator new member functions are inherited by derived classes.
 *
 * As Item 50 explains, one of the most common reasons for writing a custom memory manager is to optimize allocation
 * for objects of a specific class, not for a class or any of its derived classes.
 *
 * That is, given an operator new for a class X, the behavior of that function is typically tuned for objects of size
 * sizeof(X) — nothing larger and nothing smaller.
 *
 * Because of inheritance, however, it is possible that the operator new in a base class will be called to allocate
 * memory for an object of a derived class.
*/
class Base
{
    public:
        static void* operator new (std::size_t size) throw(std::bad_alloc);
};


class Derived : public Base { };

Derived *pointer = new Derived;     // Calls Base::operator new


/**
 * If Base’s class-specific operator new wasn’t designed to cope with this, the best way for it to handle the
 * situation is to slough off calls requesting the “wrong” amount of memory to the standard operator new.
*/
void* Base::operator new (std::size_t size) throw(std::bad_alloc)
{
    /**
     * If size is wrong, have standard operator new handle the request
     * The size-is-zero test is still here, it’s just been incorporated into the test of size against sizeof(Base)
     * By definition, sizeof(Base) can never be zero, so if size is zero, the request will be forwarded to ::operator new,
     * and it will become that function’s responsibility to treat the request in a reasonable fashion.
    */
    if (size != sizeof(Base))
    {
        return ::operator new(size);
    }
}


/**
 * If you’d like to control memory allocation for arrays on a per-class basis, you need to implement operator new’s
 * array-specific cousin, operator new[]. If you decide to write operator new[], remember that all you’re doing is
 * allocating a chunk of raw memory — you can’t do anything to the as-yet-nonexistent objects in the array.
 *
 * A base class’s operator new[] might, through inheritance, be called to allocate memory for an array of derived class
 * objects, and derived class objects are usually bigger than base class objects. Hence, you can’t assume inside
 * Base::operator new[] that the size of each object going into the array is sizeof(Base), and that means you can’t
 * assume that the number of objects in the array is (bytes requested)/sizeof(Base).
 *
 * The size_t parameter passed to operator new[] may be for more memory than will be filled with objects,
 * because, as Item 16 explains, dynamically allocated arrays may include extra space to store the number of array elements.
*/


/**
 * All you need to remember for operator delete is that C++ guarantees it’s always safe to delete the null pointer,
 * so you need to honor that guarantee.
*/
void operator delete(void* rawMemory) throw()
{
    if (rawMemory == 0)
    {
        // Do nothing if the null pointer is being deleted
        return;
    }

    /* Deallocate the memory pointed to by rawMemory */
}


class Base2
{
    public:
        static void* operator new(std::size_t size) throw(std::bad_alloc);
        static void operator delete(void *rawMemory, std::size_t size) throw();
};

void Base2::operator delete (void *rawMemory, std::size_t size) throw()
{
    // Check for null pointer
    if (rawMemory == 0)
    {
        return;
    }

    if (size != sizeof(Base))
    {
        // If size is “wrong”, have standard operator delete handle the request
        ::operator delete(rawMemory);

        return;
    }
}