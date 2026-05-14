#ifndef SERVER_H
#define SERVER_H

class Server {
    private:
        char* port;
    public:

        /**
         * @brief Construct a new Server object
         * 
         * @param port port to listen to
         */
        explicit Server(char* port);

        /**
         * @brief Initialize the server
         * 
         */
        void Init();
};

#endif // SERVER_H
