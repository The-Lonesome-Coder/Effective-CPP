#include <string>
/**
 * Use multiple inheritance judiciously.
*/

/**
 * Ambiguity issue: it becomes possible to inherit the same name.
*/
class Borrowable
{
    public:
        void checkOut();
};


class Electronic
{
    private:
        void checkOut();
};


class MP3 : public Borrowable, public Electronic { };

MP3 mp;
mp.checkOut();   // Ambiguous! Which checkOut?


/**
 * To resolve this issue, you have to specify which function you are calling.
*/
mp.Borrowable::checkOut();


/**
 * Deadly MI diamond: you have an inheritance hierarchy with more than one path between a base class and a derived class.
*/

class File { };

class InputFile : public File { };
class OutputFile : public File { };

class IOFile : public InputFile, public OutputFile { };


/**
 * Two options:
 *
 * 1. Suppose that the File class has a data member, it inherits a copy from each of its base classes,
 *    so that suggests that IOFile should have two File's data members.
 *
 * 2. Simple logic says that an IOFile object has only one file name, so the File's data member IOFile inherits through
 *    its two base classes should not be replicated.
 *
 *
 * C++ happily supports both options, though its default is to perform the replication.
 * If that’s not what you want, you must make the class with the data a virtual base.
 *
 *
 * There are two disadvantages of virtual base class:
 *
 * 1. Classes using virtual inheritance are generally larger.
 * 2. Access to data members in virtual base classes is also slower.
 *
 *
 * Advice:
 *
 * 1. Don’t use virtual bases unless you need to.
 * 2. If you must use virtual base classes, try to avoid putting data in them.
*/
class File { };

class InputFile2 : virtual public File { };
class OutputFile2 : virtual public File { };

class IOFile : public InputFile2, public OutputFile2 { };


/**
 * Multiple inheritance does have legitimate uses. One scenario involves combining public inheritance
 * from an Interface class with private inheritance from a class that helps with implementation.
*/
// The class specifies the interface to be implemented
class IPerson
{
    public:
        virtual ~IPerson();

        virtual std::string name() const = 0;
        virtual std::string birthday() const = 0;
};


class DatabaseID { };


// The class has functions useful in implementing IPerson interface
class PersonInfo
{
    public:
        explicit PersonInfo(DatabaseID id);

        virtual ~PersonInfo();

        virtual const char* theName() const;
        virtual const char* theBirthday() const;


    private:
        virtual const char* valueDeliminatorOpen() const;
        virtual const char* valueDeliminatorClose() const;
};


class CPerson : public IPerson, private PersonInfo
{
    public:
        explicit CPerson(DatabaseID id)
            : PersonInfo(id) {}

        // Implementations of the required IPerson member functions
        virtual std::string name() const
        {
            return PersonInfo::theName();
        }

        virtual std::string birthday() const
        {
            return PersonInfo::theBirthday();
        }


    private:
        // Redefinitions of the required IPerson member functions
        const char* valueDelimOpen() const
        {
            return "";
        }

        const char* valueDelimClose() const
        {
            return "";
        }
};