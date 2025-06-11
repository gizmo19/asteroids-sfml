#pragma once
#include <functional>
#include <vector>
#include <unordered_map>
#include <any>

enum class MessageType {
    AsteroidDestroyed,
    BulletFired,
    ShipDestroyed,
    GameOver,
    ScoreUpdate,
    WeaponPickedUp,
    WeaponExpired,
    RocketExplosion,
    ExplosionTriggered
};

struct Message {
    MessageType type;
    void* sender;
    std::any payload;
};

class MessageBus {
public:
    using MessageHandler = std::function<void(const Message&)>;

    static void publish(const Message& message);
    static void subscribe(MessageType type, MessageHandler handler);
    static void clear();

private:
    static std::unordered_map<MessageType, std::vector<MessageHandler>> subscribers;
};
