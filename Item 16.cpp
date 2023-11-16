#include <string>
/**
 * Use the same form in corresponding uses of new and delete.
*/
// Suppose you do this
std::string* strArray = new std::string[100];

delete strArray;   // This will only delete one object

/**
 * The result is undefined. 99 of the 100 string objects pointed to by strArray are unlikely to be properly destroyed,
 * because their destructors will probably never be called.
*/

/**
 * When you employ a new expression, you must use the corresponding delete expression.
*/
std::string* strArray2 = new std::string;
delete strArray2;

std::string* strArray2 = new std::string[100];
delete[] strArray2;   // This will delete the whole array


/**
 * If you use [] in a new expression, you must use [] in the corresponding delete expression.
 * If you don’t use [] in a new expression, you mustn’t use [] in the corresponding delete expression.
*/

/**
 * It is discouraged to use new/delete operator now. Instead, use smart pointer or std::array.
*/