#include <string>
#include <string_view>
/**
 *  Know how to access names in templatized base classes.
*/

// Suppose a message-sending application
class CompanyA
{
    public:
        void sendClearText(std::string_view message);
        void sendEncrypted(std::string_view message);
};


class CompanyB
{
    public:
        void sendClearText(std::string_view message);
        void sendEncrypted(std::string_view message);
};


// Class for holding information used to create a message
class MessageInfo { };


template <typename Company>
class MessageSender
{
    void sendClear(const MessageInfo& info)
    {
        std::string message;

        Company company;

        company.sendClearText(message);
    }
};

/**
 * The problem here is that when compilers encounter the definition for the class template LogMessageSender,
 * they don’t know what class it inherits from. Company is a template parameter, one that won’t be known until later.
 * Without knowing what Company is, there’s no way to know what the class MessageSender<Company> looks like.
 * In particular, there’s no way to know if it has a sendClear() function.
 *
 * We have to somehow disable C++’s “don’t look in templatized base classes” behavior. There are three ways to do this:
*/
// Preface calls to base class functions with “this->
template <typename Company>
class LogMessageSender : public MessageSender<Company>
{
    public:
        void sendClearMessage(const MessageInfo& info)
        {
            this->sendClear(info);
        }
};


// Employ a using declaration
template <typename Company>
class LogMessageSender : public MessageSender<Company>
{
    public:
        using MessageSender<Company>::sendClear();

        void sendClearMessage(const MessageInfo& info)
        {
            sendClear(info);
        }
};


// Explicitly specify that the function being called is in the base class
template <typename Company>
class LogMessageSender : public MessageSender<Company>
{
    public:
        void sendClearMessage(const MessageInfo& info)
        {
            MessageSender<Company>::sendClear(info);
        }
};