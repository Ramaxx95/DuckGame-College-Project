#ifndef LOBBY_H
#define LOBBY_H


#include <algorithm>
#include "client.h"
#include <mutex>
#include <condition_variable>

class Lobby {
    std::vector<Client*> clients;
    const int max_size;
    const int host_id;
    bool closed;

    // Para sincronizar jugadores
    std::condition_variable is_lobby_ready;
    std::mutex mtx;


public:
    Lobby(int lobby_size, int host_id);

    // Añade un cliente al lobby.
    bool add_to_lobby(Client* client);

    // Retorna true si el lobby está lleno (posible refactor por tell don't ask).
    bool is_full() const;

    // Retorna una referencia a los clientes que hay en el lobby y lo cierra.
    const std::vector<Client*>& get_lobby_and_close();

    int get_length() const;

    // Devuelve el ID del host.
    int get_host_id() const;

    // Retorna true si el lobby está cerrado.
    bool is_closed() const;

    // Devuelve true si todos los clientes del lobby estas listos
    bool is_ready() const;

    void wait_for_lobby_ready();

    void notify_lobby_ready();

    // Cierra el lobby
    void close();
};



#endif //LOBBY_H
