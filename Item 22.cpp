/**
 * Declare data members private.
*/

/**
 * Why not public data members?
 *
 * 1.  If everything in the public interface is a function, clients won’t have to scratch their heads trying to
 *     remember whether to use parentheses when they want to access a member of the class.
 *
 * 2. Encapsulation.
 *    2-1. If you implement access to a data member through a function, you can later replace the data
 *          member with a computation, and nobody using your class will be any the wiser.
 *    2-2. If you hide your data members from your clients,  you can ensure that class invariants are always
 *         maintained, because only member functions can affect them.
 *    2-3. You reserve the right to change your implementation decisions later.
*/
class AccessLevel
{
    public:
        // These are called getters and setters
        int getReadOnly() const
        {
            return m_readOnly;
        }

        void setReadWrite(int value)
        {
            m_readWrite = value;
        }

        int getReadWrite() const
        {
            return m_readWrite;
        }

        void setWriteOnly(int value)
        {
            m_writeOnly = value;
        }

    private:
        int m_noAccess;
        int m_readOnly;
        int m_readWrite;
        int m_writeOnly;
};


/**
 * Declare data members private. It gives clients syntactically uniform access to data,
 * affords fine-grained access control, allows invariants to be enforced,
 * and offers class authors implementation flexibility.
*/