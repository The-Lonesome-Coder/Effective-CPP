/**
 * Prefer non-member non-friend functions to member functions.
*/

/**
 *  Object-oriented principles dictate that data should be as encapsulated as possible.
 *
 * If something is encapsulated, it’s hidden from view. The more something is encapsulated, the fewer things can see it.
 *
 * Value encapsulation in the first place: it affords us the flexibility to change things in a way that
 * affects only a limited number of clients.
 *
 * The less code that can see the data (i.e., access it), the more the data is encapsulated,
 * and the more freely we can change characteristics of an object’s data, such as the number
 * of data members, their types, etc.
*/
class WebBrowser
{
    public:
        void clearCache();
        void clearHistory();
        void removeCookies();

        void clearEverything();   // Calls the above three functions, not encapsulated enough
};

/**
 * Non-member non-friend function, more encapsulated because it does not increase the number of functions
 * that can access the private parts of the class.
*/
void clearBrowser(WebBrowser& wb)
{
    wb.clearCache();
    wb.clearHistory();
    wb.removeCookies();
}


/**
 * This reasoning applies only to non-member non-friend functions.
 * Friends have the same access to a class’s private members that member functions have,
 * hence the same impact on encapsulation. From an encapsulation point of view,
 * the choice isn’t between member and non-member functions,
 * it’s between member functions and non-member non-friend functions.
 *
 * Just because concerns about encapsulation dictate that a function be a non-member of one class doesn’t
 * mean it can’t be a member of another class.
*/

/**
 * In C++, a more natural approach would be to make clearBrowser a nonmember function in the same namespace as WebBrowser.
*/
namespace WebBrowserStuff
{
    class WebBrowser2
    {
        public:
            void clearCache();
            void clearHistory();
            void removeCookies();
    };


    void clearBrowser(WebBrowser& wb)
    {
        wb.clearCache();
        wb.clearHistory();
        wb.removeCookies();
    }
}


/**
 * A much better way is to separate a huge library into individual header files, but in one namespace.
*/
// Header "web_browser.h" - header for class WebBrowser itself as well as “core” WebBrowser-related functionality
namespace WebBrowserStuff
{
    class WebBrowser3 { };
}

// Header “web_browser_bookmarks.h” - bookmark-related convenience functions
namespace WebBrowserStuff
{
    /*...*/
}

// header “web_browser_cookies.h” - cookie-related convenience functions
namespace WebBrowserStuff
{
    /*...*/
}