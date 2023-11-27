/**
 * Understand when it makes sense to replace new and delete.
*/

/**
 * Reasons to replace the compiler-provided versions of operator new or operator delete:
 *
 * 1. To detect usage errors. Failure to delete memory conjured up by new leads to memory leaks.
 *    Using more than one delete on newed memory yields undefined behavior. If operator new keeps a list of
 *    allocated addresses and operator delete removes addresses from the list, it’s easy to detect such usage errors.
 *    Similarly, a variety of programming mistakes can lead to data overruns and underruns.
 *
 * 2. To improve efficiency. The versions of operator new and operator delete that ship with compilers are designed
 *    for general-purpose use. For instance, acceptable for long-running programs and that of execute for less than
 *    a second, worry about heap fragmentation, etc.
 *
 * 3. To collect usage statistics.  Custom versions of operator new and operator delete make it easy to collect
 *    information about how your software uses its dynamic memory.
*/

/**
 * Alignment issue.
 *
 * Many computer architectures require that data of particular types be placed in memory at particular kinds of addresses.
 * For example, an architecture might require pointers occur at addresses that are a multiple of four (four byte aligned).
 *
 * Alignment is relevant here, because C++ requires that all operator news return pointers that are suitably aligned for
 * any data type. malloc() labors under the same requirement, so having operator new return a pointer it gets from
 * malloc() is safe.
 *
 * Details like alignment are the kinds of things that distinguish professional-quality memory managers from ones thrown
 * together by programmers distracted by the need to get on to other tasks. Writing a custom memory manager that almost
 * works is pretty easy. Writing one that works well is a lot harder. As a general rule, I suggest you not attempt it
 * unless you have to.
 *
 * In many cases, you don’t have to. Some compilers have switches that enable debugging and logging  * functionality
 * in their memory management functions. Another option is open source memory managers. They’re available for many platforms,
 * so you can download and try those. One such open source allocator is the Pool library from Boost.
*/

/**
 * Reasons to replace the compiler-provided versions of operator new or operator delete in more detail:
 *
 * 1. To detect usage errors (as above).
 *
 * 2. To collect statistics about the use of dynamically allocated memory (also as above).
 *
 * 3. To increase the speed of allocation and deallocation. Class-specific allocators are an example application of
 *    fixed-size allocators such as those offered by Boost’s Pool library.
 *
 * 4. To reduce the space overhead of default memory management. Allocators tuned for small objects (such as those
 *    in Boost’s Pool library) essentially eliminate such overhead.
 *
 * 5. To compensate for suboptimal alignment in the default allocator.
 *
 * 6. To cluster related objects near one another. Sometimes you want operators new and delete to do something that
 * the compiler-provided versions don’t offer.
*/