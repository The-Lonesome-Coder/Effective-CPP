#include <string>
#include <string_view>
/**
 * Differentiate between inheritance of interface and inheritance of implementation.
*/

/**
 * As a class designer, you sometimes want derived classes to inherit only the interface (declaration) of a member function.
 * Sometimes you want derived classes to inherit both a function’s interface and implementation,
 * but you want to allow them to override the implementation they inherit.
 * And sometimes you want derived classes to inherit a function’s interface and implementation without allowing them to override anything.
 *
 * Member function interfaces are always inherited (Item 32).
*/
// Assume an abstract class
class Shape
{
    public:
        /**
         * The two most salient features of pure virtual functions are that they must be re-declared by any concrete class that inherits them,
         * and they typically have no definition in abstract classes.
         *
         * The purpose of declaring a pure virtual function is to have derived classes inherit a function interface only.
        */
        virtual void draw() const = 0;

        /**
         * Derived classes inherit the interface of the function, but simple virtual functions provide an implementation that derived classes may override.
         *
         * The purpose of declaring a simple virtual function is to have derived classes inherit a function interface as well as a default implementation.
        */
        virtual void error(std::string_view message);

        /**
         * When a member function is non-virtual, it’s not supposed to behave differently in derived classes.
         * In fact, a non-virtual member function specifies an invariant over specialization,
         * because it identifies behavior that is not supposed to change, no matter how specialized a derived class becomes.
         *
         * The purpose of declaring a non-virtual function is to have derived classes inherit a function interface as well as a mandatory implementation.
        */
        int id() const;
};

class Rectangle : public Shape { };
class Ellipse : public Shape { };


/**
 * It can be dangerous to allow simple virtual functions to specify both a function interface and a default implementation.
*/
// Suppose a hierarchy of airplanes in XYZ airline
class Airport { };   // Represents airport

class Airplane
{
    virtual void fly(const Airport& destination);
};

void Airplane::fly(const Airport& destination)
{
    /* Default code for flying airplane. */
}

class AirplaneA : public Airplane { };
class AirplaneB : public Airplane { };

/**
 * Now suppose that XYZ, its fortunes on the rise, decides to acquire a new type of airplane, the Model C.
 * The Model C differs in some ways from the Model A and the Model B. In particular, it is flown differently.
 *
 * This is a disaster: an attempt is being made to fly a ModelC object as if it were a ModelA or a ModelB.
*/
class AirplaneC : public Airplane { };


/**
 * The best solution is to take advantage of the fact that pure virtual functions must be re-declared in concrete derived classes,
 * but they may also have implementations of their own.
*/
class Airplane2
{
    public:
        virtual void fly(const Airport& destination) = 0;
};

// An implementation of a pure virtual function
void Airplane2::fly(const Airport& destination)
{
    /* ... */
}

class AirplaneA2 : public Airplane2
{
    public:
        virtual void fly(const Airport& destination);
};

void AirplaneA2::fly(const Airport& destination)
{
    Airplane2::fly(destination);
}

class AirplaneC2 : public Airplane2
{
    public:
        virtual void fly(const Airport& destination);
};

void AirplaneC2::fly(const Airport& destination)
{
    /* ... */
}