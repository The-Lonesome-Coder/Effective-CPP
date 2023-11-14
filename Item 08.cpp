#include <vector>
/**
 * Prevent exceptions from leaving destructors.
*/

/**
 * For instance, assume a vector that holds 10 class objects, when the vector is destroyed, it is responsible all class
 * objects. Suppose during destruction of the first one, an exception is thrown, either the program terminates or yields
 * undefined behavior.
*/
class Example
{
    public:
        ~Example() { };   // Assume it throws an exception
};

void doSomething()
{
    std::vector<Example> example;
}   // example will be destroyed here, but due to it throws exception,
    // either the program terminates or yields undefined behavior


/**
 * What should you do if your destructor needs to perform an operation that may fail by throwing an exception?
*/
class DatabaseConnection
{
    public:
        static DatabaseConnection create();   // Return DatabaseConnection objects

        void close();   // Close connection, throw an exception if closing fails
};

/**
 * To ensure that clients don’t forget to call close on DatabaseConnection objects, a reasonable idea would be to
 * create a resource-managing class for DatabaseConnection that calls close in its destructor.
*/
class DatabaseManage
{
    public:
        ~DatabaseManage()
        {
            db.close();   // Make sure database connections are always closed
        }

    private:
        DatabaseConnection db;
};


/**
 * This allows clients to program like this.
 *
 * This is fine as long as the call to close succeeds, but if the call yields an exception,
 * DBConn’s destructor will propagate that exception, i.e. allow it to leave the destructor.
*/
int main()
{
    // Create an DatabaseConnection object and turn it over to DatabaseManage object to manage
    DatabaseManage dbc { DatabaseConnection::create() };

    // Use the DatabaseConnection object via the DatabaseManage interface ...

    // At end of block, the DatabaseManage object is destroyed, thus automatically calling close on DatabaseConnection object

}


/**
 * Two primary ways to avoid the trouble:
 *
 * 1. Terminate the program
 * 2. Swallow the exception
*/
// Terminate the program
class DatabaseManage2
{
    public:
        ~DatabaseManage2()
        {
            try
            {
                db.close();
            }
            catch (...)
            {
                // Log entry that the call to close failed, then terminate
                std::abort();
            }
        }

    private:
        DatabaseConnection db;
};


// Swallow the exception - bad idea
class DatabaseManage3
{
    public:
        ~DatabaseManage3()
        {
            try
            {
                db.close();
            }
            catch (...)
            {
                // Log entry that the call to close failed, then terminate
            }
        }

    private:
        DatabaseConnection db;
};


/**
 * A better strategy is to design DBConn’s interface so that its clients have an opportunity to react to
 * problems that may arise.
*/
class DatabaseManage4
{
    public:
        void close()   // New function for client to use
        {
            db.close();
            closed = true;
        }

        ~DatabaseManage4()
        {
            if (!closed)   // Close the connection if the client didn't
            {
                try
                {
                    db.close();
                }
                catch (...)
                {
                    // Log entry that the call to close failed, then terminate
                    // Terminate or swallow
                }
            }
        }


    private:
        DatabaseConnection db;
        bool closed;
};

/**
 * If an operation may fail by throwing an exception and there may be a need to handle that exception,
 * the exception has to come from some non-destructor function.
 *
 * Destructors that emit exceptions are dangerous, always running the risk of premature program termination
 * or undefined behavior.
*/


/**
 * Destructors should never emit exceptions. If functions called in a destructor may throw,
 * the destructor should catch any exceptions, then swallow them or terminate the program.
 *
 * If class clients need to be able to react to exceptions thrown during an operation, the class should provide a
 * regular (i.e., non-destructor) function that performs the operation.
*/