/**
 * Don't try to return a reference when you must return an object.
*/

/**
 * A reference is just a name, a name for some existing object.
 * Whenever you see the declaration for a reference, you should immediately
 * ask yourself what it is another name for, because it must be another name for something.
*/

/**
 * Suppose you have a class that has an operator overload
 *
 * This function returns a reference to result, but result is a local object,
 * and local objects are destroyed when the function exits.
*/
class Rational
{
    public:
        Rational(int denominator, int numerator)
            : m_denominator { denominator }, m_numerator { numerator } {}

        Rational& operator * (const Rational& rhs)   // Wrong. Never return a reference to local object
        {
            Rational result { this->m_denominator * rhs.m_denominator, this->m_numerator * rhs.m_numerator };

            return result;
        }

    private:
        int m_denominator;
        int m_numerator;
};


/**
 *  The right way is to return a new object.
*/
class Rational2
{
    public:
        Rational2(int denominator, int numerator)
            : m_denominator { denominator }, m_numerator { numerator } {}

        const Rational2 operator * (const Rational2& rhs)   // Wrong. Never return a reference to local object
        {
            Rational2 result { this->m_denominator * rhs.m_denominator, this->m_numerator * rhs.m_numerator };

            return result;
        }

    private:
        int m_denominator;
        int m_numerator;
};


/**
 * Never return a pointer or reference to a local stack object, a reference to a heap-allocated object,
 * or a pointer or reference to a local static object if there is a chance that more than one such object will be needed.
*/