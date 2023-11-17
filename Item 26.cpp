#include <stdexcept>
#include <string>
#include <string_view>
/**
 * Postpone variable definitions as long as possible.
*/

// Consider the following code
std::string encryptedPassword(std::string_view password)
{
    std::string encrypted { password };

    if (password.length() < 8)
    {
        throw std::logic_error { "Password too short." };
    }

    /* ...Do something... */

    return encrypted;
}


/**
 * The object "encrypted" is not completely unused in this function, but it’s unused if an exception is thrown.
 * That is, you’ll pay for the construction and destruction of encrypted even if encryptPassword throws an exception.
 * As a result, you’re better off postponing encrypted’s definition until you know you’ll need it.
*/
std::string encryptedPassword2(std::string_view password)
{
    if (password.length() < 8)
    {
        throw std::logic_error { "Password too short." };
    }

    std::string encrypted { password };

    /* ...Do something... */

    return encrypted;
}


/**
 * If a variable is used only inside a loop, you can:
 *
 * 1. Define it outside the loop and make an assignment to it on each loop iteration.
 * 2. Define the variable inside the loop.
 *
 * For classes where an assignment costs less than a constructor-destructor pair,
 * approach 1 is generally more efficient. This is especially the case as n gets large.
 * Otherwise, approach 2 is probably better.
*/