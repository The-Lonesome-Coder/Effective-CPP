#include <string>
#include <string_view>
/**
 * Never call virtual functions during construction or destruction.
*/

// Suppose you’ve got a class hierarchy for modeling stock transactions
class Transaction
{
    public:
        Transaction()
        {
            // Do something
            logTransaction();
        }

        virtual void logTransaction() const = 0;   // Log the transaction
};


class BuyTransaction : public Transaction
{
    public:
        virtual void logTransaction() const;
};


class SellTransaction : public Transaction
{
    public:
        virtual void logTransaction() const;
};


/**
 * When you execute the following code, a BuyTransaction constructor will be called,
 * but first, a Transaction constructor must be called.
 *
 * The last line of the Transaction constructor calls the virtual function logTransaction,
 * but the version of logTransaction that’s called is the one in Transaction.
 *
 * During base class construction, virtual functions never go down into derived classes.
 * Instead, the object behaves as if it were of the base type.
 *
 * Because base class constructors execute before derived class constructors,
 * derived class data members have not been initialized when base class constructors run.
 *
 * The same reason apply to destructors.
*/
BuyTransaction transaction;


/**
 * To ensure that the proper version of logTransaction is called each time an object
 * in the Transaction hierarchy is created, you can: Turn logTransaction into a non-virtual function in Transaction.
*/
class Transaction2
{
    public:
        explicit Transaction2(std::string_view logInfo)
        {
            logTransaction(logInfo);
        }

        void logTransaction(std::string_view logInfo) const;   // Now a non-virtual function
};


class BuyTransaction2 : public Transaction2
{
    public:
        BuyTransaction2(...)
            : Transaction2(createLogString(/*...*/)) { }

    private:
        static std::string createLogString(/*...*/);
};


/**
 * Don’t call virtual functions during construction or destruction,
 * because such calls will never go to a more derived class than that of the currently executing constructor or destructor.
*/