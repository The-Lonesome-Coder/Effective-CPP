#include <new>
/**
 * Understand the behavior of the new-handler.
*/

/**
 * Using new/delete is discouraged in modern code, tread lightly with them.
 *
 * Consider using smart pointer.
*/


/**
 * When operator new is unable to fulfill a memory request, it calls the new-handler function repeatedly
 * until it can find enough memory. A well-designed new handler function must do one of the following:
 *
 * 1. Make more memory available. One way to implement this strategy is to allocate a large block of memory
 *    at program start-up, then release it for use in the program the first time the new-handler is invoked.
 *
 * 2. Install a different new-handler.  If the current new-handler can’t make any more memory available,
 *    perhaps it knows of a different new-handler that can. If so, the current new-handler can install
 *    the other new-handler in its place.
 *
 * 3. De-install the new-handler. With no new-handler installed, operator new will throw an exception when memory
 *    allocation is unsuccessful.
 *
 * 4. Throw an exception. bad_alloc or some type derived from bad_alloc.
 *
 * 5. Not return. By calling abort or exit.
*/

/**
 * Suppose you want to handle memory allocation failures for the Example class.
 * You’ll have to keep track of the function to call when operator new can’t allocate enough memory for a Example object,
 * so you’ll declare a static member of type new_handler to point to the new-handler function for the class.
 *
 * Example's operator new will do the following:
 *
 * 1. Call the standard set_new_handler with Widget’s error-handling function.
 *    This installs Example’s new-handler as the global new handler.
 *
 * 2. Call the global operator new to perform the actual memory allocation.
 *    If allocation fails, the global operator new invokes Example’s new-handler,
 *    because that function was just installed as the global new-handler.
 *    If the global operator new is ultimately unable to allocate the memory, it throws a bad_alloc exception.
 *    In that case, Example’s operator new must restore the original global new handler, then propagate the exception.
 *
 * 3. If the global operator new was able to allocate enough memory for a Example object,
 *    Example’s operator new returns a pointer to the allocated memory.
*/
class Example
{
    public:
        static std::new_handler set_new_handler(std::new_handler p) throw();
        static void* operator new (std::size_t size) throw(std::bad_alloc);


    private:
        static inline std::new_handler currentHandler = 0;
};


// The set_new_handler function in Widget will save whatever pointer is passed to it,
// and it will return whatever pointer had been saved prior to the call
std::new_handler Example::set_new_handler(std::new_handler p) throw()
{
    std::new_handler oldHandler = currentHandler;
    currentHandler = p;

    return oldHandler;
}


/**
 * With a resource-handling class, the implementation of Example's operator new is quite simple.
*/
class NewHandlerHolder
{
    public:
        // Acquire current new-handler
        explicit NewHandlerHolder(std::new_handler newHandler)
            : m_handler { newHandler } {}

        // Release it
        ~NewHandlerHolder()
        {
            std::set_new_handler(m_handler);
        }

        // Prevent copying
        NewHandlerHolder(const NewHandlerHolder&) = delete;
        NewHandlerHolder& operator = (const NewHandlerHolder&) = delete;



    private:
        std::new_handler m_handler;
};


void* Example::operator new (std::size_t size) throw(std::bad_alloc)
{
    // Install Example's new handler
    NewHandlerHolder handler { std::set_new_handler(currentHandler) };

    // Allocate memory or throw
    return ::operator new (size);
}


/**
 * The code for implementing this scheme is the same regardless of the class,
 * so a reasonable goal would be to reuse it in other places.
 *
 * An easy way to make that possible is to create a “mixin-style” base class ,
 * a base class that’s designed to allow derived classes to inherit a single specific capability.
 * In this case, the ability to set a class-specific new handler.
 * Then turn the base class into a template, so that you get a different copy of the class data for each inheriting class.
 *
 * With this class template, adding set_new_handler support to Example is easy.
*/
template <typename T>
class NewHandlerSupport
{
    public:
        static std::new_handler set_new_handler(std::new_handler p) throw();
        static void* operator new (std::size_t size) throw(std::bad_alloc);


    private:
        static inline std::new_handler currentHandler = 0;
};


template<typename T>
std::new_handler NewHandlerSupport<T>::set_new_handler(std::new_handler p) throw()
{
    std::new_handler oldHandler = currentHandler;
    currentHandler = p;

    return oldHandler;
}


template<typename T>
void* NewHandlerSupport<T>::operator new(std::size_t size) throw(std::bad_alloc)
{
    NewHandlerHolder h(std::set_new_handler(currentHandler));

    return ::operator new(size);
}

class Example2 : public NewHandlerSupport<Example> { /* Same as before */ };