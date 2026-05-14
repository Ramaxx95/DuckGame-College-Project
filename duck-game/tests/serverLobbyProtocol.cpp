#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "server/lobbyProtocol.h"
#include "server/server.h"
#include "common/sockets.h"

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
    TEST(LobbyProtocolTest, TestSendMapOK) {
        MockSocket skt("8080");
        ServerLobbyProtocol protocol(skt);

        ON_CALL(skt, sendall(testing::_, testing::_, testing::_))
            .WillByDefault([](const void* /*data*/, unsigned int /*size*/, bool* closed) -> int {
                *closed = false;
                return 0;
            });

        std::vector<std::vector<char>> default_map = {
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', '#', '#', '#', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', '4', ' ', ' ', '3', ' '},
            {' ', ' ', '#', '#', '#', ' ', '#', '#', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', '1', ' ', 'W', ' ', ' ', ' ', 'W', ' ', '2'},
            {'#', '#', '#', '#', '#', '#', ' ', ' ', '#', '#'},
            {' ', ' ', '#', ' ', ' ', '#', ' ', ' ', ' ', ' '},
            {' ', ' ', '#', ' ', ' ', '#', ' ', ' ', ' ', ' '},
            {' ', ' ', '#', ' ', ' ', '#', ' ', ' ', ' ', ' '},
            {' ', ' ', '#', ' ', ' ', '#', ' ', ' ', ' ', ' '},
            {' ', ' ', '#', ' ', ' ', '#', ' ', ' ', ' ', ' '},
            {' ', ' ', '#', ' ', ' ', '#', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        };

        Map mock_map(default_map, 1);

        EXPECT_CALL(skt, sendall(testing::_, testing::_, testing::_)).Times(1);

        EXPECT_NO_THROW(protocol.send_map(mock_map));

        testing::Mock::VerifyAndClearExpectations(&skt);
    }

    TEST(LobbyProtocolTest, TestSendMapError) {
        MockSocket skt("8080");
        ServerLobbyProtocol protocol(skt);

        ON_CALL(skt, sendall(testing::_, testing::_, testing::_))
            .WillByDefault([](const void* /*data*/, unsigned int /*size*/, bool* closed) -> int {
                *closed = true;
                return 0;
            });

        Map mock_map;

        EXPECT_CALL(skt, sendall(testing::_, testing::_, testing::_)).Times(1);

        EXPECT_THROW(protocol.send_map(mock_map), std::runtime_error);
    }

    TEST(LobbyProtocolTest, TestReceiveInitialEventOK) {
        MockSocket skt("8080");
        ServerLobbyProtocol protocol(skt);

        ON_CALL(skt, recvall(testing::_, testing::_, testing::_))
            .WillByDefault([](void* data, unsigned int /*size*/, bool* closed) -> int {
                *closed = false;
                *static_cast<uint8_t*>(data) = 0x01;
                return 0;
            });

        EXPECT_CALL(skt, recvall(testing::_, testing::_, testing::_)).Times(1);

        EXPECT_NO_THROW(protocol.receive_initial_event());

        testing::Mock::VerifyAndClearExpectations(&skt);
    }

    TEST(LobbyProtocolTest, TestReceiveInitialEventError) {
        MockSocket skt("8080");
        ServerLobbyProtocol protocol(skt);

        ON_CALL(skt, recvall(testing::_, testing::_, testing::_))
            .WillByDefault([](void* /*data*/, unsigned int /*size*/, bool* closed) -> int {
                *closed = true;
                return 0;
            });

        EXPECT_CALL(skt, recvall(testing::_, testing::_, testing::_)).Times(1);

        EXPECT_THROW(protocol.receive_initial_event(), std::runtime_error);
    }

    TEST(LobbyProtocolTest, TestSendCreateOK) {
        MockSocket skt("8080");
        ServerLobbyProtocol protocol(skt);

        ON_CALL(skt, sendall(testing::_, testing::_, testing::_))
            .WillByDefault([](const void* /*data*/, unsigned int /*size*/, bool* closed) -> int {
                *closed = false;
                return 0;
            });

        std::vector<std::vector<char>> default_map = {
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', '#', '#', '#', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', '4', ' ', ' ', '3', ' '},
            {' ', ' ', '#', '#', '#', ' ', '#', '#', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', '1', ' ', 'W', ' ', ' ', ' ', 'W', ' ', '2'},
            {'#', '#', '#', '#', '#', '#', ' ', ' ', '#', '#'},
            {' ', ' ', '#', ' ', ' ', '#', ' ', ' ', ' ', ' '},
            {' ', ' ', '#', ' ', ' ', '#', ' ', ' ', ' ', ' '},
            {' ', ' ', '#', ' ', ' ', '#', ' ', ' ', ' ', ' '},
            {' ', ' ', '#', ' ', ' ', '#', ' ', ' ', ' ', ' '},
            {' ', ' ', '#', ' ', ' ', '#', ' ', ' ', ' ', ' '},
            {' ', ' ', '#', ' ', ' ', '#', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        };

        Map mock_map(default_map, 1);

        // Se llama para el mapa y para el id
        EXPECT_CALL(skt, sendall(testing::_, testing::_, testing::_)).Times(2);

        EXPECT_NO_THROW(protocol.send_create(mock_map, 1));

        testing::Mock::VerifyAndClearExpectations(&skt);
    }

    TEST(LobbyProtocolTest, TestSendCreateError) {
        MockSocket skt("8080");
        ServerLobbyProtocol protocol(skt);

        ON_CALL(skt, sendall(testing::_, testing::_, testing::_))
            .WillByDefault([](const void* /*data*/, unsigned int /*size*/, bool* closed) -> int {
                *closed = true;
                return 0;
            });

        Map mock_map;

        EXPECT_CALL(skt, sendall(testing::_, testing::_, testing::_)).Times(1);

        EXPECT_THROW(protocol.send_create(mock_map, 1), std::runtime_error);
    }

    TEST(LobbyProtocolTest, TestReceiveReadyOK) {
        MockSocket skt("8080");
        ServerLobbyProtocol protocol(skt);

        ON_CALL(skt, recvall(testing::_, testing::_, testing::_))
            .WillByDefault([](void* data, unsigned int /*size*/, bool* closed) -> int {
                *closed = false;
                *static_cast<uint8_t*>(data) = 0x05;
                return 0;
            });

        EXPECT_CALL(skt, recvall(testing::_, testing::_, testing::_)).Times(1);

        EXPECT_NO_THROW(protocol.receive_ready());

        testing::Mock::VerifyAndClearExpectations(&skt);
    }

    TEST(LobbyProtocolTest, TestReceiveReadyError) {
        MockSocket skt("8080");
        ServerLobbyProtocol protocol(skt);

        ON_CALL(skt, recvall(testing::_, testing::_, testing::_))
            .WillByDefault([](void* /*data*/, unsigned int /*size*/, bool* closed) -> int {
                *closed = true;
                return 0;
            });

        EXPECT_CALL(skt, recvall(testing::_, testing::_, testing::_)).Times(1);

        EXPECT_THROW(protocol.receive_ready(), std::runtime_error);
    }

    TEST(LobbyProtocolTest, TestSuccessJoinOK) {
        MockSocket skt("8080");
        ServerLobbyProtocol protocol(skt);

        ON_CALL(skt, sendall(testing::_, testing::_, testing::_))
            .WillByDefault([](const void* /*data*/, unsigned int /*size*/, bool* closed) -> int {
                *closed = false;
                return 0;
            });

        EXPECT_CALL(skt, sendall(testing::_, testing::_, testing::_)).Times(1);

        EXPECT_NO_THROW(protocol.success_join(1));

        testing::Mock::VerifyAndClearExpectations(&skt);
    }

    TEST(LobbyProtocolTest, TestSuccessJoinError) {
        MockSocket skt("8080");
        ServerLobbyProtocol protocol(skt);

        ON_CALL(skt, sendall(testing::_, testing::_, testing::_))
            .WillByDefault([](const void* /*data*/, unsigned int /*size*/, bool* closed) -> int {
                *closed = true;
                return 0;
            });

        EXPECT_CALL(skt, sendall(testing::_, testing::_, testing::_)).Times(1);

        EXPECT_THROW(protocol.success_join(1), std::runtime_error);
    }
}
