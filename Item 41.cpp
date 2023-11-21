#include <cstddef>
/**
 * Understand implicit interfaces and compile-time polymorphism.
*/

/**
 * Explicit interfaces and runtime polymorphism in the world of object-oriented programming is important.
 *
 * Because ex is declared to be of type Example, ex must support the Example interface.
 * We can look up this interface in the source code to see exactly what it looks like,
 * so this an explicit interface — one explicitly visible in the source code.
 *
 * Because some of Example’s member functions are virtual, ex’s calls to those functions will exhibit runtime polymorphism.
*/
class Example
{
    public:
        Example();
        virtual ~Example();

        virtual std::size_t size() const;
        virtual void normalize();
        void swap(Example& other);
};

void processing(Example& ex)
{
    if (ex.size() > 10)
    {
        Example temp { ex };
        temp.normalize();
        temp.swap(ex);
    }
}


/**
 * Explicit interfaces and runtime polymorphism in the world of templates and generic programming is less important.
 * Instead, implicit interfaces and compile-time polymorphism move to the fore.
 *
 * The interface that ex must support is determined by the operations performed on w in the template.
 *
 * The calls to functions involving w such as operator > and operator != may involve instantiating templates
 * to make these calls succeed. Such instantiation occurs during compilation.
*/
template<typename T>
void processing(T& ex)
{
    /**
     * The implicit interface for T (ex’s type) appears to have this constraint:
     * It must offer a member function named size that returns an integral value.
    */
    if (ex.size() > 10)
    {
        T temp { ex };
        temp.normalize();
        temp.swap(ex);
    }
}


/**
 * C++20 provides "concept" to check the object before using the template.
 * Implicit interface now is more explicit due to the new "concept" feature.
*/