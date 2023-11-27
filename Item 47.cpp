#include <stdexcept>
#include <iterator>
#include <typeinfo>
/**
 * Use traits classes for information about types.
 *
 * Traits classes make information about types available during compilation.
 * They’re implemented using templates and template specializations.
 *
 * In conjunction with overloading, traits classes make it possible to perform compile-time if...else tests on types.
*/

/**
 * The STL is primarily made up of templates for containers, iterators, and algorithms,
 * but it also has a few utility templates. One of these is called advance().
 * advance() moves a specified iterator a specified distance.
*/
template <typename Iterator, typename Distance>
void advance(Iterator& iter, Distance d);


/**
 * Given the different iterator capabilities, we really want to do is implement advance essentially like this.
 *
 * This requires being able to determine whether iter is a random access iterator,
 * which in turn requires knowing whether its type, Iterator, is a random access iterator type.
 * In other words, we need to get some information about a type.
 * That’s what traits let you do: they allow you to get information about a type during compilation.
*/
template <typename Iterator, typename Distance>
void advance(Iterator& iter, Distance d)
{
    if (/* iter is a random access iterator */)
    {
        iter += d;
    }
    else
    {
        if (d >= 0)
        {
            while (d--)
            {
                ++iter;
            }
        }
        else
        {
            while (d++)
            {
                --iter;
            }
        }
    }
}


/**
 * Traits aren’t a keyword or a predefined construct in C++; they’re a technique followed by C++ programmers.
 * One of the demands by the technique is that it has to work as well for built-in types as it does for user-defined types.
 *
 * The standard technique is to put it into a template and one or more specializations of that template.
 *
 * By convention, traits are always implemented as structs.
 * Another convention is that the structs used to implement traits are known as — traits classes.
*/
template <typename Iterator>
struct iterator_traits;


/**
 * The way iterator_traits works is that for each type IterT,
 * a typedef named iterator_category is declared in the struct iterator_traits<IterT>.
 * This typedef identifies the iterator category of IterT.
 *
 * iterator_traits implements this in two parts.
 * 1. It imposes the requirement that any user-defined iterator type must contain a nested "using" named
 *    iterator_category that identifies the appropriate tag struct.
 *
 * 2. The first point doesn’t work at all for iterators that are pointers,
 *    because there’s no such thing as a pointer with a nested typedef.
 *    The second part of the iterator_traits implementation handles iterators that are pointers.
*/
template </* ... */>
class deque
{
    public:
        class Iterator
        {
            public:
                using iterator_category = std::random_access_iterator_tag ;
        }
}

template </* ... */>
class list
{
    public:
    class Iterator
        {
            public:
                using iterator_category = std::bidirectional_iterator_tag;
        }
}


// iterator_traits just parrots back the iterator class’s nested typedef
template <typename Iterator>
struct iterator_traits
{
    using iterator_category = typename Iterator::iterator_category;
}


// Partial template specialization for built-in pointer types
template <typename T>
struct iterator_traits<T*>
{
    using iterator_category = std::random_access_iterator_tag;
}


/**
 * Given iterator_traits — actually std::iterator_traits, since it’s part of C++’s standard library —
 * we can refine our pseudo-code for advance().
 *
 * However, it will lead to compilation problems (Item 48). Also  Iterator's type is known during compilation,
 * so iterator_traits<Iterator>::iterator_category can also be determined during compilation.
 * Yet the if statement is evaluated at runtime.
*/
template <typename Iterator, typename Distance>
void advance(Iterator& iter, Distance d)
{
    if (typeid(typename std::iterator_traits<Iterator>::iterator_category) == typeid(std::random_access_iterator_tag))
    {
        /* ... */
    }
}


/**
 * What we really want is a conditional construct for types that is evaluated during compilation.
 *
 * When you overload some function f, you specify different parameter types for the different overloads.
 * When you call f, compilers pick the best overload, based on the arguments you’re passing.
 *
 * To get advance to behave the way we want, all we have to do is create multiple versions of an overloaded function
 * containing the “guts” of advance, declaring each to take a different type of iterator_category object.
*/
template <typename Iterator, typename Distance>
void DoAdvance(Iterator& iter, Distance d, std::random_access_iterator_tag)
{
    iter += d;
}


template <typename Iterator, typename Distance>
void DoAdvance(Iterator& iter, Distance d, std::bidirectional_iterator_tag)
{
    if (d >= 0)
    {
        while (d--)
        {
            ++iter;
        }
    }
    else
    {
        while (d++)
        {
            --iter;
        }
    }
}


template <typename Iterator, typename Distance>
void DoAdvance(Iterator& iter, Distance d, std::input_iterator_tag)
{
    if (d < 0)
    {
        if (d < 0)
        {
            throw std::out_of_range("Negative distance");
        }
    }

    while (d--)
    {
        ++iter;
    }
}


/**
 * Given the various overloads for doAdvance, all advance needs to do is call them,
 * passing an extra object of the appropriate iterator category type so that the compiler will use
 * overloading resolution to call the proper implementation.
*/
template <typename Iterator, typename Distance>
void advance(Iterator iter, Distance d)
{
    doAdvance(iter, d, typename std::iterator_traits<Iterator>::iterator_category());
}


/**
 * We can now summarize how to use a traits class:
 *
 * 1. Create a set of overloaded “worker” functions or function templates (e.g., doAdvance)
 *    that differ in a traits parameter. Implement each function in accord with the traits information passed.
 *
 * 2. Create a “master” function or function template (e.g., advance) that calls the workers,
 *    passing information provided by a traits class.
*/