#ifndef TCPDEBUGCLEAN_SERVERHANDLER_H
#define TCPDEBUGCLEAN_SERVERHANDLER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <atomic>
#include <thread>
#include <vector>
#include <cstring>
#include "ClientQueue.h"

namespace Server {

    class ServerHandler {

    private:
        int serverSocket;
        std::atomic<bool> running;
        std::vector<std::thread> clientThreads;
        std::thread listenThread;
        void HandleClient(int clientSocket);

    public:
        const int bufferSize = 14000;

        struct QueuedData {
            std::string data;
            int clientSocket;

            QueuedData(std::string d, int s) : data(std::move(d)), clientSocket(s) {};
        };

        ClientQueue<QueuedData> dataQueue;

        ServerHandler();
        ~ServerHandler();

        bool Initialize(int port);
        void Listen();
        void Close();
        static bool Respond(int clientSocket, const std::string &data);
        void CloseClient(int clientSocket);
    };
}

#endif
