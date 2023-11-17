#include <memory>
/**
 * Avoid returning "handles" to object internals.
*/

// Suppose a Rectangle class
class Point
{
    public:
        Point(int x, int y)
            : m_x { x }, m_y { y } {}

        void setX(int newX) { m_x = newX; }
        void setY(int newY) { m_y = newY; }

    private:
        int m_x;
        int m_y;
};


class RectangleData
{
    public:
        Point upperLeftCorner;
        Point lowerRightCorner;
};


/**
 * The following code will compile, but it's wrong. By designing the getter functions to return references to internal data,
 * the clients are allowed to modify the internal data, even if the function is declared const. It is dangerous.
*/
class Rectangle
{
    public:
        Point& upperLeft() const { return m_p_Data->upperLeftCorner; }
        Point& lowerRight() const { return m_p_Data->lowerRightCorner; }

    private:
        std::shared_ptr<RectangleData> m_p_Data;
};


/**
 * The solution is easy, return a new temp object.
*/
class Rectangle2
{
    public:
        Point upperLeft() const { return m_p_Data->upperLeftCorner; }
        Point lowerRight() const { return m_p_Data->lowerRightCorner; }

    private:
        std::shared_ptr<RectangleData> m_p_Data;
};


/**
 * Avoid returning handles (references, pointers, or iterators) to object internals.
 * Not returning handles increases encapsulation, helps const member functions act const,
 * and minimizes the creation of dangling handles.
 *
 * However, sometimes you have to. For example, operator [] allows you to pluck individual
 * elements out of strings and vectors, and these operators [] work by returning references
 * to the data in the containers.
*/