#include <memory>
/**
 * Provide access to raw resources in resource-managing classes.
*/

/**
 * Many APIs refer to resources directly, so unless you plan to foreswear use of such APIs
 * (something that’s rarely practical), you’ll have to bypass resource-managing objects and
 * deal with raw resources from time to time.
*/

/**
 * For example, Item 13 introduces the idea of using smart pointers like std::shared_ptr to hold
 * the result of a call to a factory function like createInvestment:
*/
class Investment
{
    public:
        bool isTaxFree() const;
};

std::shared_ptr<Investment> p_Investment;

// Suppose you'd like to use when working with investment objects
int daysHeld(const Investment* p_Investment);    // Return number of days investment has been held

// You'd like to call it like this
int days1 = daysHeld(p_Investment);   // Error, cannot implicitly convert smart pointer to raw pointer


// There are two general ways to do it: explicit conversion and implicit conversion
int days2 = daysHeld(p_Investment.get());   // Passes the raw pointer using get() function provided by std::shared_ptr

bool taxable = p_Investment->isTaxFree();   // Implicit conversion


/**
 * APIs often require access to raw resources, so each RAII class should offer a way to get at the resource it manages.
 *
 * Access may be via explicit conversion or implicit conversion. In general, explicit conversion is safer,
 * but implicit conversion is more convenient for clients.
*/