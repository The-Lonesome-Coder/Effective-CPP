#include <memory>
/**
 * Store "new" objects in smart pointers in standalone statements.
*/

/**
 * This item is partially outdated. By using the "std::shared_ptr" in <memory> library,
 * it's easier than that of discussed in the Item.
*/
class Example { };

std::shared_ptr<Example> example;