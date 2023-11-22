/**
 * Understand the two meanings of typename.
*/

/**
 * The difference between class and typename in C++ template is nothing.
 * From C++’s point of view, class and typename mean exactly the same thing when declaring a template parameter.
 *
 * C++ doesn’t always view class and typename as equivalent, however.
*/
// Suppose we have a template for a function takes an STL-compatible container holding objects can be assigned to ints
template <typename T>
void print(const T& container)
{
    // Print 2nd element in container, not valid in C++
    if (container.size() >= 2)
    {
        T::const_iterator iter { container.cbegin() };  // Get iterator to 1st element
        ++iter;                                         // Move iter to 2nd element
        int value = *iter;                              // Copy that element to an int
        std::cout << value;                             // Print the int
    }
}

/**
 * Names in a template that are dependent on a template parameter are called dependent names.
 * When a dependent name is nested inside a class, it's called a nested dependent name.
 *
 * The other local variable has type int.
 * int is a name that does not depend on any template parameter. Such names are known as non-dependent names.
 * Nested dependent names can lead to parsing difficulties.
 *
 * Until T is known, there’s no way to know whether T::const_iterator is a type or isn’t,
 * and when the template print2nd is parsed, T isn’t known.
 *
 * C++ has a rule to resolve this ambiguity: if the parser encounters a nested dependent name in a template,
 * it assumes that the name is not a type unless you tell it otherwise.
 *
 * Now it should be clear why this isn’t valid C++.
 * The declaration of iter makes sense only if C::const_iterator is a type,
 * but we haven’t told C++ that it is, and C++ assumes that it’s not.
 * To rectify the situation, we have to tell C++ that C::const_iterator is a type.
*/
template <typename T>
void print(const T& container)
{
    if (container.size() >= 2)
    {
        typename T::const_iterator iter { container.cbegin() };

        /* ...Same as before... */
    }
}

/**
 * The general rule is simple: anytime you refer to a nested dependent type name in a template,
 * you must immediately precede it by the word typename.
 *
 * typename should be used to identify only nested dependent type names; other names shouldn’t have it.
*/
template <typename T>
void foo(const T& container, typename T::iterator iter);    // typename is requires for iter


/**
 * The exception to the “typename must precede nested dependent type names” rule is that typename must not
 * precede nested dependent type names in a list of base classes or as a base class identifier in a member
 * initialization list.
*/
template <typename T>
class Derived : public Base<T>::Nested          // Base class list: typename not allowed
{
    public:
        explicit Derived(int x)
            : Base<T>::Nested { x }             // Base class identifier in member initialization list, not allowed
        {
            typename Base<T>::Nested temp;      // Use of nested dependent type, typename needed
        }
};


/**
 * typename can also be used with "using" to simplify types.
*/
template <typename Iterator>
void foo(Iterator iter)
{
    using value_type = typename std::iterator_traits<Iterator>::value_type;

    value_type temp { *iter };
}