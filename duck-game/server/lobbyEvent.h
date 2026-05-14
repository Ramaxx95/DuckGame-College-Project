#ifndef LOBBYEVENT_H
#define LOBBYEVENT_H

class LobbyEvent{
    bool is_creation_event;

    public:
        explicit LobbyEvent(bool is_creation_event) : is_creation_event(is_creation_event) {}
        bool isCreationEvent() { return is_creation_event; }
};

#endif // LOBBYEVENT_H
