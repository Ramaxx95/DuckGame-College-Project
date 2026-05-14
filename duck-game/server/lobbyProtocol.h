#ifndef LOBBYPROTOCOL_H
#define LOBBYPROTOCOL_H

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "common/map.h"
#include "common/sockets.h"

#include "lobbyEvent.h"

template <typename T>
void print_bytes(const T& data, const std::string& type, const std::string& function) {
    // Obtener un puntero a los bytes del objeto
    const uint8_t* byteData = reinterpret_cast<const uint8_t*>(&data);
    size_t size = sizeof(data);

    std::cout << "[DEBUG] Bytes " << type << " " << function << ": ";
    for (size_t i = 0; i < size; ++i) {
        // Imprimir cada byte en formato hexadecimal
        std::cout << "0x" << std::setw(2) << std::setfill('0') << std::hex
                  << static_cast<int>(byteData[i]) << " ";
    }
    std::cout << std::dec << std::endl;  // Volver a formato decimal
}

class ServerLobbyProtocol {
private:
    Socket& skt;
    bool was_closed;

public:
    /**
     * @brief Construct a new Lobby Protocol object
     *
     * @param peer reference to the socket
     */
    explicit ServerLobbyProtocol(Socket& peer);

    /**
     * @brief Receive the initial event
     *
     * Receive the initial event to know if the player is creating a lobby
     * or joining a lobby.
     *
     * @return LobbyEvent
     */
    LobbyEvent receive_initial_event();

    /**
     * @brief Send a create lobby request
     *
     * send all information about lobby creation and assign a duck number
     * to the player who created the lobby. In addition, send lobby id.
     *
     * @param maps vector of maps
     * @param id lobby id
     * @param duck_number duck number
     */
    void send_create(Map map, int duck_number);

    /**
     * @brief Send a join lobby request
     *
     * send the lobby id and the duck number to join the lobby to a player who
     * does not create the lobby.
     *
     * @param id lobby id
     * @param duck_number duck number
     */
    void success_join(int duck_number);

    /**
     * @brief Receive a ready signal
     *
     * receive the lobby id and the duck number who is ready to start the game.
     *
     * @param id lobby id
     * @param duck_number duck number
     */
    int receive_ready();

    /**
     * @brief Initialize the game
     *
     * Receive the lobby id and the map id to start the game.
     *
     * @param id lobby id
     * @param map map
     */
    int receive_init_game();


    void send_init_game(Map map, int size);

    /**
     * @brief Send a map
     *
     * Send the map to the client.
     * Convert the map to a vector of uint8_t and send it.
     * use htons to convert the size of the map to network byte order.
     *
     * @param map object to send
     * @return * void
     */
    void send_map(Map map);

    void send_maps(std::vector<Map> maps);

    // Notifica al cliente que todos los clientes de su lobby estan ready
    // falta implementar.
    void send_all_clients_ready();

    // Cierra la coneccion con el cliente de manera forzosa
    void close_connection();
};

#endif  // LOBBYPROTOCOL_H
