#include <cstddef>   // For std::size_t
#include <cstring>
#include <utility>
/**
 * Use "const" whenever possible.
*/

// The word "const" may appear before or after the type, they mean the same thing.
void f1 (const int number);
void f2 (int const number);


/**
 * Having a function return a constant value is generally inappropriate,
 * but sometimes doing so can reduce the incidence of client errors without giving up safety or efficiency.
 * For instance, declaring operator *’s return value const prevents the user from making a annoying mistake.
*/
class Rational
{
    public:
        const Rational operator * (const Rational& rhs);   // Return const value
};

if (a * b = c) ...   // An annoying mistake of missing one equal sign


/**
 * Bitwise constness
 *
 * Bitwise const believers believe that a member function is const if and only if it does not modify any of the object's
 * data member, i.e., if it doesn’t modify any of the bits inside the object. It's easy for the compilers to detect
 * violations.
 *
 * Many member functions that don’t act very const pass the bitwise test,
 * especially a member function that modifies what a pointer points to frequently doesn’t act const.
*/
class TextBlock1
{
    public:
        char& operator [] (std::size_t position) const   // Inappropriate (but bitwise const) declaration of [] operator
        { return mpText[position]; }

    private:
        char* mpText;
};

// The implementation doesn’t modify pText in any way, therefore, the following code will work
const TextBlock1 text { "Hello" };
char *pc = &text[0];
*pc = 'J';   // text now has value "Jello"


/**
 * Logical constness
 *
 * Adherents to this philosophy — and you should be among them — argue that a const member
 * function might modify some of the bits in the object on which it’s invoked,
 * but only in ways that clients cannot detect.
*/

class TextBlock3
{
    public:
        std::size_t length() const;

    private:
        char* mpText;
        std::size_t textLength;   // Last calculated length
        bool lengthIsValid;       // Whether length is currently valid
};

std::size_t TextBlock2::length() const
{
    if (!lengthIsValid)
    {
        textLength = std::strlen(mpText);
        lengthIsValid = true;               // Error, cannot assign to textLength and lengthIsValid in a const function
    }

    return textLength;
}


/**
 * Compiler disagree and insist on bitwise constness.
 * However, we can take advantage of C++’s keyword "mutable".
 * "mutable" frees non-static data members from the constraints of bitwise constness.
*/

class TextBlock3
{
    public:
        std::size_t length() const;

    private:
        char* mpText;

        // With "mutable", these data members may always be modified, even in const member function
        mutable std::size_t textLength;
        mutable bool lengthIsValid;
};

std::size_t TextBlock3::length() const
{
    if (!lengthIsValid)
    {
        textLength = std::strlen(mpText);
        lengthIsValid = true;               // No errors
    }

    return textLength;
}


/**
 * Avoiding Duplication in const and Non-const Member Functions - const-and-back-again idiom
 *
 * When the const and non-const version of the function does exactly the same, you can first add const to it.
 * By doing so, you can have the non- version of the function call the const one, then remove the const in the end.
*/
class TextBlock4
{
    const char& operator [] (std::size_t position) const
    {
        // ...
        // ...
        // ...
        return text[position];
    }

    char& operator [] (std::size_t position)
    {
        // By using std::as_const in the <utility> library, you can:
        // 1. Add const to the non-const object "*this"
        // 2. Call the const version of []
        // 3. Use const_cast to remove the constness on the object
        return const_cast<char&>(std::as_const(*this)[position]);
    }
};