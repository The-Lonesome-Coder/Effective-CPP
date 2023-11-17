#include <istream>
#include <memory>
/**
 * Strive for exception-safe code.
*/
// Suppose we have a class for representing GUI menus with background images
class Image { };

void lock(Mutex *p_m);
void unlock(Mutex *p_m);

class Mutex { };

class Menu
{
    public:
        void changeBackgroundImage(std::istream& imgSrc);

    private:
        Mutex mutex;
        Image* backgroundImage;                 // Current background image
        static inline int imageChangeCount;     // # times image has been changed
};


/**
 * The code below is not exception-safe. When an exception is thrown, exception-safe functions :
 *
 * 1. Leak no resources.
 *    The code below fails this test, because if the "new Image(imgSrc)" yield exceptions,
 *    the call to unlock will never gets executed, and the mutex is held forever.
 *
 * 2. Don't allow data structures to become corrupted.
 *    If "new Image(imgSrc)" expression, backgroundImage is left pointing to a deleted object.
 *    In addition, imageChangeCount has been incremented, even though it's not true that a new
 *    image has been installed.
*/
void Menu::changeBackgroundImage(std::istream& imgSrc)
{
    lock(&mutex);
    delete backgroundImage;
    ++imageChangeCount;
    backgroundImage = new Image(imgSrc);
    unlock(&mutex);
}


/**
 * Item 14 introduces the Lock class as a way to ensure that mutexes are released in a timely fashion.
*/
class Lock
{
    public:
        explicit Lock(Mutex *p_m)
            : m_mutexPtr { p_m }
        {
            lock(m_mutexPtr);   // Acquire resources
        }

        ~Lock()
        {
            unlock(m_mutexPtr);   // Release resources
        }

    private:
        Mutex* m_mutexPtr;
};


void Menu::changeBackgroundImage(std::istream& imgSrc)
{
    Lock ml(&mutex);    // Acquire mutex and ensure its later release
    delete backgroundImage;
    ++imageChangeCount;
    backgroundImage = new Image(imgSrc);
}


/**
 * Exception-safe functions must offer one of three guarantees. If it doesn’t, it’s not exception-safe:
 *
 * 1. Functions offering the basic guarantee promise that if an exception is thrown,
 *    everything in the program remains in a valid state. No objects or data structures
 *    become corrupted, and all objects are in an internally consistent state.
 *
 * 2. Functions offering the strong guarantee promise that if an exception is thrown,
 *    the state of the program is unchanged. Calls to such functions are atomic in the sense
 *    that if they succeed, they succeed completely, and if they fail,
 *    the program state is as if they’d never been called.
 *
 * 3. Functions offering the nothrow guarantee promise never to throw exceptions,
 *    because they always do what they promise to do. All operations on built-in types
 *    (e.g., ints, pointers, etc.) are nothrow (i.e., offer the nothrow guarantee).
 *    This is a critical building block of exception-safe code. Functions offer this promise
 *    does not mean that it will never throw an exception; it means that if it throws an exception,
 *    it’s a serious error, and the std::terminate() function should be called.
 *    Note: Since C++11, it is accomplished by the "noexcept" operator.
*/
class Menu2
{
    public:
        void changeBackgroundImage(std::istream& imgSrc);

    private:
        Mutex mutex;
        std::shared_ptr<Image> backgroundImage;
        static inline int imageChangeCount;
};

void Menu2::changeBackgroundImage(std::istream& imgSrc)
{
    Lock ml(&mutex);
    // No longer a need to manually delete the old image, handled internally by the smart pointer
    backgroundImage.reset(new Image(imgSrc));
    ++imageChangeCount;
}


/**
 * General design strategy that offers strong guarantee - copy and swap.
 *
 * In principle, it’s very simple. Make a copy of the object you want to modify,
 * then make all needed changes to the copy. If any of the modifying operations throws an exception,
 * the original object remains unchanged. After all the changes have been successfully completed,
 * swap the modified object with the original in a non-throwing operation.
 * This is usually implemented by putting all the per-object data from the “real” object
 * into a separate implementation object, then giving the real object a pointer to its implementation object.
 * This is often known as the “pimpl idiom,” and Item 31 describes it in some detail.
*/
class MenuImplementation
{
    // The implementation of Menu3 ensures this class is private, therefore, public is fine
    public:
        std::shared_ptr<Image> backgroundImage;
        static inline int imageChangeCount;
};

class Menu3
{
    public:
        void changeBackgroundImage(std::istream& imgSrc);

    private:
        Mutex mutex;
        std::shared_ptr<MenuImplementation> p_Impl;
};

void Menu3::changeBackgroundImage(std::istream& imgSrc)
{
    Lock ml(&mutex);
    std::shared_ptr<MenuImplementation> p_New(new MenuImplementation(*p_Impl));
    p_New->backgroundImage.reset(new Image(imgSrc));
    ++p_New->imageChangeCount;
    std::swap(p_Impl, p_New);
}


/**
 * Exception-safe functions leak no resources and allow no data structures to become corrupted,
 * even when exceptions are thrown. Such functions offer the basic, strong, or nothrow guarantees.
 *
 * The strong guarantee can often be implemented via copy-and-swap,
 * but the strong guarantee is not practical for all functions.
 *
 * A function can usually offer a guarantee no stronger than the weakest guarantee of the functions it calls.
*/
