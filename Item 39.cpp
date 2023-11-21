/**
 * Use private inheritance judiciously.
*/

/**
 * In private inheritance, compilers will generally not convert a derived class object into a base class object
 * if the inheritance relationship between the classes is private.
 *
 * Members inherited from a private base class become private members of the derived class,
 * even if they were protected or public in the base class.
 *
 * Private inheritance means is-implemented-in-terms-of. As such, private inheritance is purely an implementation technique.
 * (That’s why everything you inherit from a private base class becomes private in your class:
 *  it’s all just implementation detail.)
 *
 * Use composition whenever you can, and use private inheritance whenever you must,
 * primarily when protected members and/or virtual functions enter the picture.
 *
 * Private inheritance is most likely to be a legitimate design strategy when you’re dealing with
 * two classes not related by is-a where one either needs access to the protected members of another or needs to
 * redefine one or more of its virtual functions.
 *
 * Unlike composition, private inheritance can enable the empty base optimization.
 * This can be important for library developers who strive to minimize object sizes.
*/
// Suppose we decide to modify the Example class to keep track of how many times each member function is called
// To make this work, we’ll need to set up an object can be configured to tick with whatever frequency we need
class Timer
{
    public:
        explicit Timer(int tickFrequency);

        virtual void onTick() const;    // Automatically called for each tick
};


// In order for Example to redefine a virtual function in Timer, Example must inherit from Timer
// Public inheritance is inappropriate in this case, it's untrue that Example is-a Timer
// Example's clients shouldn’t be able to call onTick on an Example, since it's not part of the interface, conceptually
class Example : private Timer
{
    private:
        virtual void onTick() const;
};


// Private inheritance isn’t strictly necessary actually
// If we were determined to use composition instead, we could
// This design is more complicated than the one using only private inheritance,
// because it involves both (public) inheritance and composition, as well as the introduction of a new class
// (doesn't mean it's bad)
class Example2
{
    private:
        class Example2Timer : public Timer
        {
            virtual void onTick() const;
        };

        Example2Timer timer;
};
