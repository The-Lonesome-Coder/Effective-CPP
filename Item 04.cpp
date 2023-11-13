#include <cstddef>
#include <string_view>
/**
 * Make sure that objects are initialized before they're used
*/

/**
 * Reading uninitialized values yields undefined behavior,
 * and will eventually lead to inscrutable program behavior and a lot of unpleasant debugging.
 *
 * For nonmember objects of built-in types, you’ll need to initialize them manually,
 * for almost everything else, the responsibility for initialization falls on constructors.
*/
int x { 0 };   // Manual initialization of an int


class AddressBookEntry
{
    public:
        AddressBookEntry(std::string_view name, std::string_view address);

    private:
        std::string m_name;
        std::string m_address;
};

// These are all assignments, not initialization
AddressBookEntry::AddressBookEntry(std::string_view name, std::string_view address)
{
    m_name = name;
    m_address = address;
}


/**
 * A better way to write constructor is to use the member initialization list instead of assignment.
 *
 * To avoid having to memorize when data members must be initialized in the member initialization list
 * and when it’s optional, the easiest choice is to always use the initialization list.
 *
 * To avoid reader confusion, as well as the possibility of some truly obscure behavioral bugs,
 * always list members in the initialization list in the same order as they’re declared in the class.
*/
class AddressBookEntry2
{
    public:
        AddressBookEntry2(std::string_view name, std::string_view address);

    private:
        std::string m_name;
        std::string m_address;
};

// These are initialization
AddressBookEntry2::AddressBookEntry2(std::string_view name, std::string_view address)
    : m_name { name }, m_address { address } {}


/**
 * The order of initialization of non-local static objects defined in different translation units is important.
 *
 * The actual problem is this: if initialization of a non-local static object in one translation unit uses a
 * non-local static object in a different translation unit, the object it uses could be uninitialized, because
 * the relative order of initialization of non-local static objects defined in different translation units is undefined.
*/
class FileSystem   // Create by the library creator
{
    public:
        // ...
        std::size_t numDisks() const;   // One of many member functions
};

extern FileSystem theFileSystem;    // Declare object for clients to use


class Directory   // Create by library client
{
    public:
        Directory(std::size_t disk);

    private:
        std::size_t m_disks;
};

Directory::Directory(std::size_t disk)
{
    std::size_t disks = theFileSystem.numDisks();   // Use the "theFileSystem" object
}

// Suppose the client decides to create a single "Directory" object for temporary file
Directory tempDir(...);

/**
 * Unless theFileSystem is initialized before tempDir,
 * tempDir’s constructor will attempt to use tfs before it’s been initialized.
 *
 * Determining the “proper” order in which to initialize non-local static objects is hard. Very hard.
 * Fortunately, a small design change eliminates the problem entirely.
 * All that has to be done is to move each non-local static object into its own function, where it’s declared static.
 * These functions return references to the objects they contain.
 * In other words, non-local static objects are replaced with local static objects ("part of" Singleton pattern).
*/
class FileSystem2
{
    public:
        // ...
        std::size_t numDisks() const;
};

// Replace "theFileSystem"
FileSystem2& fileSystem()
{
    static FileSystem2 fileSystem;   // Define and initialize a local static object

    return fileSystem;               // Return a reference to it
}


class Directory2
{
    public:
        Directory2(std::size_t disk);

    private:
        std::size_t m_disks;
};

Directory2::Directory2(std::size_t disk)
{
    std::size_t disks = fileSystem().numDisks();
}

Directory2& temp()
{
    static Directory2 tDir { };   // Define and initialize local static object

    return tDir;                  // Return a reference to it
}

/**
 * The reference-returning functions dictated by this scheme are always simple:
 * define and initialize a local static object on line 1, return it on line 2.
*/