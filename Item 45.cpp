/**
 * Use member function templates to accept "all compatible types".
*/

/**
 * Use member function templates to generate functions that accept all compatible types.
*/
// Suppose we have a container class
template <typename T>
class Container
{
    public:
        template <typename U>
        void add(const U& element);
};

// To define the function template out of class
template <typename T>
template <typename U>
void Container<T>::add(const U& element)
{
    /* Do something */
}


/**
 * If you declare member templates for generalized copy construction or generalized assignment,
 * you’ll still need to declare the normal copy constructor and copy assignment operator, too.
*/
class Top { };
class Middle: public Top { };
class Bottom: public Middle { };

Top *pt1 = new Middle; // convert Middle* ⇒ Top*
Top *pt2 = new Bottom; // convert Bottom* ⇒ Top*
const Top *pct2 = pt1; // convert Top* ⇒ const Top*

/**
 * There is no way to write out all the constructors we need.
 * In principle, the number of constructors we need is unlimited.
 * Since a template can be instantiated to generate an unlimited number of functions,
 * it seems that we don’t need a constructor function for SmartPtr, we need a constructor template.
*/
template <typename T>
class SmartPointer
{
    public:
        SmartPointer(const SmartPointer& other);        // Copy constructor

        template <typename U>
        SmartPointer(const SmartPointer<U>& other);     // Generalized copy constructor

        SmartPointer& operator = (const SmartPointer& other);   // Copy assignment

        template <typename U>
        SmartPointer& operator = (const SmartPointer<U>& other);    // Generalized copy assignment
};