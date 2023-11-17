#include <utility>
#include <vector>
/**
 * Consider support for a non-throwing swap.
*/

/**
 * The default swap implementation may not thrill you. It involves copying three objects: a to temp, b to a, and temp to b.
 * For some types, none of these copies are really necessary.
 * For such types, the default swap puts you on the fast track to the slow lane.
 * Such as vectors, which is expensive to copy.
 *
 * If the default implementation of swap offers acceptable efficiency for your class or class template,
 * you don’t need to do anything.
 *
 * If the default implementation of swap isn’t efficient enough, do the following:
 *
 * 1. Offer a public swap member function that efficiently swaps the value of two objects of your type.
 * 2. Offer a non-member swap in the same namespace as your class or template. Have it call your swap member function.
 * 3. If you’re writing a class (not a class template), specialize std::swap for your class.
 *    Have it also call your swap member function.
*/
class ExampleImplementation
{
    private:
        int a;
        std::vector<double> vec;
};


template <typename T>
class Example
{
    public:
        // Public swap function
        void swap(Example& other)
        {
            std::swap(m_p_ExImp, other.m_p_ExImp);
        }

    private:
        ExampleImplementation* m_p_ExImp;
};


// Non-member swap
template <typename T>
void swap(Example<T>& one, Example<T>& other)
{
    one.swap(other);
}


/**
 * Provide a swap member function when std::swap would be inefficient for your type.
 * Make sure your swap doesn’t throw exceptions.
 *
 * If you offer a member swap, also offer a non-member swap that calls the member.
 * For classes (not templates), specialize std::swap, too.
 *
 * When calling swap, employ a using declaration for std::swap, then call swap without namespace qualification.
 *
 * It’s fine to totally specialize std templates for user-defined types,
 * but never try to add something completely new to std.
*/