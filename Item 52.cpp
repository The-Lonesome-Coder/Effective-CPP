#include <iostream>
#include <new>
/**
 * Write placement delete if you write placement new.
 *
 * When you write a placement version of operator new, be sure to write the corresponding placement version of
 * operator delete. If you don’t, your program may experience subtle, intermittent memory leaks.
 *
 * When you declare placement versions of new and delete, be sure not to unintentionally hide the normal versions
 * of those functions.
*/

/**
 * The which-delete-goes-with-this-new issue does arise, however, when you start declaring non-normal forms of
 * operator new — forms that take additional parameters.
 *
 * Suppose you write a class-specific operator new that requires specification of an ostream to which allocation
 * information should be logged, and you also write a normal class-specific operator delete.
 *
 * When an operator new function takes extra parameters, that function is known as a placement version of new.
 *
 * This class will give rise to subtle memory leaks. If memory allocation succeeds and the Example constructor
 * throws an exception, the runtime system is responsible for undoing the allocation that operator new performed.
 * However, the runtime system can’t really understand how the called version of operator new works,
 * so it can’t undo the allocation itself. Instead, the runtime system looks for a version of operator delete that
 * takes the same number and types of extra arguments as operator new, and, if it finds it, that’s the one it calls.
*/
class Example
{
    public:
        // Non-normal form of new
        static void* operator new(std::size_t size, std::ostream& log) throw(std::bad_alloc);

        // Normal class-specific form of delete
        static void operator delete (void* p_memory, std::size_t size) throw();
};

// Call operator new, passing std::cerr as the ostream, this leaks memory if the Example constructor throws
Example *p_example = new (std::cerr) Example;



// With class-specific form of delete, no leak will happen
class Example2
{
    public:
        // Non-normal form of new
        static void* operator new(std::size_t size, std::ostream& log) throw(std::bad_alloc);

        // Non-normal class-specific form of delete
        static void operator delete (void* p_memory, std::ostream& log) throw();
};

// No leak this time
Example2 *p_example2 = new (std::cerr) Example2;


// However, consider what happens if no exception is thrown and we get to a delete in client code
delete p_example2;   // Invokes the normal operator delete


/**
 * As the comment indicates, this calls the normal operator delete, not the placement version.
 * Placement delete is called if an exception arises from a constructor call that’s coupled to a call to a placement new.
 * Applying delete to a pointer (such as pw above) never yields a call to a placement version of delete.
 *
 * This means that to forestall all memory leaks associated with placement versions of new, you must provide both the
 * normal operator delete and a placement version that takes the same extra arguments as operator new does.
 *
 * Incidentally, because member function names hide functions with the same names in outer scopes (see Item 33),
 * you need to be careful to avoid having class-specific news hide other news (including the normal versions)
 * that your clients expect. If you declare any operator news in a class, you’ll hide all the standard forms below.
 *
 *  Unless you mean to prevent class clients from using the standard forms, be sure to make them available in addition
 * to any custom operator new forms you create.
*/
// C++ offers the following forms of operator new at global scope
void* operator new(std::size_t) throw(std::bad_alloc); // normal new
void* operator new(std::size_t, void*) throw(); // placement new
void* operator new(std::size_t, const std::nothrow_t&) throw(); // Nothrow new


// Create a base class containing all the normal forms of new and delete
class StandardNewDeleteForms
{
    public:
        // normal new/delete
        static void* operator new(std::size_t size) throw(std::bad_alloc)
        { return ::operator new(size); }

        static void operator delete(void *pMemory) throw()
        { ::operator delete(pMemory); }


        // placement new/delete
        static void* operator new(std::size_t size, void *ptr) throw()
        { return ::operator new(size, ptr); }

        static void operator delete(void *pMemory, void *ptr) throw()
        { return ::operator delete(pMemory, ptr); }


        // nothrow new/delete
        static void* operator new(std::size_t size, const std::nothrow_t& nt) throw()
        { return ::operator new(size, nt); }

        static void operator delete(void *pMemory, const std::nothrow_t&) throw()
        { ::operator delete(pMemory); }
};