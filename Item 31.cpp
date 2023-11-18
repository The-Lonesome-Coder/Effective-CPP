#include <memory>
#include <string>
#include <string_view>
/**
 * Minimize compilation dependencies between files.
*/

// Suppose a Person class
class Date { };

class Person
{
    public:
        Person(std::string_view name, std::string_view address, const Date& date)
            : m_name { name }, m_address { address }, m_birthday { date } {}

        std::string name() const { return m_name; }
        std::string address() const { return m_address; }
        Date birthday() const { return m_birthday; }


    private:
        std::string m_name;
        std::string m_address;
        Date m_birthday;
};


/**
 * To achieve the topic, you can implement the "pimpl idiom", which hides the implementation from the class.
 *
 * This type of class is called "handle class".
 *
 * With this design, clients of Person are divorced from the details of dates and persons.
 * The implementations of those classes can be modified at will, but Person clients need not recompile.
 * Additionally, because they're unable to see the details of Person's implementation,
 * clients are unlikely to write code that somehow depends on those details.
*/

/**
 * The key to this separation is replacement of dependencies on definitions with dependencies on declarations.
 * That’s the essence of minimizing compilation dependencies: make your header files self-sufficient whenever it’s practical,
 * and when it’s not, depend on declarations in other files, not definitions. Hence:
 *
 * 1. Avoid using objects when object references and pointers will do.
 *    You may define references and pointers to a type with only a declaration for the type.
 *    Defining objects of a type necessitates the presence of the type's definition.
 *
 * 2. Depend on class declarations instead of class definitions whenever you can.
 *    Note that you never need a class definition to declare a function using that class,
 *    not even if the function passes or returns the class type by value.
 *
 * 3. Provide separate header file for declarations and definitions.
*/
// Person2Implementation.h
class Person2Implementation
{
    public:
        Person2Implementation(std::string_view name, std::string_view address, const Date& date);

        std::string m_name;
        std::string m_address;
        Date m_birthday;
};


// Person2.h
class Person2
{
    public:
        Person2(std::string_view name, std::string_view address, const Date& date);

        std::string name() const;
        std::string address() const;
        Date birthday() const;


    private:
        std::shared_ptr<Person2Implementation> m_p_implementation;
};


// Person2Implementation.cpp
Person2Implementation::Person2Implementation(std::string_view name, std::string_view address, const Date& date)
    : m_name { name }, m_address { address }, m_birthday { date } {}


/**
 * An alternative to the handle class is to make Person a special kind of abstract base class, called "Interface class".
 * It typically has no data members, no constructors, a virtual destructor and a set of virtual functions that specify the interface.
 *
 * Clients of this class must program in terms of Person pointers and references,
 * because it’s not possible to instantiate classes containing pure virtual functions.
 *
 * Clients of an Interface class must have a way to create new objects.
 * They typically do it by calling a function that plays the role of the constructor for the derived classes that are actually instantiated.
 * Such functions are typically called factory functions (see Item 13) or virtual constructors.
 * They return pointers (preferably smart pointers — see Item 18) to dynamically allocated objects that support the Interface class’s interface.
*/
class Person3
{
    public:
        // Virtual constructor, return a std::shared_ptr tp a new Person initialized with the given parameters
        static std::shared_ptr<Person3> create(std::string_view name, std::string_view address, const Date& date);

        virtual ~Person3();

        virtual std::string name() const = 0;
        virtual std::string address() const = 0;
        virtual Date birthday() const = 0;
};


/**
 * At some point, of course, concrete classes supporting the Interface class’s interface must be defined and real constructors must be called.
 * That all happens behind the scenes inside the files containing the implementations of the virtual constructors.
*/
class RealPerson : public Person3
{
    RealPerson(std::string_view name, std::string_view address, const Date& date)
        : m_name { name }, m_address { address }, m_birthday { date } {}

    /* ... */

    private:
        std::string m_name;
        std::string m_address;
        Date m_birthday;
};