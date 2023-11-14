/**
 * Handle assignment to self in operator =.
*/

// An assignment to self occurs when an object is assigned to itself, this sounds silly, but it’s legal.
class Example { /*...*/ };

Example ex;
ex = ex;

/**
 * An implementation of operator= that looks reasonable on the surface but is unsafe in the presence of assignment to self.
*/
class Assist { };

class Example2
{
    public:
        Example2& operator = (const Example2& rhs)
        {
            delete as;
            as = new Assist(*rhs.as);

            return *this;
        }


    private:
        Assist* as;
};


/**
 * The traditional way to prevent this error is to implement an identity test at the top of operator=.
 *
 * But the previous version of operator= wasn’t just self-assignment-unsafe, it was also exception-unsafe.
 * If the “new Bitmap” expression yields an exception, the Example3 will end up holding a pointer to a deleted Assist.
*/
class Example3
{
    public:
        Example3& operator = (const Example3& rhs)
        {
            // Identity test
            if (this == &rhs)
            {
                return *this;
            }

            delete as;
            as = new Assist(*rhs.as);

            return *this;
        }


    private:
        Assist* as;
};


/**
 * Making operator= exception-safe typically renders it self-assignment-safe too.
 *
 * If “new Assist” throws an exception, pb (and the Widget it’s inside of) remains unchanged.
*/
class Example4
{
    public:
        Example4& operator = (const Example4& rhs)
        {
            Assist* pAs = as;           // Remember the original "as"
            as = new Assist(*rhs.as);   // Point "as" to a copy of rhs's "as"
            delete pAs;                 // delete the original "as"

            return *this;
        }

    private:
        Assist* as;
};


/**
 * The (used to be) most recommended technique - Copy and Swap idiom. (Now would be Move and Swap).
*/
class Example5
{
    public:
        void swap(Example5& rhs);

        Example5& operator = (const Example5& rhs)
        {
            Example5 temp { rhs };
            swap(temp);

            return *this;
        }

};


/**
 * Make sure operator= is well-behaved when an object is assigned to itself.
 * Techniques include comparing addresses of source and target objects, careful statement ordering, and copy-and-swap.
 *
 * Make sure that any function operating on more than one object behaves correctly
 * if two or more of the objects are the same.
*/