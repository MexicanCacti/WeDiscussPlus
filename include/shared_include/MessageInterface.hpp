#ifndef MESSAGE_INTERFACE_HPP
#define MESSAGE_INTERFACE_HPP

#include "MessageType.hpp"
#include <string>
#include <vector>

class MessageInterface {
protected:
    bool _success_bit;
    std::string _messageContents;
    std::string _to_user_name;
    int _to_user_id;
    std::string _from_user_name;
    int _from_user_id;
    int _to_chatroom_id;
    int _from_chatroom_id;
    MessageType _messageType;

    static void serializeInt(std::vector<char>& buffer, int val);
    static int deserializeInt(const std::vector<char>& buffer, size_t& offset);
    static void serializeString(std::vector<char>& buffer, const std::string& str);
    static std::string deserializeString(const std::vector<char>& buffer, size_t& offset);

public:
    MessageInterface() : 
        _success_bit(true),
        _messageContents(""), 
        _to_user_name(""), 
        _to_user_id(-1), 
        _from_user_name(""), 
        _from_user_id(-1), 
        _to_chatroom_id(-1), 
        _from_chatroom_id(-1), 
        _messageType(MessageType::UNDEFINED) {}

    virtual ~MessageInterface() = default;

    inline bool getSuccessBit() const { return _success_bit; }
    inline std::string getMessageContents() const { return _messageContents; }
    inline std::string getToUsername() const { return _to_user_name; }
    inline int getToUserID() const { return _to_user_id; }
    inline std::string getFromUsername() const { return _from_user_name; }
    inline int getFromUserID() const { return _from_user_id; }
    inline int getToChatroomID() const { return _to_chatroom_id; }
    inline int getFromChatroomID() const { return _from_chatroom_id; }
    inline MessageType getMessageType() const { return _messageType; }
    static std::string messageTypeToString(const MessageType& messageType);

    virtual std::vector<char> serialize() const = 0;
    virtual void printMessage() const = 0;
};

#endif 