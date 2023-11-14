#include <string>
/**
 * Declare destructor virtual in polymorphic base class.
*/

/**
 * Because C++ specifies that when a derived class object is deleted through a pointer to a base class
 * with a non-virtual destructor, results are undefined, what typically happens at runtime is that the
 * derived part of the object is never destroyed, leading to a curious “partially destroyed” object.
 *
 * Solution: give the base class a virtual destructor. Then deleting a derived class object will do exactly what you want.
*/
class TimeKeeper
{
    public:
        TimeKeeper() {};
        virtual ~TimeKeeper() {};


    private:
        std::string m_name;
};

class AtomicClock : public TimeKeeper { };
class WaterClock : public TimeKeeper { };
class Watch : public TimeKeeper { };

// Returns a pointer to a dynamic-ally allocated object of a class derived from TimeKeeper
TimeKeeper* getTimeKeeper() { };

// Get dynamically allocated object from TimeKeeper hierarchy
TimeKeeper* ptk = getTimeKeeper();

// ... Do something

// Now will not lead to resource leak. However... using new/delete is strongly discouraged in C++
delete ptk;


/**
 * The implementation of virtual functions requires that objects carry information that can be used at runtime to
 * determine which virtual functions should be invoked on the object. This information typically takes the form of
 * a pointer called a vptr (“virtual table pointer”). The vptr points to an array of function pointers called a
 * vtbl (“virtual table”); each class with virtual functions has an associated vtbl. When a virtual function is
 * invoked on an object, the actual function called is determined by following the object’s vptr to a vtbl and
 * then looking up the appropriate function pointer in the vtbl.
 *
 * What is important is that if the Point class contains a virtual function, objects of that type will increase in size.
 * On a 32-bit architecture, they’ll go from 64 bits (for the two ints) to 96 bits (for the ints plus the vptr);
 * on a 64-bit architecture, they may go from 64 to 128 bits, because pointers on such architectures are 64 bits in size.
*/

/**
 * It is possible to get bitten by the non-virtual destructor problem even in the complete absence of virtual functions.
 * Including all STL container types, if you’re ever tempted to inherit from a standard container or any other class with
 * a non-virtual destructor, resist the temptation.
 *
 * In Effective C++, it mentioned that C++ offers no derivation-prevention mechanism akin to Java’s final classes. Since
 * the keyword "final" was introduced in C++11, C++ offered such mechanism; however, none of the STL library type are marked
 * "final".
*/

/**
 * The rule for giving base classes virtual destructors applies only to polymorphic base classes —
 * to base classes designed to allow the manipulation of derived class types through base class interfaces.
*/