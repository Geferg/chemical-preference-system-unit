#ifndef TCPDEBUGCLEAN_TCPCONNECTION_H
#define TCPDEBUGCLEAN_TCPCONNECTION_H

#include <winsock2.h>
#include <atomic>
#include <thread>
#include <vector>
#include "ThreadSafeQueue.h"

namespace Tcp {
    class TcpConnection {

    private:
        SOCKET serverSocket;
        std::atomic<bool> running;
        std::vector<std::thread> clientThreads;
        std::thread listenThread;
        void HandleClient(SOCKET clientSocket);

    public:
        const int bufferSize = 14000;

        struct QueuedData {
            std::string data;
            SOCKET clientSocket;

            QueuedData(std::string d, SOCKET s) : data(std::move(d)), clientSocket(s) {};
        };

        ThreadSafeQueue<QueuedData> dataQueue;

        TcpConnection();
        ~TcpConnection();

        bool Initialize(int port);
        void Listen();
        void Close();
        static bool Respond(SOCKET clientSocket, const std::string &data);
        void CloseClient(SOCKET clientSocket);
    };
}

#endif //TCPDEBUGCLEAN_TCPCONNECTION_H
