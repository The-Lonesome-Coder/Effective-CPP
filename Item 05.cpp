#include <string>
#include <string_view>
/**
 * Know what functions C++ silently writes and calls.
*/

/**
 * The compilers will declare their own versions of a copy constructor, a copy assignment operator, and a destructor,
 * even if you don't declare them yourself.
*/
class Empty {};

// The compiler will generate these
class Empty
{
    public:
        Empty() { }                                     // Default constructor
        Empty(const Empty& rhs) { }                     // Copy constructor
        ~Empty() { }                                    // Destructor - non-virtual unless inherited from a base class
        Empty& operator = (const Empty& rhs) {  }       // Copy assignment operator
};


/**
 * If you declared a constructor, compiler won’t generate a default constructor.
 * It means that if you’ve carefully engineered a class to require constructor arguments,
 * you don’t have to worry about compilers overriding your decision by adding a constructor that takes no arguments.
*/
template <typename T>
class Object
{
    public:
        Object(std::string_view name, const T& value);   // Self-declared constructor, therefore, no compiler constructor
        // No self-declared copy constructor or copy assignment, therefore, compilers will generate those functions
    private:
        std::string m_name;
        T m_value;
};