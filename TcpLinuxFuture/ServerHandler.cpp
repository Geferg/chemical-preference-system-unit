#include <iostream>
#include <arpa/inet.h>
#include "../Formatting/Protocol.h"
#include "ServerHandler.h"

namespace Server {
    ServerHandler::ServerHandler() : serverSocket(-1), running(false) {}

    ServerHandler::~ServerHandler() {
        Close();
    }

    bool ServerHandler::Initialize(int port) {
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == -1) {
            std::cerr << "Socket creation failed.\n";
            return false;
        }

        // Set socket options
        int opt = 1;
        if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
            std::cerr << "Set socket option failed.\n";
            close(serverSocket);
            return false;
        }

        sockaddr_in address{};
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        if (bind(serverSocket, (struct sockaddr *)&address, sizeof(address)) < 0) {
            std::cerr << "Bind failed.\n";
            close(serverSocket);
            return false;
        }

        if (listen(serverSocket, SOMAXCONN) < 0) {
            std::cerr << "Listen failed.\n";
            close(serverSocket);
            return false;
        }

        std::cout << "Server initialized on port " << port << ".\n";

        running = true;
        return true;
    }

    void ServerHandler::Listen() {
        listenThread = std::thread([this]() {
            while(running) {
                sockaddr_in clientAddr{};
                socklen_t addrLen = sizeof(clientAddr);
                int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
                if(clientSocket == -1) {
                    std::cerr << "Accept failed.\n";
                    if(!running) break;
                    continue;
                }

                clientThreads.emplace_back(&ServerHandler::HandleClient, this, clientSocket);
            }
        });
    }

    void ServerHandler::Close() {
        running = false;
        close(serverSocket);

        if(listenThread.joinable()) listenThread.join();

        for(auto& t : clientThreads) {
            if(t.joinable()) t.join();
        }
    }

    void ServerHandler::HandleClient(int clientSocket) {
        char buffer[bufferSize];

        while(true) {
            memset(buffer, 0, sizeof(buffer));
            int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
            if (bytesReceived <= 0) {
                if (bytesReceived == 0) {
                    std::cout << "Clean disconnect.\n";
                } else {
                    std::cerr << "Error occurred.\n";
                }
                break;
            }

            //TODO could prioritize some cancellation action here

            std::cout << "Size: " << bytesReceived << "\n";
            dataQueue.push(QueuedData(std::string(buffer, bytesReceived), clientSocket));
        }
    }

    bool ServerHandler::Respond(int clientSocket, const std::string &data) {
        if (clientSocket == -1) {
            std::cerr << "Invalid socket\n";
            return false;
        }

        int bytesSent = send(clientSocket, data.c_str(), data.size(), 0);

        if (bytesSent < 0) {
            std::cerr << "Error occurred.\n";
            return false;
        }

        return true;
    }

    void ServerHandler::CloseClient(int clientSocket) {
        close(clientSocket);
    }
}