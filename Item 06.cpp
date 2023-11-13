/**
 * Explicitly disallow the use of compiler-generated functions you do not want.
*/

/**
 * The trick in Effective C++ no longer applied, due to the usage of "delete" keyword on functions feature added in C++11.
 *
 * To disallow the use of specific functions, use "= delete;".
*/
class HomeForSale
{
    public:
        HomeForSale(const HomeForSale&) = delete;
        HomeForSale& operator=(const HomeForSale&) = delete;
};