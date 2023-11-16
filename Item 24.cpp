/**
 * Declare non-member functions when type conversions should apply to all parameters.
*/

/**
 * Most of the time, having classes support implicit type conversions is generally a bad idea.
 * Of course, there are exceptions to this rule, such as numerical types.
*/
class Rational
{
    public:
        Rational(int denominator, int numerator = 1)    // Not explicit, allows implicit int-to-Rational conversion
            : m_denominator { denominator }, m_numerator { numerator } {}

        int denominator() const
        {
            return m_denominator;
        }

        int numerator() const
        {
            return m_numerator;
        }

        // const Rational operator * (const Rational& rhs) const;   // Use the non-member overload below

    private:
        int m_denominator;
        int m_numerator;
};


// This allows you to multiply Rational with ease
Rational oneEight { 1, 8 };
Rational oneHalf { 1, 2 };

Rational result = oneEight * oneHalf;


// But this gives an error - there is no non-member operator* taking an int and a Rational
Rational result2 = 2 * oneEight;


/**
 * The best approach is to make operator* a non-member function,
 * thus allowing compilers to perform implicit type conversions on all arguments.
*/
const Rational operator * (const Rational& lhs, const Rational& rhs)
{
    return Rational { lhs.numerator() * rhs.numerator(), lhs.denominator() * rhs.denominator() };
}

Rational oneFourth { 1, 4 };
Rational result = 2 * oneFourth;
Rational result2 = oneFourth * oneFourth;
