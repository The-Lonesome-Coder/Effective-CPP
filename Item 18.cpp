/**
 * Make interfaces easy to use correctly and hard to use incorrectly.
*/
// Suppose you're designing a class representing dates in time
class Date
{
    public:
        Date(int month, int day, int year)
            : m_month { month }, m_day { day }, m_year { year } {}

    private:
        int m_month;
        int m_day;
        int m_year;
};


// There are some errors that clients might easily make
Date d1 { 30, 3, 1995 };   // Should be "3, 30", not "30, 3"
Date d2 { 3, 40, 1995 };   // Same as above


// To prevent such incident, you can introduce new types
class Day
{
    public:
        explicit Day(int day)
        : m_day { day } {}

    private:
        int m_day;
};

class Month
{
    public:
        explicit Month(int month)
        : m_month { month } {}

    private:
        int m_month ;
};

class Year
{
    public:
        explicit Year(int year)
        : m_year { year } {}

    private:
        int m_year;
};


/**
 * Then you can use these types in Date constructor.
 *
 * In this case, you can prevent clients make errors.
*/
class Date2
{
    public:
        Date2(const Month& month, const Day& day, const Year& year)
            : m_month { month }, m_day { day }, m_year { year } {}

    private:
        Month m_month;
        Day m_day;
        Year m_year;
};


/**
 * Once the right types are in place, it can sometimes be reasonable to restrict the values of those types.
 *  For example, there are only 12 valid month values, so the Month type should reflect that.
*/
class Month
{
    public:
        static Month Jan() { return Month(1); }   // Functions return all valid Month values
        static Month Feb() { return Month(2); }
        static Month Mar() { return Month(3); }
        /*...*/
        static Month Dec() { return Month(12); }

    private:
        explicit Month(int month)   // Make it private to prevent creation of new Month values
            : m_month { month } {}

        int m_month ;
};

// The client then can use the class like this
Date2 date { Month::Mar(), Day { 30 }, Year { 1995 } };


/**
 * Good interfaces are easy to use correctly and hard to use incorrectly.
 * You should strive for these characteristics in all your interfaces.
 *
 * Ways to facilitate correct use include consistency in interfaces and behavioral compatibility with built-in types.
 *
 * Ways to prevent errors include creating new types, restricting operations on types, constraining object values,
 * and eliminating client resource management responsibilities.
*/