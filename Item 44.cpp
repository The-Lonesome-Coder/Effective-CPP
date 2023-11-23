#include <cstddef>
#include <boost/scoped_array.hpp>
/**
 * Factor parameter-independent code out of templates.
*/

/**
 * When you’re writing a function and you realize that some part of the function’s implementation is
 * essentially the same as another function’s implementation, you factor the common code out of the two functions,
 * put it into a third function, and have both of the other functions call the new one.
 *
 * In template code, replication is implicit: there’s only one copy of the template source code,
 * so you have to train yourself to sense the replication that may take place when a template is
 * instantiated multiple times.
*/
template <typename T, std::size_t Size>
class SquareMatrix
{
    public:
        void invert();
};

// If you call invert() on two different objects, there will be two copies of invert() be instantiated
SquareMatrix<double, 5> m1;
m1.invert();    // call SquareMatrix<double, 5>::invert()

SquareMatrix<double, 10> m2;
m2.invert();    // call SquareMatrix<double, 10>::invert()


/**
 * Your can create a version of the function that took a value as a parameter,
 * then call the parameterized function with 5 or 10 instead of replicating the code.
 *
 * The parameterized version of invert is in a base class, in which it’s templatized only on the type of
 * objects in the matrix, not on the size of the matrix. Hence, all matrices holding a given type of
 * object will share a single SquareMatrixBase class. They will thus share a single copy of that class’s
 * version of invert.
*/
template <typename T>
class SquareMatrixBase
{
    protected:
        void invert(std::size_t size);
};

template <typename T, std::size_t Size>
class SquareMatrix : private SquareMatrixBase<T>
{
    public:
        void invert()
        {
            invert(Size);
        }

    private:
        using SquareMatrixBase<T>::invert();
};


/**
 * How does SquareMatrixBase::invert know what data to operate on? It knows the size of the matrix from its parameter,
 * but how does it know where the data for a particular matrix is?
 *
 * The solution is to have SquareMatrixBase store a pointer to the memory for the matrix values.
 * And as long as it’s storing that, it might as well store the matrix size, too.
*/
template <typename T>
class SquareMatrixBase
{
    protected:
        SquareMatrixBase(std::size_t size, T* p_member)
            : m_size { size }, m_p_data { p_member } {}

        void setDataPointer(T* pointer)
        {
            m_p_data = pointer;
        }

        void invert(std::size_t size);


    private:
        std::size_t m_size;     // Size of matrix
        T* m_p_data;            // Pointer to matrix values
};


template <typename T, std::size_t Size>
class SquareMatrix : private SquareMatrixBase<T>
{
    public:
        SquareMatrix()
            : SquareMatrixBase<T>(Size, data) {}    // Send matrix size and data pointer to base class

        void invert()
        {
            invert(Size);
        }

    private:
        using SquareMatrixBase<T>::invert();

        T data[n * n];
};


/**
 * An alternative would be to put the data for each matrix on the heap.
*/
template <typename T, std::size_t Size>
class SquareMatrix : private SquareMatrixBase<T>
{
    public:
        SquareMatrix()
            : SquareMatrixBase<T>(Size, data), m_p_data { new T[n * n] }
        {
            this->setDataPointer(m_p_data.get());
        }

        void invert()
        {
            invert(Size);
        }

    private:
        using SquareMatrixBase<T>::invert();

        boost::scoped_array<T> m_p_data;
};


/**
 * Templates generate multiple classes and multiple functions, so any template code not dependent on a
 * template parameter causes bloat.
 *
 * Bloat due to non-type template parameters can often be eliminated by replacing template parameters
 * with function parameters or class data members.
 *
 * Bloat due to type parameters can be reduced by sharing implementations for instantiation types with
 * identical binary representations.
*/