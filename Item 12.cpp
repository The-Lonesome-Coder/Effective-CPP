#include <string>
/**
 * Copy all parts of an object.
*/

/**
 * Before another data member is added, the existing copying functions are working fine.
 * After that, the existing copying functions are performing a partial copy.
 *
 * If you add a data member to a class, you need to make sure that you update the copying functions and all the constructors.
 *
 * When you’re writing a copying function, be sure to
 * (1) copy all local data members
 * (2) invoke the appropriate copying function in all base classes, too.
*/
class Customer
{
    public:
        Customer(const Customer& rhs)                       // Copy constructor
            : m_name { rhs.m_name } { }

        Customer& operator = (const Customer& rhs)          // Copy assignment
        {
            m_name = rhs.m_name;

            return *this;
        }

    private:
        std::string m_name;
};

class VipCustomer : public Customer
{
    public:
        VipCustomer(const VipCustomer& rhs)
            : Customer(rhs), m_priority { rhs.m_priority } {}   // Remember to invoke base class copy constructor

        VipCustomer& operator = (const VipCustomer& rhs)
        {
            Customer::operator = (rhs);                         // Assign base class parts
            m_priority = rhs.m_priority;

            return *this;
        }

    private:
        int m_priority;
};


/**
 * Copying functions should be sure to copy all of an object’s data members and all of its base class parts.
 *
 * Don’t try to implement one of the copying functions in terms of the other.
 * Instead, put common functionality in a third function that both call.
*/