/**
 * Never redefine a function's inherited default parameter value.
*/

/**
 * Virtual functions are dynamically bound, but default parameter values are statically bound.
 *
 * In this example, ps, pc, and pr are all declared to be of type pointer-toShape,
 * so they all have that as their static type.
 *
 * An object’s dynamic type is determined by the type of the object to which it currently refers.
 * That is, its dynamic type indicates how it will behave. In the example, pc’s dynamic type is Circle*,
 * and pr’s dynamic type is Rectangle*.
*/
class Shape
{
    public:
        enum Color { Red, Green, Blue };

        virtual void draw(Color color = Red) const = 0;
};


class Rectangle : public Shape
{
    public:
        // Not a good idea to define different parameter in virtual functions
        virtual void draw(Color color = Green) const override;
};


class Circle : public Shape
{
    public:
        virtual void draw(Color color) const override;
};

Shape *p_shape;
Shape *p_circle = new Circle;
Shape *p_rec = new Rectangle;


/**
 * Virtual functions are dynamically bound, meaning that the particular function called is determined by the
 * dynamic type of the object through which it’s invoked.
*/
p_circle->draw(Shape::Red);   // Calls Circle::draw(Shape::Red)
p_rec->draw(Shape::Red);      // Calls Rectangle::draw(Shape::Red)


/**
 * The twist comes in when you consider virtual functions with default parameter values, because,
 * virtual functions are dynamically bound, but default parameters are statically bound.
 *
 * In Rectangle::draw, the default parameter value is Green. Because pr’s static type is Shape*,
 * however, the default parameter value for this function call is taken from the Shape class,
 * not the Rectangle class! The result is a call consisting of a strange and almost certainly
 * unanticipated combination of the declarations for draw in both the Shape and Rectangle classes.
 *
 * C++ insist on acting in this perverse manner has to do with runtime efficiency.
 * If default parameter values were dynamically bound, compilers would have to come up with a way to
 * determine the appropriate default value(s) for parameters of virtual functions at runtime,
 * which would be slower and more complicated than the current mechanism of determining them during compilation.
*/
p_rec->draw();   // Calls Rectangle::draw(Shape::Red)
