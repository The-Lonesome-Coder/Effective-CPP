/**
 * Define non-member functions inside templates when type conversions are desired.
 *
 * When writing a class template that offers functions related to the template that support implicit type
 * conversions on all parameters, define those functions as friends inside the class template.
*/

/**
 * Let's extend the Rational class in Item 24, and make it a template.
*/
template <typename T>
class Rational
{
    public:
        Rational(const T& numerator = 0, const T& denominator = 1)
            : m_numerator { numerator }, m_denominator { denominator } {}

        const T numerator() const;
        const T denominator() const;

    private:
        T m_numerator;
        T m_denominator;
};


template <typename T>
const Rational<T> operator * (const Rational<T>& lhs, const Rational<T>& rhs)
{
    /* ... */
}


Rational<int> oneHalf { 1, 2 };
Rational<int> result = oneHalf * 2;     // Won't compile


/**
 * In above code, compilers do not know which function we want to call.
 * Instead, they’re trying to figure out what function to instantiate (i.e., create) from the template named operator *.
 * The problem is they can't figure out what T is.  In this case, those types are Rational<int>
 * (the type of oneHalf) and int (the type of 2). Each parameter is considered separately.
 *
 * Implicit type conversion via constructor calls is not considered during template argument deduction,
 * such conversions are used during function calls. Therefore, the compiler cannot implicitly convert int
 * (in this case: 2) to Rational<int>.
 *
 * We can relieve compilers of the challenge of template argument deduction by taking advantage of the fact that a friend
 * declaration in a template class can refer to a specific function. That means the class Rational<T> can declare operator *
 * for Rational<T> as a friend function. Class templates don’t depend on template argument deduction
 * (that process applies only to function templates), so T is always known at the time the class Rational<T> is instantiated.
*/
template <typename T>
class Rational2
{
    public:
        Rational2(const T& numerator = 0, const T& denominator = 1)
            : m_numerator { numerator }, m_denominator { denominator } {}

        friend const Rational2<T> operator * (const Rational2<T>& lhs, const Rational2<T>& rhs)
        {
            return Rational2(lhs.numerator() * rhs.numerator(), lhs.denominator() * rhs.denominator());
        }

        const T numerator() const;
        const T denominator() const;

    private:
        T m_numerator;
        T m_denominator;
};


template <typename T>
const Rational2<T> operator * (const Rational2<T>& lhs, const Rational2<T>& rhs)
{
    /* ... */
}

Rational2<int> oneHalf { 1, 2 };
Rational2<int> result = oneHalf * 2;


/**
 * You can minimize the impact of such inline declarations by having operator* do nothing but call
 * a helper function defined outside of the class. In the example in this Item, there’s not much point in doing that,
 * because operator* is already implemented as a one-line function, but for more complex function bodies,
 * it may be desirable.
 *
 * As a template, of course, doMultiply won’t support mixed-mode multiplication, but it doesn’t need to.
 * It will only be called by operator*, and operator * does support mixed-mode operations!
*/
template <typename T>
class Rational3
{
    public:
        Rational3(const T& numerator = 0, const T& denominator = 1)
            : m_numerator { numerator }, m_denominator { denominator } {}

        friend const Rational3<T> operator * (const Rational3<T>& lhs, const Rational3<T>& rhs)
        {
            return multiply(lhs, rhs);
        }


    private:
        T m_numerator;
        T m_denominator;
};


template <typename T>
const Rational3<T> multiply(const Rational3<T>& lhs, const Rational3<T>& rhs)
{
    return Rational3<T>>(lhs.numerator() * rhs.numerator(), lhs.denominator() * rhs.denominator());
}