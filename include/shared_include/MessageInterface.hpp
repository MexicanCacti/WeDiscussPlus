#ifndef MESSAGE_INTERFACE_HPP
#define MESSAGE_INTERFACE_HPP

#include "MessageType.hpp"
#include <string>
#include <vector>

class MessageInterface {
public:
    virtual ~MessageInterface() = default;

    // Getters
    virtual std::string getMessageContents() const = 0;
    virtual std::string getToUsername() const = 0;
    virtual int getToUserID() const = 0;
    virtual std::string getFromUsername() const = 0;
    virtual int getFromUserID() const = 0;
    virtual int getToChatroomID() const = 0;
    virtual int getFromChatroomID() const = 0;
    virtual MessageType getMessageType() const = 0;

    // Setters
    virtual void setMessageContents(const std::string& contents) = 0;
    virtual void setToUsername(const std::string& username) = 0;
    virtual void setToUserID(int id) = 0;
    virtual void setFromUsername(const std::string& username) = 0;
    virtual void setFromUserID(int id) = 0;
    virtual void setToChatroomID(int id) = 0;
    virtual void setFromChatroomID(int id) = 0;
    virtual void setMessageType(MessageType type) = 0;

    // Serialization
    virtual std::vector<char> serialize() const = 0;
    virtual void printMessage() const = 0;
};

#endif 