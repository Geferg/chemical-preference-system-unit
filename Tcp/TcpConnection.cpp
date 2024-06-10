#include <iostream>
#include "TcpConnection.h"
#include "../Formatting/Protocol.h"

namespace Tcp {

    TcpConnection::TcpConnection() : serverSocket(INVALID_SOCKET), running(false) {}

    TcpConnection::~TcpConnection() {
        Close();
    }

    bool TcpConnection::Initialize(int port) {
        WSADATA wsaData;

        if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
            std::cerr << "WSA startup failed.\n";
            return false;
        }

        // Create socket
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == INVALID_SOCKET) {
            std::cerr << "Socket creation failed with error: " << WSAGetLastError() << "\n";
            WSACleanup();
            return false;
        }

        // Ensure blocking mode
        u_long mode = 0;
        if(ioctlsocket(serverSocket, FIONBIO, &mode) != 0) {
            std::cerr << "Failed to set blocking mode: " << WSAGetLastError() << "\n";
            closesocket(serverSocket);
            WSACleanup();
            return false;
        }

        // Bind socket
        sockaddr_in address{};
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        if(bind(serverSocket, (struct sockaddr *)&address, sizeof(address)) == SOCKET_ERROR) {
            std::cerr << "Bind failed with error: " << WSAGetLastError() << "\n";
            closesocket(serverSocket);
            WSACleanup();
            return false;
        }

        // Listen for connections
        if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
            std::cerr << "Listen failed with error: " << WSAGetLastError() << "\n";
            closesocket(serverSocket);
            WSACleanup();
            return false;
        }

        std::cout << "Server initialized on port " << port << ".\n";

        running = true;
        return true;
    }

    void TcpConnection::Listen() {
        listenThread = std::thread([this]() {
            while(running) {
                sockaddr_in clientAddr{};
                int addrLen = sizeof(clientAddr);
                SOCKET clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
                if(clientSocket == INVALID_SOCKET) {
                    std::cerr << "Accept failed with error: " << WSAGetLastError() << "\n";
                    if(!running) break;
                    continue;
                }

                clientThreads.emplace_back(&TcpConnection::HandleClient, this, clientSocket);
            };
        });
    }

    void TcpConnection::Close() {
        running = false;
        closesocket(serverSocket);

        if(listenThread.joinable()) listenThread.join();

        for(auto& t : clientThreads) {
            if(t.joinable()) t.join();
        }

        WSACleanup();
        std::cout << "Server closed.\n";
    }

    void TcpConnection::HandleClient(SOCKET clientSocket) {
        char buffer[bufferSize];

        while(true) {
            memset(buffer, 0, sizeof(buffer));
            int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
            if (bytesReceived <= 0) {
                if (bytesReceived == 0) {
                    std::cout << "Clean disconnect.\n";
                }
                else {
                    std::cerr << "Error occurred: " << WSAGetLastError() << "\n";
                }
                break;
            }
            //std::cout << "raw: " << buffer << "\n";

            std::cout << "Size: " << bytesReceived << "\n";
            dataQueue.push(QueuedData(std::string(buffer, bytesReceived), clientSocket));
        }
    }

    bool TcpConnection::Respond(SOCKET clientSocket, const std::string &data) {
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Invalid socket\n";
            return false;
        }

        int bytesSent = send(clientSocket, data.c_str(), data.size(), 0);

        if (bytesSent == SOCKET_ERROR) {
            std::cerr << "Error occurred: " << WSAGetLastError() << "\n";
            return false;
        }

        return true;
    }

    void TcpConnection::CloseClient(SOCKET clientSocket) {
        closesocket(clientSocket);
    }

}