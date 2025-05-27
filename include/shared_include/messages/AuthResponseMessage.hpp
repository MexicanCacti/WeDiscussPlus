#pragma once

#include "MessageInterface.hpp"
#include "MessageFactory.hpp"
#include "ChatroomMessage.hpp"
#include "UserMessage.hpp"
#include <unordered_map>

class AuthResponseMessage : public MessageInterface {
    public:
        AuthResponseMessage() = default;
        AuthResponseMessage(const std::vector<char>& data, size_t& offset);

        void setUserMap(const std::unordered_map<int, std::string>& userMap);
        void setChatrooms(const std::vector<ChatroomData>& chatrooms);
        void setInbox(const std::vector<std::shared_ptr<MessageInterface>>& messages);

        const std::unordered_map<int, std::string>& getUserMap() const { return _userMap; }
        const std::vector<ChatroomData>& getChatrooms() const { return _chatrooms; }
        const std::vector<std::shared_ptr<MessageInterface>>& getInbox() const { return _inbox; }

        std::vector<char> serialize() const override;
        void deserialize(const std::vector<char>& data, size_t& offset) override;
        void printMessage() const override;

    private:
        std::unordered_map<int, std::string> _userMap;  // userID : userName
        std::vector<ChatroomData> _chatrooms;
        std::vector<std::shared_ptr<MessageInterface>> _inbox;
}; 