#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "client/lobbyProtocol.h"
#include "common/sockets.h"
#include <arpa/inet.h>
#include <stdexcept>
#include <vector>

using ::testing::HasSubstr;
using ::testing::ThrowsMessage;
using ::testing::AllOf;

// Mock cook book
// https://google.github.io/googletest/gmock_cook_book.html
class MockSocket : public Socket {
public:
    explicit MockSocket(const char* port): Socket(port) {}
    MOCK_METHOD(int, sendall, (const void* data, unsigned int sz, bool* was_closed), (override));
    MOCK_METHOD(int, recvall, (void* data, unsigned int sz, bool* was_closed), (override));
    MOCK_METHOD(void, shutdown, (int how), (override));
    MOCK_METHOD(int, close, (), (override));
};

namespace {
    TEST(LobbyProtocolTest, ReceiveCreateSuccessOK) {
        // Uso el skt del cliente porque no tengo un server levantado
        // Es irrelevante para el test ya que estoy mockeando los metodos
        MockSocket skt("8080");
        LobbyProtocol protocol(skt);

        // Datos para el test
        uint8_t duck_number_mock = 1;
        uint16_t map_len_mock = 1200;  // Simula la longitud del mapa en bytes (30x40 mapa).
        std::vector<uint8_t> serialized_map(map_len_mock, '.'); // Simula un mapa lleno de '.'.

        // Mock de recvall para recibir el número del pato.
        EXPECT_CALL(skt, recvall(testing::_, testing::Eq(sizeof(uint8_t)), testing::_))
            .WillOnce([&duck_number_mock](void* data, size_t, bool* closed) -> int {
                *closed = false;
                *static_cast<uint8_t*>(data) = duck_number_mock;
                return sizeof(uint8_t);
            });

        // Mock de recvall para recibir la longitud del mapa.
        EXPECT_CALL(skt, recvall(testing::_, testing::Eq(sizeof(uint16_t)), testing::_))
            .WillOnce([&map_len_mock](void* data, size_t, bool* closed) -> int {
                *closed = false;
                uint16_t len_net_order = htons(map_len_mock);
                memcpy(data, &len_net_order, sizeof(len_net_order));
                return sizeof(uint16_t);
            });

        // Mock de recvall para recibir el contenido del mapa.
        EXPECT_CALL(skt, recvall(testing::_, testing::Eq(map_len_mock), testing::_))
            .WillOnce([&serialized_map](void* data, size_t, bool* closed) -> int {
                *closed = false;
                std::copy(serialized_map.begin(), serialized_map.end(), static_cast<uint8_t*>(data));
                return serialized_map.size();
            });

        int duck_number = 0;

        // Verificaciones
        EXPECT_NO_THROW({
            Map received_map = protocol.receive_create_success(duck_number);
            EXPECT_EQ(duck_number, duck_number_mock);

            std::vector<uint8_t> received_serialized_map = received_map.serialize();
            EXPECT_EQ(received_serialized_map, serialized_map);
        });
    }

    TEST(LobbyProtocolTest, ReceiveCreateSuccessError) {
        // Uso el skt del cliente porque no tengo un server levantado
        // Es irrelevante para el test ya que estoy mockeando los metodos
        MockSocket skt("8080");
        LobbyProtocol protocol(skt);

        // Mock de recvall para recibir el número del pato.
        EXPECT_CALL(skt, recvall(testing::_, testing::Eq(sizeof(uint8_t)), testing::_))
            .WillOnce([](void* /*data*/, size_t, bool* closed) -> int {
                *closed = true;
                return 0;
            });

        int duck_number = 0;
        EXPECT_THROW(protocol.receive_create_success(duck_number), std::runtime_error);
    }

    TEST(LobbyProtocolTest, SendCreateOK) {
        // Uso el skt del cliente porque no tengo un server levantado
        // Es irrelevante para el test ya que estoy mockeando los metodos
        MockSocket skt("8080");
        LobbyProtocol protocol(skt);

        ON_CALL(skt, sendall(testing::_, testing::_, testing::_))
            .WillByDefault([](const void* /*data*/, unsigned int /*size*/, bool* closed) -> int {
                *closed = false;
                return 0;
            });

        EXPECT_CALL(skt, sendall(testing::_, testing::_, testing::_)).Times(1);

        EXPECT_NO_THROW(protocol.send_create());
    }

    TEST(LobbyProtocolTest, SendCreateError) {
        // Uso el skt del cliente porque no tengo un server levantado
        // Es irrelevante para el test ya que estoy mockeando los metodos
        MockSocket skt("8080");
        LobbyProtocol protocol(skt);

        ON_CALL(skt, sendall(testing::_, testing::_, testing::_))
            .WillByDefault([](const void* /*data*/, unsigned int /*size*/, bool* closed) -> int {
                *closed = true;
                return 0;
            });

        EXPECT_CALL(skt, sendall(testing::_, testing::_, testing::_)).Times(1);

        EXPECT_THROW(protocol.send_create(), std::runtime_error);
    }

    TEST(LobbyProtocolTest, RequestJoinOK) {
        // Uso el skt del cliente porque no tengo un server levantado
        // Es irrelevante para el test ya que estoy mockeando los metodos
        MockSocket skt("8080");
        LobbyProtocol protocol(skt);

        ON_CALL(skt, sendall(testing::_, testing::_, testing::_))
            .WillByDefault([](const void* /*data*/, unsigned int /*size*/, bool* closed) -> int {
                *closed = false;
                return 0;
            });

        EXPECT_CALL(skt, sendall(testing::_, testing::_, testing::_)).Times(1);

        EXPECT_NO_THROW(protocol.request_join());
    }

    TEST(LobbyProtocolTest, RequestJoinError) {
        // Uso el skt del cliente porque no tengo un server levantado
        // Es irrelevante para el test ya que estoy mockeando los metodos
        MockSocket skt("8080");
        LobbyProtocol protocol(skt);

        ON_CALL(skt, sendall(testing::_, testing::_, testing::_))
            .WillByDefault([](const void* /*data*/, unsigned int /*size*/, bool* closed) -> int {
                *closed = true;
                return 0;
            });

        EXPECT_CALL(skt, sendall(testing::_, testing::_, testing::_)).Times(1);

        EXPECT_THROW(protocol.request_join(), std::runtime_error);
    }

    TEST(LobbyProtocolTest, SendReadyOK) {
        // Uso el skt del cliente porque no tengo un server levantado
        // Es irrelevante para el test ya que estoy mockeando los metodos
        MockSocket skt("8080");
        LobbyProtocol protocol(skt);

        ON_CALL(skt, sendall(testing::_, testing::_, testing::_))
            .WillByDefault([](const void* /*data*/, unsigned int /*size*/, bool* closed) -> int {
                *closed = false;
                return 0;
            });

        EXPECT_CALL(skt, sendall(testing::_, testing::_, testing::_)).Times(1);

        EXPECT_NO_THROW(protocol.send_ready());
    }

    TEST(LobbyProtocolTest, SendReadyError) {
        // Uso el skt del cliente porque no tengo un server levantado
        // Es irrelevante para el test ya que estoy mockeando los metodos
        MockSocket skt("8080");
        LobbyProtocol protocol(skt);

        ON_CALL(skt, sendall(testing::_, testing::_, testing::_))
            .WillByDefault([](const void* /*data*/, unsigned int /*size*/, bool* closed) -> int {
                *closed = true;
                return 0;
            });

        EXPECT_CALL(skt, sendall(testing::_, testing::_, testing::_)).Times(1);

        EXPECT_THROW(protocol.send_ready(), std::runtime_error);
    }

    TEST(LobbyProtocolTest, InitGameOK) {
        // Uso el skt del cliente porque no tengo un server levantado
        // Es irrelevante para el test ya que estoy mockeando los metodos
        MockSocket skt("8080");
        LobbyProtocol protocol(skt);

        ON_CALL(skt, sendall(testing::_, testing::_, testing::_))
            .WillByDefault([](const void* /*data*/, unsigned int /*size*/, bool* closed) -> int {
                *closed = false;
                return 0;
            });

        EXPECT_CALL(skt, sendall(testing::_, testing::_, testing::_)).Times(1);

        EXPECT_NO_THROW(protocol.init_game(1));
    }

    TEST(LobbyProtocolTest, InitGameError) {
        // Uso el skt del cliente porque no tengo un server levantado
        // Es irrelevante para el test ya que estoy mockeando los metodos
        MockSocket skt("8080");
        LobbyProtocol protocol(skt);

        ON_CALL(skt, sendall(testing::_, testing::_, testing::_))
            .WillByDefault([](const void* /*data*/, unsigned int /*size*/, bool* closed) -> int {
                *closed = true;
                return 0;
            });

        EXPECT_CALL(skt, sendall(testing::_, testing::_, testing::_)).Times(1);

        EXPECT_THROW(protocol.init_game(1), std::runtime_error);
    }

    TEST(LobbyProtocolTest, ReceiveJoinSuccessOK) {
        // Uso el skt del cliente porque no tengo un server levantado
        // Es irrelevante para el test ya que estoy mockeando los metodos
        MockSocket skt("8080");
        LobbyProtocol protocol(skt);

        int duck_number_mock = 1;

        ON_CALL(skt, recvall(testing::_, testing::_, testing::_))
            .WillByDefault([&duck_number_mock](void* data, size_t, bool* closed) -> int {
                *closed = false;
                *static_cast<uint8_t*>(data) = duck_number_mock;
                return sizeof(uint8_t);
            });

        EXPECT_CALL(skt, recvall(testing::_, testing::_, testing::_)).Times(1);

        EXPECT_NO_THROW({
            int player_id = 0;
            protocol.reveive_join_success(player_id);
            EXPECT_EQ(player_id, duck_number_mock);
        });
    }

    TEST(LobbyProtocolTest, ReceiveJoinSuccessError) {
        // Uso el skt del cliente porque no tengo un server levantado
        // Es irrelevante para el test ya que estoy mockeando los metodos
        MockSocket skt("8080");
        LobbyProtocol protocol(skt);

        ON_CALL(skt, recvall(testing::_, testing::_, testing::_))
            .WillByDefault([](void* /*data*/, size_t, bool* closed) -> int {
                *closed = true;
                return 0;
            });

        EXPECT_CALL(skt, recvall(testing::_, testing::_, testing::_)).Times(1);

        int player_id = 0;
        EXPECT_THROW(protocol.reveive_join_success(player_id), std::runtime_error);
    }

    TEST(LobbyProtocolTest, ReceiveMapIdsOK) {
        // Uso el skt del cliente porque no tengo un server levantado
        // Es irrelevante para el test ya que estoy mockeando los metodos
        MockSocket skt("8080");
        LobbyProtocol protocol(skt);

        uint16_t len = 3;
        std::vector<uint8_t> map_ids = {0, 1, 2};

        ON_CALL(skt, recvall(testing::_, testing::Eq(sizeof(uint16_t)), testing::_))
            .WillByDefault([&len](void* data, size_t, bool* closed) -> int {
                *closed = false;
                uint16_t len_net_order = htons(len);
                memcpy(data, &len_net_order, sizeof(len_net_order));
                return sizeof(len_net_order);
            });

        ON_CALL(skt, recvall(testing::_, testing::Eq(sizeof(uint8_t) * len), testing::_))
            .WillByDefault([&len, &map_ids](void* data, size_t, bool* closed) -> int {
                *closed = false;
                std::copy(map_ids.begin(), map_ids.end(), static_cast<uint8_t*>(data));
                return map_ids.size();
            });

        EXPECT_CALL(skt, recvall(testing::_, testing::_, testing::_)).Times(2);

        EXPECT_NO_THROW({
            std::vector<int> received_map_ids = protocol.receive_map_ids();
            EXPECT_EQ(received_map_ids.size(), map_ids.size());
            EXPECT_EQ(received_map_ids, std::vector<int>(map_ids.begin(), map_ids.end()));
        });
    }
}
