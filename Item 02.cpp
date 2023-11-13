/**
 * Prefer "const", "enum" and "inline" to "#define" (Prefer the compiler to the preprocessor).
 *
 * NOTE:: With compiler getting stronger and stronger in optimization, the usage of inline has changed.
 *        See: Is there still a use for inline? (https://stackoverflow.com/questions/29796264/is-there-still-a-use-for-inline)
*/

// When you do something like this
#define ASPECT_RATIO 1.653

/**
 * The name "ASPECT_RATIO" will never been seen by compilers.
 * It may be removed by the preprocessor before the source code ever gets to a compiler.
 *
 * This may be confusing if you get an error during compilation involving the use of the constant,
 * because the name ASPECT_RATIO may not get entered into the symbol table,
 * the error message may refer to 1.653, not ASPECT_RATIO, therefore, you will have no idea where the "1.653" came from.
*/

// The solution is to replace the macro with a constant
const double AspectRatio {1.653};

/**
 * As a language constant, the name "AspectRatio" is definitely seen by compilers,
 * and is certainly entered into their symbol tables.
*/


// Class-specific constants
/**
 * To limit the scope of a constant to a class, you must make it a member,
 * and to ensure there’s at most one copy of the constant, you must make it a static member
*/
class Player1
{
    private:
        static const int NumTurns {5};   // Constant declaration
        int scores[NumTurns];            // Use of constant
};


// "the enum hack"
/**
 * Some old compilers do not accept in-class constant initialization,
 * a quick solution is to define it out of scope.
*/
class Player2
{
    private:
        static const int NumTurns;      // Declaration of static class constant; goes in header file
        int scores[NumTurns];
};

const int Player2::NumTurns {5};        // Definition of static class constant; goes into implementation file


/**
 * "the enum hack" takes advantage of the fact that the values of an enumerated type can be used where ints are expected.
 *
 * "the enum hack" behaves in some ways more like a #define than a const does, and sometimes that’s what you want.
 *
 * Lots of code employs it, so you need to recognize it when you see it.
 * In fact, the enum hack is a fundamental technique of template meta-programming.
*/
class Player3
{
    private:
        enum { NumTurns = 5 };          // “the enum hack” — makes NumTurns a symbolic name for 5
        int scores[NumTurns];
};