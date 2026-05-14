#ifndef CLIENT_H
#define CLIENT_H

#include <common/sockets.h>
#include <vector>
#include "lobbyProtocol.h"
#include "lobbyEvent.h"
#include "common/map.h"

class Client {
    Socket perr;
    bool socket_moved;
    ServerLobbyProtocol protocol;
    const int id;
    int player_number;
    bool ready;


public:
    Client(Socket perr, int id);

    // Retorna un LobbyEvent, que puede ser
    // Crear Lobby o Entrar a un lobby
    LobbyEvent recive();

    // Espera que el cliente seleccione iniciar juego.
    int wait_for_init_game();

    // Notifica al cliente que ya puede iniciar el juego y le envia los datos necesarios
    void notify_init_game(Map map, int size);

    // Notifica al cliente su player_number, que puede ser
    // negativo en caso de que no haya podido entrar a un lobby.
    void notify_join_res();

    // Envia un mapa al cliente.
    void send_map(Map map);

    // Envia un mapa al cliente.
    void send_map_ids(std::vector<Map> maps);

    // Libera el socket del cliente.
    // Warning! luego de esta accion el cliente ya no podra enviar mensajes.
    Socket&& release_socket();

    // Retorna el id del cliente.
    int get_id() const;

    int get_duck_id() const;

    // setea un numero al cliente
    void set_player_number(int number);

    // Espera que el cliente seleccione ready.
    void wait_ready();

    // Retorna true si el jugador esta listo.
    bool is_ready() const;

    void notify_lobby_ready();

    void end_connection();
};



#endif //CLIENT_H
