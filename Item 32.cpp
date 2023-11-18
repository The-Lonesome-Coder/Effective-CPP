#include <cassert>
/**
 * Make sure public inheritance models "is-a".
*/

/**
 * The single most important rule in object-oriented programming with C++ is this: public inheritance means “is-a.”
 *
 * If you write that class D (“Derived”) publicly inherits from class B (“Base”),
 * you are telling C++ compilers (as well as human readers of your code) that every object of type D is also an object of type B,
 * but not vice versa.
*/
class Rectangle
{
    public:
        virtual void setHeight(int newHeight);
        virtual void setWidth(int newWidth);

        virtual int height() const;
        virtual int width() const;

        /* ... */
};


void makeBigger(Rectangle& rectangle)
{
    int oldHeight = rectangle.height();
    rectangle.setWidth(rectangle.width() + 10);
    assert(rectangle.height() == oldHeight);
}


class Square : public Rectangle
{
    /* ... */
};


/**
 * The fundamental difficulty in this case is that something applicable to a rectangle (its width may be
 * modified independently of its height) is not applicable to a square (its width and height must be the same).
 * But public inheritance asserts that everything that applies to base class objects — everything! — also applies
 * to derived class objects. In the case of rectangles and squares (as well as an example involving sets and lists in
 * Item 38), that assertion fails to hold, so using public inheritance to model their relationship is simply incorrect.
*/