#include <list>
/**
 * Model "has-a" or "is-implemented-in-terms-of" through composition.
*/

/**
 * Composition is the relationship between types that arises when objects of one type contain objects of another type.
 *
 * Composition means either “has-a” or “is-implemented-in-terms-of.”
 * That’s because you are dealing with two different domains in your software.
 *
 * That’s because you are dealing with two different domains in your software.
 * Some objects in your programs correspond to things in the world you are modeling,
 * e.g., people, vehicles, video frames, etc.
 * Such objects are part of the application domain. Other objects are purely implementation artifacts,
 * e.g., buffers, mutexes, search trees, etc. These kinds of objects correspond to your software’s implementation domain.
 *
 * When composition occurs between objects in the application domain, it expresses a has-a relationship.
 * When it occurs in the implementation domain, it expresses an is-implemented-in-terms-of relationship.
*/
// The wrong way to use list for set
template <typename T>
class Set : public std::list<T> { /* ... */ };


// The right way to use list for set
template <class T>
class Set
{
    public:
        bool member(const T& item) const;

        void insert(const T& item);
        void remove(const T& item);

        std::size_t size() const;


    private:
        std::list<T> data;
};