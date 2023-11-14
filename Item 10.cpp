/**
 * Have assignment operators return a reference to *this.
*/

/**
 * This convention applies to all assignment operators.
 *
 * Also applies even if the operator’s parameter type is unconventional.
*/
class Example
{
    public:
        Example& operator = (const Example& rhs)
        {
            /*...*/
            return *this;
        }

        Example& operator += (const Example& rhs)
        {
            /*...*/
            return *this;
        }

        Example& operator = (int& rhs)
        {
            /*...*/
            return *this;
        }
};