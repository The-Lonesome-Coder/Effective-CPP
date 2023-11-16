/**
 * Treat class design as type design.
*/

/**
 * To design effective class, you should confront these questions"
 *
 * How should object initialization differ from object assignment?
 * The answer to this question determines the behavior of and the differences between your constructors
 * and your assignment operators. It’s important not to confuse initialization with assignment,
 * because they correspond to different function calls.
 *
 *
 * What does it mean for objects of your new type to be passed by value?
 * Remember, the copy constructor defines how pass-by value is implemented for a type.
 *
 *
 * What are the restrictions on legal values for your new type?
 * Usually, only some combinations of values for a class’s data members are valid.
 * Those combinations determine the invariants your class will have to maintain.
 * The invariants determine the error checking you’ll have to do inside your member functions,
 * especially your constructors, assignment operators, and “setter” functions.
 * It may also affect the exceptions your functions throw and, on the off chance you use them,
 * your functions’ exception specifications.
 *
 *
 * Does your new type fit into an inheritance graph?
 * If you inherit from existing classes, you are constrained by the design of those classes,
 * particularly by whether their functions are virtual or non-virtual.
 * If you wish to allow other classes to inherit from your class, that affects whether the
 * functions you declare are virtual, especially your destructor.
 *
 *
 * What kind of type conversions are allowed for your new type?
 * Your type exists in a sea of other types, so should there be conversions between your type and other types?
 * If you wish to allow objects of type T1 to be implicitly converted into objects of type T2,
 * you will want to write either a type conversion function in class T1 (e.g., operator T2) or a
 * non-explicit constructor in class T2 that can be called with a single argument.
 * If you wish to allow explicit conversions only, you’ll want to write functions to perform the conversions,
 * but you’ll need to avoid making them type conversion operators or non-explicit constructors
 * that can be called with one argument.
 *
 *
 * What operators and functions make sense for the new type?
 * The answer to this question determines which functions you’ll declare for your class.
 * Some functions will be member functions, but some will not.
 *
 *
 * What standard functions should be disallowed?
 * Those are the ones you’ll need to declare private.
 *
 *
 * Who should have access to the members of your new type?
 * This question helps you determine which members are public, which are protected, and which are private.
 * It also helps you determine which classes and/or functions should be friends,
 * as well as whether it makes sense to nest one class inside another.
 *
 *
 * What is the “undeclared interface” of your new type?
 * What kind of guarantees does it offer with respect to performance, exception safety,
 * and resource usage (e.g., locks and dynamic memory)?
 * The guarantees you offer in these areas will impose constraints on your class implementation.
 *
 *
 * How general is your new type?
 * Perhaps you’re not really defining a new type. Perhaps you’re defining a whole family of types.
 * If so, you don’t want to define a new class, you want to define a new 0class template.
 *
 *
 * Is a new type really what you need?
 * If you’re defining a new derived class only so you can add functionality to an existing class,
 * perhaps you’d better achieve your goals by simply defining one or more non-member functions or templates.
*/