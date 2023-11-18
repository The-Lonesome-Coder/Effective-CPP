/**
 * Avoid hiding inherited names.
*/

/**
 * The following code follows scope-based hiding rule, all functions named mf1 and mf3 in the
 * base class are hidden by the functions named mf1 and mf3 in the derived class.
 * This applies even though the functions in the base and derived classes take different parameter types,
 * and it also applies regardless of whether the functions are virtual or non-virtual.
*/
class Base
{
    public:
        virtual void mf1() = 0;
        virtual void mf1(int x);

        virtual void mf2();

        void mf3();
        void mf3(double x);

    private:
        int x;
};


class Derived : public Base
{
    public:
        virtual void mf1();

        void mf3();

        void mf4();
};


/**
 * The rationale behind this behavior is that it prevents you from accidentally inheriting overloads
 * from distant base classes when you create a new derived class in a library or application framework.
 *
 * You typically want to inherit the overloads, therefore, you’ll almost always want to override C++’s
 * default hiding of inherited names.
*/
class Base2
{
    public:
        virtual void mf1() = 0;
        virtual void mf1(int x);

        virtual void mf2();

        void mf3();
        void mf3(double x);

    private:
        int x;
};


class Derived2 : public Base2
{
    public:
        // Make all things in Base named mf1 and mf3 visible in Derived's scope
        using Base2::mf1;
        using Base2::mf3;

        virtual void mf1();

        void mf3();

        void mf4();
};


/**
 * It’s conceivable that you sometimes won’t want to inherit all the functions from your base classes.
 * Under public inheritance, this should never be the case, because, again,
 * it violates public inheritance’s is-a relationship between base and derived classes.
 *
 * Under private inheritance, however, it can make sense.
*/
class Base3
{
    public:
        virtual void mf1() = 0;
        virtual void mf1(int x);
};


class Derived3 : private Base3
{
    // Forwarding function
    virtual void mf1()
    {
        Base3::mf1();   // In this case, clients cannot call Base::mf1() directly via Derived class object
    }
};


/**
 * Names in derived classes hide names in base classes. Under public inheritance, this is never desirable.
 *
 * To make hidden names visible again, employ using declarations or forwarding functions.
*/