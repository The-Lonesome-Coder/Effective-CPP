#include <list>
#include <iterator>
#include <typeinfo>
/**
 * Be aware of template meta-programming.
*/

/**
 * Template meta-programming (TMP) is the process of writing template based C++ programs that execute during compilation.
 *
 * TMP has two great strengths:
 *
 * 1. It makes some things easy that would otherwise be hard or impossible.
 * 2. Because template meta-programs execute during C++ compilation, they can shift work from runtime to compile-time.
*/

/**
 * Consider the pseudo-code for STL’s advance in Item. We can use typeid to make the pseudo-code real.
 *
 * Item 47 notes that this typeid-based approach is less efficient than the one using traits, because:
 *
 * 1. The type testing occurs at runtime instead of during compilation.
 * 2. The code to do the runtime type testing must be present in the executable.
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
 * Item 47 mentions that the typeid-based implementation of advance can lead to compilation problems.
 *
 * The version of advance that will be generated will be the following.
 *
 * The problem is the using +=. In this case, we’re trying to use += on a list<int>::iterator,
 * but list<int>::iterator is a bidirectional iterator (see Item 47), so it doesn’t support +=.
 * However, compilers are obliged to make sure that all source code is valid, even if it’s not executed,
 * and “iter += d” isn’t valid when iter isn’t a random access iterator.
*/
std::list<int>::iterator iter;

advance(iter, 10);   // Won't compiler with above implementation


void advance(std::list<int>::iterator& iter, int d)
{
    if (typeid(typename std::iterator_traits<std::list<int>::iterator>::iterator_category) == typeid(std::random_access_iterator_tag))
    {
        iter += d;      // Error, won't compile
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
 * For another glimpse into how things work in TMP, let’s look at loops.
 *
 * TMP has no real looping construct, so the effect of loops is accomplished via recursion.
 * Even the recursion isn’t the normal kind, however, because TMP loops don’t involve recursive function calls,
 * they involve recursive template instantiations.
 *
 * The “hello world” program of TMP is computing a factorial during compilation.
 *
 * Given this template meta-program, you get the value of factorial(n) by referring to Factorial<n>::value.
 *
 * The looping part of the code occurs where the template instantiation Factorial<n> references the
 * template instantiation Factorial<n-1>.
*/
// General case: the value of Factorial<n> is n times the value of Factorial<n - 1>
template <unsigned int n>
struct Factorial
{
    enum { value = n * Factorial<n - 1>::value };
};

template <>
struct Factorial<0>
{
    enum { value = 1 };
};


/**
 * To grasp why TMP is worth knowing about, it’s important to have a better understanding of what it can accomplish:
 *
 * 1. Ensuring dimensional unit correctness. In scientific and engineering applications,
 *    it’s essential that dimensional units (e.g., mass, distance, time, etc.) be combined correctly.
 *    Using TMP, it’s possible to ensure that all dimensional unit combinations in a program are correct,
 *    no matter how complex the calculations.
 *
 * 2. Optimizing matrix operations. Calculating large matrices in the “normal” way calls for the creation
 *    of temporary matrices. Using an advanced template technology related to TMP called expression templates,
 *    it’s possible to eliminate the temporaries and merge the loops. The resulting software uses less memory
 *    and runs dramatically faster.
 *
 * 3. Generating custom design pattern implementations. Design patterns can be implemented in many ways.
 *    Using a TMP-based technology called policy-based design, it’s possible to create templates representing
 *    independent design choices that can be combined in arbitrary ways to yield pattern implementations
 *    with custom behavior. For example, this technique has been used to allow a few templates implementing
 *    smart pointer behavioral policies to generate (during compilation) any of hundreds of different smart
 *    pointer types. Generalized beyond the domain of programming artifacts like design patterns and smart pointers,
 *    this technology is a basis for what’s known as generative programming.
*/