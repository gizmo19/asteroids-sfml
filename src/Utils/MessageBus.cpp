#include "../../include/Utils/MessageBus.hpp"

std::unordered_map<MessageType, std::vector<MessageBus::MessageHandler>> MessageBus::subscribers;

void MessageBus::publish(const Message& message) {
    auto it = subscribers.find(message.type);
    if (it != subscribers.end()) {
        for (auto& handler : it->second) {
            handler(message);
        }
    }
}

void MessageBus::subscribe(MessageType type, MessageHandler handler) {
    subscribers[type].push_back(handler);
}

void MessageBus::clear() {
    subscribers.clear();
}
