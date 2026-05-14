#include "acceptor.h"

#include <memory>
#include <utility>

Acceptor::Acceptor(const char* hostname, GameManager& lobby_manager)
    : sk(hostname), lobby_manager(lobby_manager), running(true), id_counter(0) {}

void Acceptor::run() {
    try {
        while (running) {
            // Aqui es donde se tiene que crear el player al que se le enviaran datos del lobby
            try {
                auto player = sk.accept();

                managers.push_back(std::make_unique<ClientManager>
                (std::move(player), lobby_manager, id_counter++));
            } catch (std::exception& e) {
                std::cout << "Se cerro el socket acceptador" << std::endl;
                break;
            }

            managers.back()->start();

            clean_finished_managers();
        }
        running = false;
    } catch (std::exception& e) {
        std::cout << "Error en el acceptor" << std::endl;
    }
}

void Acceptor::clean_finished_managers() {
    std::erase_if(managers,
                  [](const std::unique_ptr<ClientManager>& manager) {
                      if (!manager->is_running()) {
                          manager->join();
                          return true;
                      }
                      return false;
                  });
}

void Acceptor::stop() {
    if (running) {
        running = false;
        sk.shutdown(2);
        sk.close();
    }
}

Acceptor::~Acceptor() {
    stop();
    for (auto& manager : managers) {
        manager->stop();
        manager->join();
    }
    // No es necesaria esta llamada
    managers.clear();
}

