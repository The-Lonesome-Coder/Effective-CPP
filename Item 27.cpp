/**
 * Minimize casting.
*/

/**
 * C++ also offers four new cast forms:
 *
 * 1. const_cast<T>(expression)
 * 2. dynamic_cast<T>(expression)
 * 3. reinterpret_cast<T>(expression)
 * 4. static_cast<T>(expression)
*/

/**
 * An interesting thing about casts is that it’s easy to write something that looks right, but is wrong.
 * Many application frameworks, for example, require that virtual member function implementations in derived
 * classes call their base class counterparts first.
 *
 * The following example will not work. It does not invoke that function on the current object!
 * Instead, the cast creates a new, temporary copy of the base class part of *this, then invokes resize() on the copy!
*/
class Window
{
    public:
        virtual void resize();
};

class SpecialWindow : public Window
{
    public:
        virtual void resize()
        {
            static_cast<Window>(*this).resize();    // This does not work
        }

};

/**
 * The solution is to eliminate the cast, replacing it with what you really want to say.
 *
 * This example also demonstrates that if you find yourself wanting to cast,
 * it’s a sign that you could be approaching things the wrong way.
*/
class SpecialWindow2 : public Window
{
    public:
        virtual void resize()
        {
            Window::resize();   // Calls Window::resize() on *this
        }
};


/**
 * Avoid casts whenever practical, especially dynamic_casts in performance-sensitive code.
 * If a design requires casting, try to develop a cast-free alternative.
 *
 * When casting is necessary, try to hide it inside a function.
 * Clients can then call the function instead of putting casts in their own code.
 *
 * Prefer C++-style casts to old-style casts. They are easier to see, and they are more specific about what they do.
*/